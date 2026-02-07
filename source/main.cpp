//
// Copyright (C) YuqiaoZhang(HanetakaChou)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
//

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cassert>
#include <algorithm>
#include "user_input_model.h"
#include "user_input_controller.h"
#include "../thirdparty/McRT-Malloc/include/mcrt_vector.h"
#include "../thirdparty/McRT-Malloc/include/mcrt_string.h"
#include "../thirdparty/McRT-Malloc/include/mcrt_set.h"
#include "../thirdparty/McRT-Malloc/include/mcrt_tick_count.h"
#include "../thirdparty/Brioche-Analytic-Rendering-Interface/include/brx_anari.h"
#include "../thirdparty/Brioche-Window-System-Integration/include/brx_wsi.h"

extern bool internal_platform_get_open_file_name(uint32_t filter_count, char const *const *filter_names, char const *const *filter_specs, int &inout_file_type_index, mcrt_string *out_file_name, uint64_t *out_file_timestamp, mcrt_vector<uint8_t> *out_file_data);

extern bool internal_platform_get_file_timestamp_and_data(char const *file_name, uint64_t *out_file_timestamp, mcrt_vector<uint8_t> *out_file_data);

struct wsi_state_t
{
    brx_anari_device *m_anari_device;
    int32_t m_window_width;
    int32_t m_window_height;
    float m_window_width_scale;
    float m_window_height_scale;
    double m_tick_count_resolution;
    uint64_t m_tick_count_previous_frame;
    ui_model_t m_ui_model;
    ui_controller_t m_ui_controller;
};

static void *internal_imgui_malloc_wrapper(size_t size, void *user_data);
static void internal_imgui_free_wrapper(void *ptr, void *user_data);

static void internal_key_press_handler(void *handler_context, int key, bool shift_key, bool caps_key, bool ctrl_key, bool alt_key);
static void internal_key_release_handler(void *handler_context, int key, bool shift_key, bool caps_key, bool ctrl_key, bool alt_key);
static void internal_button_press_handler(void *handler_context, int button, int x, int y);
static void internal_button_release_handler(void *handler_context, int button, int x, int y);
static void internal_scroll_up_handler(void *handler_context, int x, int y);
static void internal_scroll_down_handler(void *handler_context, int x, int y);
static void internal_motion_handler(void *handler_context, int x, int y, bool left_button, bool middle_button, bool right_button);
static void internal_resize_handler(void *handler_context, int width, int height, float width_scale, float height_scale);

static inline BRX_ANARI_MORPH_TARGET_NAME const wrap(BRX_MOTION_MORPH_TARGET_NAME const morph_target_name)
{
    static_assert(static_cast<uint32_t>(BRX_ANARI_MORPH_TARGET_NAME_MMD_BROW_HAPPY) == static_cast<uint32_t>(BRX_MOTION_MORPH_TARGET_NAME_MMD_BROW_HAPPY), "");
    static_assert(static_cast<uint32_t>(BRX_ANARI_MORPH_TARGET_NAME_MMD_BROW_ANGRY) == static_cast<uint32_t>(BRX_MOTION_MORPH_TARGET_NAME_MMD_BROW_ANGRY), "");
    static_assert(static_cast<uint32_t>(BRX_ANARI_MORPH_TARGET_NAME_MMD_BROW_SAD) == static_cast<uint32_t>(BRX_MOTION_MORPH_TARGET_NAME_MMD_BROW_SAD), "");
    static_assert(static_cast<uint32_t>(BRX_ANARI_MORPH_TARGET_NAME_MMD_BROW_SURPRISED) == static_cast<uint32_t>(BRX_MOTION_MORPH_TARGET_NAME_MMD_BROW_SURPRISED), "");
    static_assert(static_cast<uint32_t>(BRX_ANARI_MORPH_TARGET_NAME_MMD_EYE_BLINK) == static_cast<uint32_t>(BRX_MOTION_MORPH_TARGET_NAME_MMD_EYE_BLINK), "");
    static_assert(static_cast<uint32_t>(BRX_ANARI_MORPH_TARGET_NAME_MMD_EYE_BLINK_L) == static_cast<uint32_t>(BRX_MOTION_MORPH_TARGET_NAME_MMD_EYE_BLINK_L), "");
    static_assert(static_cast<uint32_t>(BRX_ANARI_MORPH_TARGET_NAME_MMD_EYE_BLINK_R) == static_cast<uint32_t>(BRX_MOTION_MORPH_TARGET_NAME_MMD_EYE_BLINK_R), "");
    static_assert(static_cast<uint32_t>(BRX_ANARI_MORPH_TARGET_NAME_MMD_EYE_HAPPY) == static_cast<uint32_t>(BRX_MOTION_MORPH_TARGET_NAME_MMD_EYE_HAPPY), "");
    static_assert(static_cast<uint32_t>(BRX_ANARI_MORPH_TARGET_NAME_MMD_EYE_ANGRY) == static_cast<uint32_t>(BRX_MOTION_MORPH_TARGET_NAME_MMD_EYE_ANGRY), "");
    static_assert(static_cast<uint32_t>(BRX_ANARI_MORPH_TARGET_NAME_MMD_EYE_SAD) == static_cast<uint32_t>(BRX_MOTION_MORPH_TARGET_NAME_MMD_EYE_SAD), "");
    static_assert(static_cast<uint32_t>(BRX_ANARI_MORPH_TARGET_NAME_MMD_EYE_SURPRISED) == static_cast<uint32_t>(BRX_MOTION_MORPH_TARGET_NAME_MMD_EYE_SURPRISED), "");
    static_assert(static_cast<uint32_t>(BRX_ANARI_MORPH_TARGET_NAME_MMD_MOUTH_A) == static_cast<uint32_t>(BRX_MOTION_MORPH_TARGET_NAME_MMD_MOUTH_A), "");
    static_assert(static_cast<uint32_t>(BRX_ANARI_MORPH_TARGET_NAME_MMD_MOUTH_I) == static_cast<uint32_t>(BRX_MOTION_MORPH_TARGET_NAME_MMD_MOUTH_I), "");
    static_assert(static_cast<uint32_t>(BRX_ANARI_MORPH_TARGET_NAME_MMD_MOUTH_U) == static_cast<uint32_t>(BRX_MOTION_MORPH_TARGET_NAME_MMD_MOUTH_U), "");
    static_assert(static_cast<uint32_t>(BRX_ANARI_MORPH_TARGET_NAME_MMD_MOUTH_E) == static_cast<uint32_t>(BRX_MOTION_MORPH_TARGET_NAME_MMD_MOUTH_E), "");
    static_assert(static_cast<uint32_t>(BRX_ANARI_MORPH_TARGET_NAME_MMD_MOUTH_O) == static_cast<uint32_t>(BRX_MOTION_MORPH_TARGET_NAME_MMD_MOUTH_O), "");
    static_assert(static_cast<uint32_t>(BRX_ANARI_MORPH_TARGET_NAME_MMD_MOUTH_HAPPY) == static_cast<uint32_t>(BRX_MOTION_MORPH_TARGET_NAME_MMD_MOUTH_HAPPY), "");
    static_assert(static_cast<uint32_t>(BRX_ANARI_MORPH_TARGET_NAME_MMD_MOUTH_ANGRY) == static_cast<uint32_t>(BRX_MOTION_MORPH_TARGET_NAME_MMD_MOUTH_ANGRY), "");
    static_assert(static_cast<uint32_t>(BRX_ANARI_MORPH_TARGET_NAME_MMD_MOUTH_SAD) == static_cast<uint32_t>(BRX_MOTION_MORPH_TARGET_NAME_MMD_MOUTH_SAD), "");
    static_assert(static_cast<uint32_t>(BRX_ANARI_MORPH_TARGET_NAME_MMD_MOUTH_SURPRISED) == static_cast<uint32_t>(BRX_MOTION_MORPH_TARGET_NAME_MMD_MOUTH_SURPRISED), "");
    static_assert(static_cast<uint32_t>(BRX_ANARI_MORPH_TARGET_NAME_MMD_COUNT) == static_cast<uint32_t>(BRX_MOTION_MORPH_TARGET_NAME_MMD_COUNT), "");
    return static_cast<BRX_ANARI_MORPH_TARGET_NAME const>(morph_target_name);
}

static inline brx_anari_rigid_transform const *wrap(brx_motion_rigid_transform const *rigid_transform)
{
    static_assert(sizeof(brx_anari_rigid_transform) == sizeof(brx_motion_rigid_transform), "");
    static_assert(offsetof(brx_anari_rigid_transform, m_rotation) == offsetof(brx_motion_rigid_transform, m_rotation), "");
    static_assert(offsetof(brx_anari_rigid_transform, m_translation) == offsetof(brx_motion_rigid_transform, m_translation), "");
    return reinterpret_cast<brx_anari_rigid_transform const *>(rigid_transform);
}

#if defined(__GNUC__)

#if defined(__linux__)

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "../thirdparty/Brioche-ImGui/imgui.h"
#include "../thirdparty/Brioche-ImGui/backends/imgui_impl_glut.h"

int main(int argc, char *argv[])

#elif defined(__MACH__)

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "../thirdparty/Brioche-ImGui/imgui.h"
#include "../thirdparty/Brioche-ImGui/backends/imgui_impl_osx.h"

extern "C" void ImGui_ImplOSX_Platform_SetImeData(ImGuiContext *, ImGuiViewport *viewport, ImGuiPlatformImeData *data);

int main(int argc, char *argv[])

#else
#error Unknown Platform
#endif

#elif defined(_MSC_VER)

#define NOMINMAX 1
#define WIN32_LEAN_AND_MEAN 1
#include <sdkddkver.h>
#include <Windows.h>

#include "../thirdparty/Brioche-ImGui/imgui.h"
#include "../thirdparty/Brioche-ImGui/backends/imgui_impl_win32.h"
#include "../thirdparty/libiconv/include/iconv.h"

#ifndef NDEBUG
int wmain(int argc, wchar_t *argv[], wchar_t *envp[])
#else
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
#endif

#else
#error Unknown Compiler
#endif
{
    // use high priority on Windows to make the launch faster
    // we do NOT have the access right to do this on Linux
#if defined(__GNUC__)
#if defined(__linux__)
#elif defined(__MACH__)
#else
#error Unknown Platform
#endif
#elif defined(_MSC_VER)

    {
        HANDLE current_process = GetCurrentProcess();

        BOOL res_set_priority_class = SetPriorityClass(current_process, HIGH_PRIORITY_CLASS);
        assert(FALSE != res_set_priority_class);

        HANDLE current_thread = GetCurrentThread();

        BOOL res_set_Thread_priority = SetThreadPriority(current_thread, THREAD_PRIORITY_HIGHEST);
        assert(FALSE != res_set_Thread_priority);
    }
#else
#error Unknown Compiler
#endif

    // Vulkan Validation Layer
#if defined(__GNUC__)
#if defined(__linux__)
#ifndef NDEBUG
    {
        char dir_name[4096] = {};
        {
            ssize_t res_read_link = readlink("/proc/self/exe", dir_name, (sizeof(dir_name) / sizeof(dir_name[0])));
            assert(-1 != res_read_link);

            assert(res_read_link < (sizeof(dir_name) / sizeof(dir_name[0])));

            for (ssize_t index_plus_one = res_read_link; index_plus_one > 0; --index_plus_one)
            {
                ssize_t index = index_plus_one - 1;

                if ('/' != dir_name[index])
                {
                    dir_name[index] = '\0';
                }
                else
                {
                    break;
                }
            }
        }

        // We assume that the "VkLayer_khronos_validation.json" is at the same directory of the executable file
        int res_set_env_vk_layer_path = setenv("VK_LAYER_PATH", dir_name, 1);
        assert(0 == res_set_env_vk_layer_path);
    }
#endif
#elif defined(__MACH__)
    // Molten VK
#else
#error Unknown Platform
#endif
#elif defined(_MSC_VER)
#ifndef NDEBUG
    {
        // We assume that the "VkLayer_khronos_validation.json" is at the same directory of the executable file
        WCHAR file_name[4096];
        DWORD const res_get_file_name = GetModuleFileNameW(NULL, file_name, sizeof(file_name) / sizeof(file_name[0]));
        assert(0U != res_get_file_name);

        for (int i = (res_get_file_name - 1); i > 0; --i)
        {
            if (L'\\' == file_name[i])
            {
                file_name[i] = L'\0';
                break;
            }
        }

        BOOL const res_set_environment_variable = SetEnvironmentVariableW(L"VK_LAYER_PATH", file_name);
        assert(FALSE != res_set_environment_variable);
    }
#endif

#else
#error Unknown Compiler
#endif

    wsi_state_t wsi_state = wsi_state_t{
        NULL,
        1280,
        720,
        1.0F,
        1.0F,
        1.0 / static_cast<double>(mcrt_tick_count_per_second()),
        mcrt_tick_count_now()};

    brx_wsi_init_connection();

    // ImGui_ImplWin32_EnableDpiAwareness();

    brx_wsi_init_main_window(
        "Brioche Puppet",
        internal_key_press_handler,
        internal_key_release_handler,
        internal_button_press_handler,
        internal_button_release_handler,
        internal_scroll_up_handler,
        internal_scroll_down_handler,
        internal_motion_handler,
        internal_resize_handler,
        wsi_state.m_window_width,
        wsi_state.m_window_height,
        &wsi_state);

    brx_wsi_get_main_window_scale(&wsi_state.m_window_width_scale, &wsi_state.m_window_height_scale);

    {
        IMGUI_CHECKVERSION();

        ImGui::SetAllocatorFunctions(internal_imgui_malloc_wrapper, internal_imgui_free_wrapper);

        ImGui::CreateContext();

        ImGuiIO &io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

        ImGui::StyleColorsDark();
    }

#if defined(__GNUC__)
#if defined(__linux__)
    ImGui_ImplGLUT_Init();
    ImGui_ImplGLUT_ReshapeFunc(wsi_state.m_window_width, wsi_state.m_window_height);
#elif defined(__MACH__)
    // Molten VK
    ImGui_ImplOSX_Init(ImGui_ImplOSX_Platform_SetImeData);
    ImGui_ImplOSX_ReshapeFunc(wsi_state.m_window_width, wsi_state.m_window_height, wsi_state.m_window_width_scale, wsi_state.m_window_height_scale);
#else
#error Unknown Platform
#endif
#elif defined(_MSC_VER)
    ImGui_ImplWin32_Init(brx_wsi_get_main_window());
#else
#error Unknown Compiler
#endif

    wsi_state.m_anari_device = brx_anari_new_device(brx_wsi_get_connection());

    ui_model_init(wsi_state.m_anari_device, &wsi_state.m_ui_model);

    ui_controller_init(&wsi_state.m_ui_model, &wsi_state.m_ui_controller);

    wsi_state.m_anari_device->frame_attach_window(brx_wsi_get_main_window(), 1.0F / wsi_state.m_window_width_scale, 1.0F / wsi_state.m_window_height_scale);

    brx_wsi_show_main_window();

    brx_wsi_run_main_loop(
        [](void *tick_context) -> bool
        {
            wsi_state_t &wsi_state = (*static_cast<wsi_state_t *>(tick_context));

            if ((wsi_state.m_window_width > 0) && (wsi_state.m_window_height > 0) && (wsi_state.m_window_width_scale > 1E-3F) && (wsi_state.m_window_height_scale > 1E-3F))
            {
                float interval_time;
                {
                    uint64_t const tick_count_current_frame = mcrt_tick_count_now();
                    interval_time = static_cast<float>(static_cast<double>(tick_count_current_frame - wsi_state.m_tick_count_previous_frame) * wsi_state.m_tick_count_resolution);
                    wsi_state.m_tick_count_previous_frame = tick_count_current_frame;
                }

                // User Camera
                user_camera_simulate(interval_time, &wsi_state.m_ui_model, &wsi_state.m_ui_controller);

                // UI
                {
#if defined(__GNUC__)
#if defined(__linux__)
                    ImGui_ImplGLUT_NewFrame(interval_time);
#elif defined(__MACH__)
                    ImGui_ImplOSX_NewFrame(interval_time);
#else
#error Unknown Platform
#endif
#elif defined(_MSC_VER)
                    ImGui_ImplWin32_NewFrame(interval_time);
#else
#error Unknown Compiler
#endif
                    ImGui::NewFrame();

                    ui_simulate(wsi_state.m_anari_device, &wsi_state.m_ui_model, &wsi_state.m_ui_controller);

                    // ImGui::EndFrame();
                    ImGui::Render();
                }

                // Motion
                {
                    mcrt_set<brx_motion_video_detector *> video_detectors;
                    mcrt_set<brx_motion_animation_instance *> animation_instances;
                    for (auto const &instance_model : wsi_state.m_ui_model.m_instance_models)
                    {
                        brx_motion_skeleton_instance *const skeleton_instance = instance_model.second.m_skeleton_instance;

                        if (NULL != skeleton_instance)
                        {
                            brx_motion_video_detector *const video_detector = skeleton_instance->get_input_video_detector();
                            brx_motion_animation_instance *const animation_instance = skeleton_instance->get_input_animation_instance();

                            assert((NULL == video_detector) || (NULL == animation_instance));

                            if (NULL != video_detector)
                            {
                                video_detectors.insert(video_detector);
                            }
                            else if (NULL != animation_instance)
                            {
                                animation_instances.insert(animation_instance);
                            }
                        }
                        else
                        {
                            assert(INVALID_TIMESTAMP == instance_model.second.m_video_detector);
                            assert(INVALID_TIMESTAMP == instance_model.second.m_instance_motion);
                        }
                    }

                    mcrt_set<brx_motion_video_capture *> video_captures;
                    for (brx_motion_video_detector *const video_detector : video_detectors)
                    {
                        if (NULL != video_detector)
                        {
                            brx_motion_video_capture *const video_capture = video_detector->get_input();

                            if (NULL != video_capture)
                            {
                                video_captures.insert(video_capture);
                            }
                            else
                            {
                                assert(false);
                            }
                        }
                        else
                        {
                            assert(false);
                        }
                    }

                    for (brx_motion_video_capture *const video_capture : video_captures)
                    {
                        if (NULL != video_capture)
                        {

                            video_capture->step();
                        }
                        else
                        {
                            assert(false);
                        }
                    }

                    for (brx_motion_video_detector *const video_detector : video_detectors)
                    {
                        if (NULL != video_detector)
                        {

                            video_detector->step();
                        }
                        else
                        {
                            assert(false);
                        }
                    }

                    float const clipped_interval_time = std::min(interval_time, 1.0F / 30.0F);
                    for (brx_motion_animation_instance *const animation_instance : animation_instances)
                    {
                        if (NULL != animation_instance)
                        {
                            animation_instance->step(clipped_interval_time);
                        }
                        else
                        {
                            assert(false);
                        }
                    }

                    for (auto const &instance_model : wsi_state.m_ui_model.m_instance_models)
                    {
                        if (NULL != instance_model.second.m_skeleton_instance)
                        {
                            brx_anari_surface_group_instance *const surface_group_instance = instance_model.second.m_surface_group_instance;
                            brx_motion_skeleton_instance *const skeleton_instance = instance_model.second.m_skeleton_instance;

                            if ((NULL != surface_group_instance) && (NULL != skeleton_instance))
                            {
                                brx_motion_video_detector *const video_detector = skeleton_instance->get_input_video_detector();
                                brx_motion_animation_instance *const animation_instance = skeleton_instance->get_input_animation_instance();

                                assert((NULL == video_detector) || (NULL == animation_instance));

                                if (NULL != video_detector)
                                {
                                    for (uint32_t morph_target_name_index = 0U; morph_target_name_index < BRX_MOTION_MORPH_TARGET_NAME_MMD_COUNT; ++morph_target_name_index)
                                    {
                                        BRX_MOTION_MORPH_TARGET_NAME const morph_target_name = static_cast<BRX_MOTION_MORPH_TARGET_NAME>(morph_target_name_index);
                                        float const morph_target_weight = video_detector->get_morph_target_weight(skeleton_instance->get_input_video_detector_face_index(), morph_target_name);
                                        surface_group_instance->set_morph_target_weight(wrap(morph_target_name), morph_target_weight);
                                    }
                                }
                                else if (NULL != animation_instance)
                                {
                                    for (uint32_t morph_target_name_index = 0U; morph_target_name_index < BRX_MOTION_MORPH_TARGET_NAME_MMD_COUNT; ++morph_target_name_index)
                                    {
                                        BRX_MOTION_MORPH_TARGET_NAME const morph_target_name = static_cast<BRX_MOTION_MORPH_TARGET_NAME>(morph_target_name_index);
                                        float const morph_target_weight = animation_instance->get_morph_target_weight(morph_target_name);
                                        surface_group_instance->set_morph_target_weight(wrap(morph_target_name), morph_target_weight);
                                    }
                                }

                                {
                                    skeleton_instance->step(wsi_state.m_ui_model.m_physics_ragdoll_quality);
                                    surface_group_instance->set_skin_transforms(skeleton_instance->get_skin_transform_count(), wrap(skeleton_instance->get_skin_transforms()));
                                }
                            }
                            else
                            {
                                assert(false);
                            }
                        }
                        else
                        {
                            assert(INVALID_TIMESTAMP == instance_model.second.m_video_detector);
                            assert(INVALID_TIMESTAMP == instance_model.second.m_instance_motion);
                        }
                    }
                }

                // Camera
                {
                    wsi_state.m_anari_device->camera_set(wsi_state.m_ui_model.m_camera_position, wsi_state.m_ui_model.m_camera_direction, wsi_state.m_ui_model.m_camera_up, wsi_state.m_ui_model.m_camera_fovy, wsi_state.m_ui_model.m_camera_near, wsi_state.m_ui_model.m_camera_far);
                }

                // Renderer
                {
                    wsi_state.m_anari_device->renderer_set(brx_anari_vec3{wsi_state.m_ui_model.m_renderer_background_r, wsi_state.m_ui_model.m_renderer_background_g, wsi_state.m_ui_model.m_renderer_background_b}, wsi_state.m_ui_model.m_renderer_style, wsi_state.m_ui_model.m_renderer_toon_shading_first_shade_color_step, wsi_state.m_ui_model.m_renderer_toon_shading_first_shade_color_feather, wsi_state.m_ui_model.m_renderer_toon_shading_second_shade_color_step, wsi_state.m_ui_model.m_renderer_toon_shading_second_shade_color_feather, wsi_state.m_ui_model.m_renderer_toon_shading_base_color, wsi_state.m_ui_model.m_renderer_toon_shading_first_shade_color, wsi_state.m_ui_model.m_renderer_toon_shading_second_shade_color, wsi_state.m_ui_model.m_renderer_toon_shading_high_color_power, wsi_state.m_ui_model.m_renderer_toon_shading_rim_light_power, wsi_state.m_ui_model.m_renderer_toon_shading_rim_light_inside_mask);
                }

                // Directional Lighting
                {
                    wsi_state.m_anari_device->directional_light_set(wsi_state.m_ui_model.m_directional_lighting_visible, brx_anari_vec3{wsi_state.m_ui_model.m_directional_lighting_color_r * wsi_state.m_ui_model.m_directional_lighting_irradiance, wsi_state.m_ui_model.m_directional_lighting_color_g * wsi_state.m_ui_model.m_directional_lighting_irradiance, wsi_state.m_ui_model.m_directional_lighting_color_b * wsi_state.m_ui_model.m_directional_lighting_irradiance}, brx_anari_vec3{wsi_state.m_ui_model.m_directional_lighting_direction_x, wsi_state.m_ui_model.m_directional_lighting_direction_y, wsi_state.m_ui_model.m_directional_lighting_direction_z});
                }

                // Area Lighting
                {
                    wsi_state.m_anari_device->quad_light_set_enable_debug_renderer(wsi_state.m_ui_model.m_quad_light_enable_debug_renderer);

                    mcrt_vector<BRX_ANARI_QUAD_LIGHT> quad_lights;
                    quad_lights.reserve(wsi_state.m_ui_model.m_area_lightings.size());
                    for (auto const &area_lighting : wsi_state.m_ui_model.m_area_lightings)
                    {
                        quad_lights.push_back(BRX_ANARI_QUAD_LIGHT{{area_lighting.second.m_color_r * area_lighting.second.m_radiance, area_lighting.second.m_color_g * area_lighting.second.m_radiance, area_lighting.second.m_color_b * area_lighting.second.m_radiance}, {area_lighting.second.m_position_x, area_lighting.second.m_position_y, area_lighting.second.m_position_z}, {area_lighting.second.m_edge1_x, area_lighting.second.m_edge1_y, area_lighting.second.m_edge1_z}, {area_lighting.second.m_edge2_x, area_lighting.second.m_edge2_y, area_lighting.second.m_edge2_z}});
                    }
                    wsi_state.m_anari_device->quad_light_set(quad_lights.size(), quad_lights.data());
                }

                // Environment Lighting
                {
                    wsi_state.m_anari_device->hdri_light_set_enable_skybox_renderer(wsi_state.m_ui_model.m_hdri_light_enable_skybox_renderer);
                }

                // Render
                wsi_state.m_anari_device->renderer_render_frame(wsi_state.m_ui_controller.m_ui_view);
            }

            return true;
        },
        &wsi_state);

    {
        wsi_state.m_anari_device->frame_detach_window();

        ui_model_uninit(wsi_state.m_anari_device, &wsi_state.m_ui_model);

        brx_anari_release_device(wsi_state.m_anari_device);
    }

#if defined(__GNUC__)
#if defined(__linux__)
    ImGui_ImplGLUT_Shutdown();
#elif defined(__MACH__)
    ImGui_ImplOSX_Shutdown();
#else
#error Unknown Platform
#endif
#elif defined(_MSC_VER)
    ImGui_ImplWin32_Shutdown();
#else
#error Unknown Compiler
#endif

    ImGui::DestroyContext();

    brx_wsi_uninit_main_window();

    brx_wsi_uninit_connection();

    return 0;
}

static void *internal_imgui_malloc_wrapper(size_t size, void *)
{
    return mcrt_malloc(size, 16);
}

static void internal_imgui_free_wrapper(void *ptr, void *)
{
    return mcrt_free(ptr);
}

static void internal_key_press_handler(void *handler_context, int key, bool shift_key, bool caps_key, bool ctrl_key, bool alt_key)
{
    wsi_state_t &wsi_state = (*static_cast<wsi_state_t *>(handler_context));

    D3DUtil_CameraKeys mapped_camera_key;
    switch (key)
    {
    case ImGuiKey_W:
    {
        mapped_camera_key = CAM_MOVE_FORWARD;
    }
    break;
    case ImGuiKey_S:
    {
        mapped_camera_key = CAM_MOVE_BACKWARD;
    }
    break;
    case ImGuiKey_A:
    {
        mapped_camera_key = CAM_STRAFE_LEFT;
    }
    break;
    case ImGuiKey_D:
    {
        mapped_camera_key = CAM_STRAFE_RIGHT;
    }
    break;
    case ImGuiKey_Q:
    {
        mapped_camera_key = CAM_MOVE_UP;
    }
    break;
    case ImGuiKey_E:
    {
        mapped_camera_key = CAM_MOVE_DOWN;
    }
    break;
    case ImGuiKey_Escape:
    {
        if (!(wsi_state.m_ui_controller.m_ui_view = (!wsi_state.m_ui_controller.m_ui_view)))
        {
            wsi_state.m_ui_controller.m_show_renderer_manager = false;
            wsi_state.m_ui_controller.m_show_video_capture_manager = false;
            wsi_state.m_ui_controller.m_show_asset_motion_manager = false;
            wsi_state.m_ui_controller.m_show_asset_model_manager = false;
            wsi_state.m_ui_controller.m_show_asset_image_manager = false;
            wsi_state.m_ui_controller.m_show_video_detector_manager = false;
            wsi_state.m_ui_controller.m_show_instance_motion_manager = false;
            wsi_state.m_ui_controller.m_show_instance_model_manager = false;
            wsi_state.m_ui_controller.m_show_camera_manager = false;
            wsi_state.m_ui_controller.m_show_physics_ragdoll_manager = false;
            wsi_state.m_ui_controller.m_show_window_manager = false;
            wsi_state.m_ui_controller.m_show_directional_lighting_manager = false;
            wsi_state.m_ui_controller.m_show_area_lighting_manager = false;
            wsi_state.m_ui_controller.m_show_environment_lighting_manager = false;
            wsi_state.m_ui_controller.m_show_global_illumination_manager = false;
            wsi_state.m_ui_controller.m_show_acknowledgement = false;
        }

        mapped_camera_key = CAM_UNKNOWN;
    }
    break;
    default:
    {
        mapped_camera_key = CAM_UNKNOWN;
    }
    };

    wsi_state.m_ui_controller.m_first_person_camera.HandleKeyDownMessage(mapped_camera_key);
}

static void internal_key_release_handler(void *handler_context, int key, bool shift_key, bool caps_key, bool ctrl_key, bool alt_key)
{
    wsi_state_t &wsi_state = (*static_cast<wsi_state_t *>(handler_context));

    D3DUtil_CameraKeys mapped_camera_key;
    switch (key)
    {
    case ImGuiKey_W:
    {
        mapped_camera_key = CAM_MOVE_FORWARD;
    }
    break;
    case ImGuiKey_S:
    {
        mapped_camera_key = CAM_MOVE_BACKWARD;
    }
    break;
    case ImGuiKey_A:
    {
        mapped_camera_key = CAM_STRAFE_LEFT;
    }
    break;
    case ImGuiKey_D:
    {
        mapped_camera_key = CAM_STRAFE_RIGHT;
    }
    break;
    case ImGuiKey_Q:
    {
        mapped_camera_key = CAM_MOVE_UP;
    }
    break;
    case ImGuiKey_E:
    {
        mapped_camera_key = CAM_MOVE_DOWN;
    }
    break;
    default:
    {
        mapped_camera_key = CAM_UNKNOWN;
    }
    };

    wsi_state.m_ui_controller.m_first_person_camera.HandleKeyUpMessage(mapped_camera_key);
}

static void internal_button_press_handler(void *handler_context, int button, int window_x, int window_y)
{
}

static void internal_button_release_handler(void *handler_context, int button, int window_x, int window_y)
{
}

static void internal_scroll_up_handler(void *handler_context, int window_x, int window_y)
{
}

static void internal_scroll_down_handler(void *handler_context, int window_x, int window_y)
{
}

static void internal_motion_handler(void *handler_context, int window_x, int window_y, bool left_button, bool middle_button, bool right_button)
{
    wsi_state_t &wsi_state = (*static_cast<wsi_state_t *>(handler_context));

    float normalized_x = static_cast<float>(static_cast<double>(window_x) / static_cast<double>(wsi_state.m_window_width));
    float normalized_y = static_cast<float>(static_cast<double>(window_y) / static_cast<double>(wsi_state.m_window_height));

    wsi_state.m_ui_controller.m_first_person_camera.HandleMouseMoveMessage(normalized_x, normalized_y, left_button, middle_button, right_button);
}

static void internal_resize_handler(void *handler_context, int new_width, int new_height, float new_width_scale, float new_height_scale)
{
    wsi_state_t &wsi_state = (*static_cast<wsi_state_t *>(handler_context));

    if ((wsi_state.m_window_width != new_width) || (wsi_state.m_window_height != new_height) || (std::abs(wsi_state.m_window_width_scale - new_width_scale) > 1E-3F) || (std::abs(wsi_state.m_window_height_scale - new_height_scale) > 1E-3F))
    {
        if ((new_width > 0) && (new_height > 0) && (new_width_scale > 1E-3F) && (new_height_scale > 1E-3F))
        {
            wsi_state.m_anari_device->frame_resize_window(1.0F / wsi_state.m_window_width_scale, 1.0F / wsi_state.m_window_height_scale);
        }

        wsi_state.m_window_width = new_width;
        wsi_state.m_window_height = new_height;

        wsi_state.m_window_width_scale = new_width_scale;
        wsi_state.m_window_height_scale = new_height_scale;
    }
}

extern bool internal_platform_get_open_file_name(uint32_t filter_count, char const *const *filter_names, char const *const *filter_specs, int &inout_file_type_index, mcrt_string *out_file_name, uint64_t *out_file_timestamp, mcrt_vector<uint8_t> *out_file_data)
{
    if (brx_wsi_get_open_file_name(
            filter_count,
            filter_names,
            filter_specs,
            &inout_file_type_index,
            out_file_name,
            [](void *out_file_name_std_string, char const *s) -> void
            {
                assert(static_cast<mcrt_string *>(out_file_name_std_string)->empty());
                static_cast<mcrt_string *>(out_file_name_std_string)->assign(s);
            }))
    {
        return internal_platform_get_file_timestamp_and_data(out_file_name->c_str(), out_file_timestamp, out_file_data);
    }
    else
    {
        return false;
    }
}

#if defined(__GNUC__)
#if defined(__linux__)
extern bool internal_platform_get_file_timestamp_and_data(char const *file_name, uint64_t *out_file_timestamp, mcrt_vector<uint8_t> *out_file_data)
{
    bool file_timestamp_found;
    bool file_data_found;
    {
        int file = open(file_name, O_RDONLY);
        if (file >= 0)
        {
            struct stat buf;
            int res_fstat = fstat(file, &buf);

            if (0 == res_fstat)
            {
                (*out_file_timestamp) = static_cast<uint64_t>(1000000000ULL) * static_cast<uint64_t>(buf.st_mtim.tv_sec) + static_cast<uint64_t>(buf.st_mtim.tv_nsec);
                file_timestamp_found = true;

                if (NULL != out_file_data)
                {
                    if (buf.st_size < static_cast<int64_t>(INT32_MAX))
                    {
                        assert(out_file_data->empty());
                        out_file_data->resize(static_cast<int64_t>(buf.st_size));

                        size_t offset = 0U;
                        while (offset < buf.st_size)
                        {
                            ssize_t size_read = read(file, out_file_data->data() + offset, buf.st_size - offset);
                            if (size_read > 0)
                            {
                                offset += static_cast<uint32_t>(size_read);
                            }
                            else if (0 == size_read)
                            {
                                // Tolerance: unexpected EOF
                                assert(false);
                                break;
                            }
                            else if ((-1 == size_read) && (EINTR == errno))
                            {
                                // Do Nothing
                            }
                            else
                            {
                                out_file_data->clear();
                                assert(out_file_data->empty());
                                break;
                            }
                        }
                        assert(buf.st_size == offset);
                        file_data_found = true;
                    }
                    else
                    {
                        assert(false);
                        assert(out_file_data->empty());
                        file_data_found = false;
                    }
                }
                else
                {
                    file_data_found = true;
                }
            }
            else
            {
                assert(false);
                assert(out_file_data->empty());
                file_timestamp_found = false;
                file_data_found = false;
            }

            int res_close_handle = close(file);
            assert(0 == res_close_handle);
        }
        else
        {
            // assert(ERROR_FILE_NOT_FOUND == GetLastError());
            assert(out_file_data->empty());
            file_timestamp_found = false;
            file_data_found = false;
        }
    }

    if (file_timestamp_found && file_data_found)
    {
        assert(0U != (*out_file_timestamp));
        assert((NULL == out_file_data) || (!out_file_data->empty()));
        return true;
    }
    else
    {
        if (file_timestamp_found)
        {
            assert(!file_data_found);
            assert(0U != (*out_file_timestamp));
            (*out_file_timestamp) = 0U;
        }
        else
        {
            // assert(0U == (*out_file_timestamp));
        }

        if (file_data_found)
        {
            assert(!file_timestamp_found);
            assert((NULL == out_file_data) || (!out_file_data->empty()));
            if (NULL != out_file_data)
            {
                out_file_data->clear();
            }
        }
        else
        {
            assert((NULL == out_file_data) || out_file_data->empty());
        }

        return false;
    }
}
#elif defined(__MACH__)
extern bool internal_platform_get_file_timestamp_and_data(char const *file_name, uint64_t *out_file_timestamp, mcrt_vector<uint8_t> *out_file_data)
{
    bool file_timestamp_found;
    bool file_data_found;
    {
        int file = open(file_name, O_RDONLY);
        if (file >= 0)
        {
            struct stat buf;
            int res_fstat = fstat(file, &buf);

            if (0 == res_fstat)
            {
                (*out_file_timestamp) = static_cast<uint64_t>(1000000000ULL) * static_cast<uint64_t>(buf.st_mtimespec.tv_sec) + static_cast<uint64_t>(buf.st_mtimespec.tv_nsec);
                file_timestamp_found = true;

                if (NULL != out_file_data)
                {
                    if (buf.st_size < static_cast<int64_t>(INT32_MAX))
                    {
                        assert(out_file_data->empty());
                        out_file_data->resize(static_cast<int64_t>(buf.st_size));

                        size_t offset = 0U;
                        while (offset < buf.st_size)
                        {
                            ssize_t size_read = read(file, out_file_data->data() + offset, buf.st_size - offset);
                            if (size_read > 0)
                            {
                                offset += static_cast<uint32_t>(size_read);
                            }
                            else if (0 == size_read)
                            {
                                // Tolerance: unexpected EOF
                                assert(false);
                                break;
                            }
                            else if ((-1 == size_read) && (EINTR == errno))
                            {
                                // Do Nothing
                            }
                            else
                            {
                                out_file_data->clear();
                                assert(out_file_data->empty());
                                break;
                            }
                        }
                        assert(buf.st_size == offset);
                        file_data_found = true;
                    }
                    else
                    {
                        assert(false);
                        assert(out_file_data->empty());
                        file_data_found = false;
                    }
                }
                else
                {
                    file_data_found = true;
                }
            }
            else
            {
                assert(false);
                assert(out_file_data->empty());
                file_timestamp_found = false;
                file_data_found = false;
            }

            int res_close_handle = close(file);
            assert(0 == res_close_handle);
        }
        else
        {
            // assert(ERROR_FILE_NOT_FOUND == GetLastError());
            assert(out_file_data->empty());
            file_timestamp_found = false;
            file_data_found = false;
        }
    }

    if (file_timestamp_found && file_data_found)
    {
        assert(0U != (*out_file_timestamp));
        assert((NULL == out_file_data) || (!out_file_data->empty()));
        return true;
    }
    else
    {
        if (file_timestamp_found)
        {
            assert(!file_data_found);
            assert(0U != (*out_file_timestamp));
            (*out_file_timestamp) = 0U;
        }
        else
        {
            // assert(0U == (*out_file_timestamp));
        }

        if (file_data_found)
        {
            assert(!file_timestamp_found);
            assert((NULL == out_file_data) || (!out_file_data->empty()));
            if (NULL != out_file_data)
            {
                out_file_data->clear();
            }
        }
        else
        {
            assert((NULL == out_file_data) || out_file_data->empty());
        }

        return false;
    }
}
#else
#error Unknown Platform
#endif
#elif defined(_MSC_VER)
static inline bool internal_platform_get_file_timestamp_and_data(wchar_t const *file_name_utf16, uint64_t *out_file_timestamp, mcrt_vector<uint8_t> *out_file_data)
{
    bool file_timestamp_found;
    bool file_data_found;
    {
        HANDLE file = CreateFileW(file_name_utf16, FILE_GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (INVALID_HANDLE_VALUE != file)
        {
            FILETIME last_write_time;
            BOOL res_get_file_time = GetFileTime(file, NULL, NULL, &last_write_time);
            if (FALSE != res_get_file_time)
            {
                (*out_file_timestamp) = ((static_cast<uint64_t>(last_write_time.dwHighDateTime) << 32ULL) | static_cast<uint64_t>(last_write_time.dwLowDateTime));
                file_timestamp_found = true;
            }
            else
            {
                assert(false);
                file_timestamp_found = false;
            }

            LARGE_INTEGER length;
            BOOL res_get_file_size_ex = GetFileSizeEx(file, &length);
            if ((FALSE != res_get_file_size_ex))
            {
                if (NULL != out_file_data)
                {
                    if ((static_cast<int64_t>(length.QuadPart) > 0U) && (static_cast<int64_t>(length.QuadPart) < static_cast<int64_t>(INT32_MAX)))
                    {
                        assert(out_file_data->empty());
                        out_file_data->resize(static_cast<int64_t>(length.QuadPart));

                        DWORD read_size;
                        BOOL res_read_file = ReadFile(file, out_file_data->data(), static_cast<DWORD>(length.QuadPart), &read_size, NULL);
                        if (FALSE != res_read_file)
                        {
                            if (static_cast<DWORD>(length.QuadPart) == read_size)
                            {
                                file_data_found = true;
                            }
                            else
                            {
                                // Tolerance: unexpected EOF
                                assert(false);
                                file_data_found = true;
                            }
                        }
                        else
                        {
                            assert(false);
                            out_file_data->clear();
                            file_data_found = false;
                        }
                    }
                    else
                    {
                        assert(false);
                        assert(out_file_data->empty());
                        file_data_found = false;
                    }
                }
                else
                {
                    file_data_found = true;
                }
            }
            else
            {
                assert(false);
                assert(out_file_data->empty());
                file_data_found = false;
            }

            BOOL res_close_handle = CloseHandle(file);
            assert(FALSE != res_close_handle);
        }
        else
        {
#ifndef NDEBUG
            DWORD last_error = GetLastError();
#endif
            assert((ERROR_FILE_NOT_FOUND == last_error) || (ERROR_PATH_NOT_FOUND == last_error));
            assert(out_file_data->empty());
            file_timestamp_found = false;
            file_data_found = false;
        }
    }

    if (file_timestamp_found && file_data_found)
    {
        assert(0U != (*out_file_timestamp));
        assert((NULL == out_file_data) || (!out_file_data->empty()));
        return true;
    }
    else
    {
        if (file_timestamp_found)
        {
            assert(!file_data_found);
            assert(0U != (*out_file_timestamp));
            (*out_file_timestamp) = 0U;
        }
        else
        {
            // assert(0U == (*out_file_timestamp));
        }

        if (file_data_found)
        {
            assert(!file_timestamp_found);
            assert((NULL == out_file_data) || (!out_file_data->empty()));
            if (NULL != out_file_data)
            {
                out_file_data->clear();
            }
        }
        else
        {
            assert((NULL == out_file_data) || out_file_data->empty());
        }

        return false;
    }
}

extern bool internal_platform_get_file_timestamp_and_data(char const *file_name, uint64_t *out_file_timestamp, mcrt_vector<uint8_t> *out_file_data)
{
    mcrt_wstring file_name_utf16;
    {
        assert(file_name_utf16.empty());

        mcrt_string src_utf8 = file_name;
        mcrt_wstring &dst_utf16 = file_name_utf16;

        assert(dst_utf16.empty());

        if (!src_utf8.empty())
        {
            dst_utf16.resize(src_utf8.size() + 1U);

            size_t in_bytes_left = sizeof(src_utf8[0]) * src_utf8.size();
            size_t out_bytes_left = sizeof(dst_utf16[0]) * dst_utf16.size();
            char *in_buf = src_utf8.data();
            char *out_buf = reinterpret_cast<char *>(dst_utf16.data());

            iconv_t conversion_descriptor = iconv_open("UTF-16LE", "UTF-8");
            assert(((iconv_t)(-1)) != conversion_descriptor);

            size_t conversion_result = iconv(conversion_descriptor, &in_buf, &in_bytes_left, &out_buf, &out_bytes_left);
            assert(((size_t)(-1)) != conversion_result);

            int result = iconv_close(conversion_descriptor);
            assert(-1 != result);

            dst_utf16.resize(reinterpret_cast<decltype(&dst_utf16[0])>(out_buf) - dst_utf16.data());
        }
    }

    return internal_platform_get_file_timestamp_and_data(file_name_utf16.c_str(), out_file_timestamp, out_file_data);
}
#else
#error Unknown Compiler
#endif
