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

extern bool internal_platform_get_open_file_name(void *platform_context, uint32_t filter_count, char const *const *filter_names, char const *const *filter_specs, int &inout_file_type_index, mcrt_string *out_file_name, uint64_t *out_file_timestamp, mcrt_vector<uint8_t> *out_file_data);

extern bool internal_platform_get_file_timestamp_and_data(char const *file_name, uint64_t *out_file_timestamp, mcrt_vector<uint8_t> *out_file_data);

struct wsi_state_t
{
    brx_anari_device *m_anari_device;
    int32_t m_window_width;
    int32_t m_window_height;
    double m_tick_count_resolution;
    uint64_t m_tick_count_previous_frame;
    bool m_ui_view;
    ui_model_t m_ui_model;
    ui_controller_t m_ui_controller;
};

static wsi_state_t s_wsi_state;

static void *internal_imgui_malloc_wrapper(size_t size, void *user_data);
static void internal_imgui_free_wrapper(void *ptr, void *user_data);

static void internal_key_press_handler(int key, bool shift_key, bool caps_key, bool ctrl_key, bool alt_key);
static void internal_key_release_handler(int key, bool shift_key, bool caps_key, bool ctrl_key, bool alt_key);
static void internal_button_press_handler(int button, int x, int y);
static void internal_button_release_handler(int button, int x, int y);
static void internal_scroll_up_handler(int x, int y);
static void internal_scroll_down_handler(int x, int y);
static void internal_motion_handler(int x, int y, bool left_button, bool middle_button, bool right_button);
static void internal_resize_handler(int width, int height);

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
#include <errno.h>

#include "../thirdparty/Brioche-ImGui/imgui.h"
#include "../thirdparty/Brioche-ImGui/backends/imgui_impl_glut.h"

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

// DirectX 12 Agility SDK
extern "C" __declspec(dllexport) UINT const D3D12SDKVersion = 613U;
extern "C" __declspec(dllexport) char const *D3D12SDKPath = ".\\D3D12\\";

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

    brx_wsi_init_connection();

    // ImGui_ImplWin32_EnableDpiAwareness();

    // Initialize
    s_wsi_state = wsi_state_t{
        NULL,
        1280,
        720,
        1.0 / static_cast<double>(mcrt_tick_count_per_second()),
        mcrt_tick_count_now(),
        true};

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
        s_wsi_state.m_window_width, s_wsi_state.m_window_height);

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
    ImGui_ImplGLUT_ReshapeFunc(s_wsi_state.m_window_width, s_wsi_state.m_window_height);
#else
#error Unknown Platform
#endif
#elif defined(_MSC_VER)
    ImGui_ImplWin32_Init(brx_wsi_get_main_window());
#else
#error Unknown Compiler
#endif

    s_wsi_state.m_anari_device = brx_anari_new_device(brx_wsi_get_connection());

    ui_model_init(s_wsi_state.m_anari_device, &s_wsi_state.m_ui_model);

    ui_controller_init(s_wsi_state.m_anari_device, &s_wsi_state.m_ui_controller);

    s_wsi_state.m_anari_device->frame_attach_window(brx_wsi_get_main_window());

    brx_wsi_show_main_window();

    // Run
    while (brx_wsi_wait_window())
    {
        // Render
        if (s_wsi_state.m_window_width > 0 && s_wsi_state.m_window_height > 0)
        {
            float interval_time;
            {
                uint64_t const tick_count_current_frame = mcrt_tick_count_now();
                interval_time = static_cast<float>(static_cast<double>(tick_count_current_frame - s_wsi_state.m_tick_count_previous_frame) * s_wsi_state.m_tick_count_resolution);
                s_wsi_state.m_tick_count_previous_frame = tick_count_current_frame;
            }

            // User Camera
            user_camera_simulate(interval_time, s_wsi_state.m_anari_device, &s_wsi_state.m_ui_model, &s_wsi_state.m_ui_controller);

            // UI
            {
#if defined(__GNUC__)
#if defined(__linux__)
                ImGui_ImplGLUT_NewFrame(interval_time);
#else
#error Unknown Platform
#endif
#elif defined(_MSC_VER)
                ImGui_ImplWin32_NewFrame();
#else
#error Unknown Compiler
#endif
                ImGui::NewFrame();

                ui_simulate(brx_wsi_get_main_window(), s_wsi_state.m_anari_device, &s_wsi_state.m_ui_model, &s_wsi_state.m_ui_controller);

                // ImGui::EndFrame();
                ImGui::Render();
            }

            // Motion
            {
                mcrt_set<brx_motion_video_detector *> video_detectors;
                mcrt_set<brx_motion_animation_instance *> animation_instances;
                for (auto const &instance_model : s_wsi_state.m_ui_model.m_instance_models)
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

                for (auto const &instance_model : s_wsi_state.m_ui_model.m_instance_models)
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
                                skeleton_instance->step(s_wsi_state.m_ui_controller.m_physics_ragdoll_quality);
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

            s_wsi_state.m_anari_device->renderer_render_frame(s_wsi_state.m_ui_view);
        }
    }

    // Destroy
    {
        s_wsi_state.m_anari_device->frame_detach_window();

        ui_model_uninit(s_wsi_state.m_anari_device, &s_wsi_state.m_ui_model);

        brx_anari_release_device(s_wsi_state.m_anari_device);
    }

#if defined(__GNUC__)
#if defined(__linux__)
    ImGui_ImplGLUT_Shutdown();
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

static void internal_key_press_handler(int key, bool shift_key, bool caps_key, bool ctrl_key, bool alt_key)
{

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
        if (!(s_wsi_state.m_ui_view = (!s_wsi_state.m_ui_view)))
        {
            s_wsi_state.m_ui_controller.m_show_video_capture_manager = false;
            s_wsi_state.m_ui_controller.m_show_asset_motion_manager = false;
            s_wsi_state.m_ui_controller.m_show_asset_model_manager = false;
            s_wsi_state.m_ui_controller.m_show_asset_image_manager = false;
            s_wsi_state.m_ui_controller.m_show_video_detector_manager = false;
            s_wsi_state.m_ui_controller.m_show_instance_motion_manager = false;
            s_wsi_state.m_ui_controller.m_show_instance_model_manager = false;
            s_wsi_state.m_ui_controller.m_show_camera_manager = false;
            s_wsi_state.m_ui_controller.m_show_physics_ragdoll_manager = false;
            s_wsi_state.m_ui_controller.m_show_environment_lighting_manager = false;
            s_wsi_state.m_ui_controller.m_show_global_illumination_manager = false;
        }

        mapped_camera_key = CAM_UNKNOWN;
    }
    break;
    default:
    {
        mapped_camera_key = CAM_UNKNOWN;
    }
    };

    s_wsi_state.m_ui_controller.m_first_person_camera.HandleKeyDownMessage(mapped_camera_key);
}

static void internal_key_release_handler(int key, bool shift_key, bool caps_key, bool ctrl_key, bool alt_key)
{

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

    s_wsi_state.m_ui_controller.m_first_person_camera.HandleKeyUpMessage(mapped_camera_key);
}

static void internal_button_press_handler(int button, int window_x, int window_y)
{
}

static void internal_button_release_handler(int button, int window_x, int window_y)
{
}

static void internal_scroll_up_handler(int window_x, int window_y)
{
}

static void internal_scroll_down_handler(int window_x, int window_y)
{
}

static void internal_motion_handler(int window_x, int window_y, bool left_button, bool middle_button, bool right_button)
{
    float normalized_x = static_cast<float>(static_cast<double>(window_x) / static_cast<double>(s_wsi_state.m_window_width));
    float normalized_y = static_cast<float>(static_cast<double>(window_y) / static_cast<double>(s_wsi_state.m_window_height));

    s_wsi_state.m_ui_controller.m_first_person_camera.HandleMouseMoveMessage(normalized_x, normalized_y, left_button, middle_button, right_button);
}

static void internal_resize_handler(int new_width, int new_height)
{
    if (s_wsi_state.m_window_width != new_width || s_wsi_state.m_window_height != new_height)
    {
        if (new_width > 0 && new_height > 0)
        {
            s_wsi_state.m_anari_device->frame_resize_window();
        }

        s_wsi_state.m_window_width = new_width;
        s_wsi_state.m_window_height = new_height;
    }
}

#if defined(__GNUC__)
#if defined(__linux__)

#include <fcntl.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#include <cinttypes>
#include <cstring>

extern bool internal_platform_get_file_timestamp_and_data(char const *file_name, uint64_t *out_file_timestamp, mcrt_vector<uint8_t> *out_file_data);

static mcrt_string s_file_name = secure_getenv("HOME");

extern bool internal_platform_get_open_file_name(void *platform_context, uint32_t filter_count, char const *const *filter_names, char const *const *filter_specs, int &inout_file_type_index, mcrt_string *out_file_name, uint64_t *out_file_timestamp, mcrt_vector<uint8_t> *out_file_data)
{
    mcrt_string file_name;
    {
        mcrt_vector<char *> argv(static_cast<size_t>(3U + filter_count + 1U));

        mcrt_string argv_0;
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

            argv_0 = dir_name;
            argv_0 += "BRX-OpenFileDialog";
        }
        argv[0] = argv_0.data();

        argv[1] = s_file_name.data();

        mcrt_string argv_2;
        {
            char in_name_filer_index_text[] = {"-2147483647"};
            std::snprintf(in_name_filer_index_text, sizeof(in_name_filer_index_text) / sizeof(in_name_filer_index_text[0]), "%" PRId32, static_cast<int32_t>(inout_file_type_index));

            argv_2 = in_name_filer_index_text;
        }
        argv[2] = argv_2.data();

        mcrt_vector<mcrt_string> file_types(static_cast<size_t>(filter_count));
        for (uint32_t filter_index = 0U; filter_index < filter_count; ++filter_index)
        {
            mcrt_string modified_filter_spec = filter_specs[filter_index];
            std::replace(modified_filter_spec.begin(), modified_filter_spec.end(), ';', ' ');

            file_types[filter_index] = filter_names[filter_index];
            file_types[filter_index] += " (";
            file_types[filter_index] += modified_filter_spec;
            file_types[filter_index] += ")";

            argv[3U + filter_index] = file_types[filter_index].data();
        }

        argv[3U + filter_count] = NULL;

        int socket_vector[2];
        {
            int res_socketpair = socketpair(AF_UNIX, SOCK_STREAM, 0, socket_vector);
            assert(0 == res_socketpair);
        }

        {
            int flags = fcntl(socket_vector[0], F_GETFD);
            assert(-1 != flags);

            int res_fcntl_setfd = fcntl(socket_vector[0], F_SETFD, (flags | FD_CLOEXEC));
            assert(0 == res_fcntl_setfd);
        }

        {
            int flags = fcntl(socket_vector[1], F_GETFD);
            assert(-1 != flags);

            int res_fcntl_setfd = fcntl(socket_vector[1], F_SETFD, (flags | FD_CLOEXEC));
            assert(0 == res_fcntl_setfd);
        }

        pid_t child_pid = fork();
        if (0 == child_pid)
        {
            {
                int res_close = close(socket_vector[0]);
                assert(0 == res_close);
            }

            {
                int res_close = close(STDOUT_FILENO);
                assert(0 == res_close);
            }

            {
                int res_dup2 = dup2(socket_vector[1], STDOUT_FILENO);
                assert(STDOUT_FILENO == res_dup2);
            }

            {
                int res_close = close(socket_vector[1]);
                assert(0 == res_close);
            }

            {
                int res_execvp = execvp(argv[0], argv.data());
                assert(-1 == res_execvp);
            }

            // fail execvp
            // no return
            _exit(-1);
        }

        if (child_pid > 0)
        {
            mcrt_vector<char> child_stdout;
            {
                {
                    int res_close = close(socket_vector[1]);
                    assert(0 == res_close);
                }

                assert(child_stdout.empty());
                while (true)
                {
                    char buffer_read[4096];
                    ssize_t size_read = read(socket_vector[0], buffer_read, sizeof(buffer_read));
                    if (size_read > 0)
                    {
                        size_t offset = child_stdout.size();
                        child_stdout.resize(offset + size_read);
                        std::memcpy(child_stdout.data() + offset, buffer_read, size_read);
                    }
                    else if (0 == size_read)
                    {
                        // EOF
                        break;
                    }
                    else if ((-1 == size_read) && (EINTR == errno))
                    {
                        // Do Nothing
                    }
                    else
                    {
                        child_stdout.clear();
                        assert(child_stdout.empty());
                        break;
                    }
                }

                {
                    int res_close = close(socket_vector[0]);
                    assert(0 == res_close);
                }

                while (true)
                {
                    int stat_loc;
                    pid_t res_waitpid = waitpid(child_pid, &stat_loc, 0);
                    if (res_waitpid > 0)
                    {
                        if ((WIFEXITED(stat_loc)) && (0 == (WEXITSTATUS(stat_loc))))
                        {
                            // Do Nothing
                        }
                        else
                        {
                            child_stdout.clear();
                            assert(child_stdout.empty());
                        }

                        break;
                    }
                    else if ((-1 == res_waitpid) && (EINTR == errno))
                    {
                        // Do Nothing
                    }
                    else
                    {
                        child_stdout.clear();
                        assert(child_stdout.empty());
                        break;
                    }
                }
            }

            if (!child_stdout.empty())
            {
                mcrt_string result_name_filer_index_text;
                mcrt_string result_file_name;
                {
                    char *result_name_filer_index_text_end = static_cast<char *>(std::memchr(child_stdout.data(), '\0', child_stdout.size()));
                    if (NULL != result_name_filer_index_text_end)
                    {
                        result_name_filer_index_text.assign(child_stdout.data(), result_name_filer_index_text_end - child_stdout.data());

                        char *result_file_name_begin = result_name_filer_index_text_end + 1;
                        if (result_file_name_begin < (child_stdout.data() + child_stdout.size()))
                        {
                            char *result_file_name_end = static_cast<char *>(std::memchr(result_file_name_begin, '\0', (child_stdout.data() + child_stdout.size()) - result_file_name_begin));

                            if (NULL != result_file_name_end)
                            {
                                assert((child_stdout.data() + child_stdout.size()) == (result_file_name_end + 1));
                                result_file_name.assign(result_file_name_begin, result_file_name_end - result_file_name_begin);
                            }
                        }
                    }
                }

                inout_file_type_index = std::strtoll(result_name_filer_index_text.c_str(), NULL, 10);

                assert(file_name.empty());
                file_name = std::move(result_file_name);
            }
        }
        else
        {
            {
                int res_close = close(socket_vector[0]);
                assert(0 == res_close);
            }

            {
                int res_close = close(socket_vector[1]);
                assert(0 == res_close);
            }

            assert(file_name.empty());
        }
    }

    if (!file_name.empty())
    {
        s_file_name = file_name;

        bool file_timestamp_and_data_found = internal_platform_get_file_timestamp_and_data(file_name.c_str(), out_file_timestamp, out_file_data);

        if (file_timestamp_and_data_found)
        {
            assert(0U != (*out_file_timestamp));
            assert((NULL == out_file_data) || (!out_file_data->empty()));

            assert(out_file_name->empty());
            (*out_file_name) = std::move(file_name);

            return true;
        }
        else
        {
            assert(out_file_name->empty());
            assert((NULL == out_file_data) || (out_file_data->empty()));
            return false;
        }
    }
    else
    {
        assert(out_file_name->empty());
        assert((NULL == out_file_data) || (out_file_data->empty()));
        return false;
    }
}

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
#else
#error Unknown Platform
#endif
#elif defined(_MSC_VER)

#include <windowsx.h>
#include <commdlg.h>
#include <shobjidl_core.h>
#include "../thirdparty/libiconv/include/iconv.h"

static HRESULT STDMETHODCALLTYPE NoRegCoCreate(const __wchar_t *dllName, REFCLSID rclsid, REFIID riid, void **ppv)
{
    HMODULE dynamic_library = GetModuleHandleW(dllName);
    if (NULL == dynamic_library)
    {
        assert(ERROR_MOD_NOT_FOUND == GetLastError());

        dynamic_library = LoadLibraryW(dllName);
        assert(NULL != dynamic_library);
    }

    decltype(DllGetClassObject) *const pfn_dll_get_class_object = reinterpret_cast<decltype(DllGetClassObject) *>(GetProcAddress(dynamic_library, "DllGetClassObject"));

    if (NULL == pfn_dll_get_class_object)
    {
        return HRESULT_FROM_WIN32(GetLastError());
    }

    IClassFactory *class_factory = NULL;
    HRESULT hr_dll_get_class_object = pfn_dll_get_class_object(rclsid, IID_PPV_ARGS(&class_factory));
    if (!SUCCEEDED(hr_dll_get_class_object))
    {
        return hr_dll_get_class_object;
    }

    HRESULT hr_class_factory_create_instance = class_factory->CreateInstance(nullptr, riid, ppv);
    class_factory->Release();

    // TODO: DllCanUnloadNow

    return hr_class_factory_create_instance;
}

static inline bool internal_platform_get_file_timestamp_and_data(wchar_t const *file_name_utf16, uint64_t *out_file_timestamp, mcrt_vector<uint8_t> *out_file_data);

extern bool internal_platform_get_open_file_name(void *platform_context, uint32_t filter_count, char const *const *filter_names, char const *const *filter_specs, int &inout_file_type_index, mcrt_string *out_file_name, uint64_t *out_file_timestamp, mcrt_vector<uint8_t> *out_file_data)
{
    // we may reference https://github.com/mlabbe/nativefiledialog

    mcrt_wstring file_name_utf16;
    {
        HRESULT res_co_initialize_ex = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
        assert(SUCCEEDED(res_co_initialize_ex));

        IFileOpenDialog *file_open_dialog = NULL;
        // HRESULT res_co_create_instance = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&file_open_dialog));
        HRESULT res_co_create_instance = NoRegCoCreate(L"Comdlg32.dll", CLSID_FileOpenDialog, IID_PPV_ARGS(&file_open_dialog));
        assert(SUCCEEDED(res_co_create_instance));

        FILEOPENDIALOGOPTIONS file_open_dialog_options;
        HRESULT res_file_open_dialog_get_options = file_open_dialog->GetOptions(&file_open_dialog_options);
        assert(SUCCEEDED(res_file_open_dialog_get_options));

        file_open_dialog_options |= FOS_FORCEFILESYSTEM;
        file_open_dialog_options &= (~static_cast<FILEOPENDIALOGOPTIONS>(FOS_ALLOWMULTISELECT));

        HRESULT res_file_open_dialog_set_options = file_open_dialog->SetOptions(file_open_dialog_options);
        assert(SUCCEEDED(res_file_open_dialog_set_options));

        mcrt_vector<mcrt_wstring> filter_names_utf16(static_cast<size_t>(filter_count));
        mcrt_vector<mcrt_wstring> filter_specs_utf16(static_cast<size_t>(filter_count));
        mcrt_vector<COMDLG_FILTERSPEC> filters(static_cast<size_t>(filter_count));
        {
            iconv_t conversion_descriptor = iconv_open("UTF-16LE", "UTF-8");
            assert(((iconv_t)(-1)) != conversion_descriptor);

            for (uint32_t filter_index = 0U; filter_index < filter_count; ++filter_index)
            {
                {
                    mcrt_string src_utf8 = filter_names[filter_index];
                    mcrt_wstring &dst_utf16 = filter_names_utf16[filter_index];

                    assert(dst_utf16.empty());
                    assert(!src_utf8.empty());

                    dst_utf16.resize(src_utf8.size() + 1U);

                    size_t in_bytes_left = sizeof(src_utf8[0]) * src_utf8.size();
                    size_t out_bytes_left = sizeof(dst_utf16[0]) * dst_utf16.size();
                    char *in_buf = src_utf8.data();
                    char *out_buf = reinterpret_cast<char *>(dst_utf16.data());

                    size_t conversion_result = iconv(conversion_descriptor, &in_buf, &in_bytes_left, &out_buf, &out_bytes_left);
                    assert(((size_t)(-1)) != conversion_result);

                    dst_utf16.resize(reinterpret_cast<decltype(&dst_utf16[0])>(out_buf) - dst_utf16.data());
                }

                filters[filter_index].pszName = filter_names_utf16[filter_index].data();

                {
                    mcrt_string src_utf8 = filter_specs[filter_index];
                    mcrt_wstring &dst_utf16 = filter_specs_utf16[filter_index];

                    assert(dst_utf16.empty());
                    assert(!src_utf8.empty());

                    dst_utf16.resize(src_utf8.size() + 1U);

                    size_t in_bytes_left = sizeof(src_utf8[0]) * src_utf8.size();
                    size_t out_bytes_left = sizeof(dst_utf16[0]) * dst_utf16.size();
                    char *in_buf = src_utf8.data();
                    char *out_buf = reinterpret_cast<char *>(dst_utf16.data());

                    size_t conversion_result = iconv(conversion_descriptor, &in_buf, &in_bytes_left, &out_buf, &out_bytes_left);
                    assert(((size_t)(-1)) != conversion_result);

                    dst_utf16.resize(reinterpret_cast<decltype(&dst_utf16[0])>(out_buf) - dst_utf16.data());
                }

                filters[filter_index].pszSpec = filter_specs_utf16[filter_index].data();
            }

            int result = iconv_close(conversion_descriptor);
            assert(-1 != result);
        }

        HRESULT res_file_open_dialog_set_file_types = file_open_dialog->SetFileTypes(filter_count, filters.data());
        assert(SUCCEEDED(res_file_open_dialog_set_file_types));

        HRESULT res_file_open_dialog_set_file_type_index = file_open_dialog->SetFileTypeIndex(std::min(std::max(static_cast<int>(1), (static_cast<int>(inout_file_type_index) + static_cast<int>(1))), (static_cast<int>(filter_count) + static_cast<int>(1))));
        assert(SUCCEEDED(res_file_open_dialog_set_file_type_index));

        HRESULT res_file_open_dialog_show = file_open_dialog->Show(static_cast<HWND>(platform_context));
        if (SUCCEEDED(res_file_open_dialog_show))
        {
            UINT file_type;
            HRESULT res_file_open_dialog_get_file_type_index = file_open_dialog->GetFileTypeIndex(&file_type);
            assert(SUCCEEDED(res_file_open_dialog_get_file_type_index));
            inout_file_type_index = (static_cast<int>(file_type) - 1);

            IShellItem *item;
            HRESULT res_file_open_dialog_get_result = file_open_dialog->GetResult(&item);
            assert(SUCCEEDED(res_file_open_dialog_get_result));

            WCHAR *name;
            HRESULT res_shell_item_get_display_name = item->GetDisplayName(SIGDN_FILESYSPATH, &name);
            assert(SUCCEEDED(res_shell_item_get_display_name));

            assert(file_name_utf16.empty());
            file_name_utf16 = name;

            CoTaskMemFree(name);

            item->Release();
        }
        else
        {
            assert(HRESULT_FROM_WIN32(ERROR_CANCELLED) == res_file_open_dialog_show);

            assert(file_name_utf16.empty());
        }

        file_open_dialog->Release();

        CoFreeUnusedLibraries();

        CoUninitialize();
    }

    if (!file_name_utf16.empty())
    {
        bool file_timestamp_and_data_found = internal_platform_get_file_timestamp_and_data(file_name_utf16.c_str(), out_file_timestamp, out_file_data);

        if (file_timestamp_and_data_found)
        {
            assert(0U != (*out_file_timestamp));
            assert((NULL == out_file_data) || (!out_file_data->empty()));

            mcrt_wstring &src_utf16 = file_name_utf16;
            mcrt_string &dst_utf8 = (*out_file_name);

            assert(dst_utf8.empty());
            assert(!src_utf16.empty());

            constexpr size_t const UNI_MAX_UTF8_BYTES_PER_CODE_POINT = 4U;
            dst_utf8.resize(src_utf16.size() * UNI_MAX_UTF8_BYTES_PER_CODE_POINT + 1U);

            size_t in_bytes_left = sizeof(src_utf16[0]) * src_utf16.size();
            size_t out_bytes_left = sizeof(dst_utf8[0]) * dst_utf8.size();
            char *in_buf = reinterpret_cast<char *>(src_utf16.data());
            char *out_buf = dst_utf8.data();

            iconv_t conversion_descriptor = iconv_open("UTF-8", "UTF-16LE");
            assert(((iconv_t)(-1)) != conversion_descriptor);

            size_t conversion_result = iconv(conversion_descriptor, &in_buf, &in_bytes_left, &out_buf, &out_bytes_left);
            assert(((size_t)(-1)) != conversion_result);

            int result = iconv_close(conversion_descriptor);
            assert(-1 != result);

            dst_utf8.resize(reinterpret_cast<decltype(&dst_utf8[0])>(out_buf) - &dst_utf8[0]);

            return true;
        }
        else
        {
            assert(out_file_name->empty());
            assert((NULL == out_file_data) || (out_file_data->empty()));
            return false;
        }
    }
    else
    {
        assert(out_file_name->empty());
        assert((NULL == out_file_data) || (out_file_data->empty()));
        return false;
    }
}

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
            assert(ERROR_FILE_NOT_FOUND == GetLastError());
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
