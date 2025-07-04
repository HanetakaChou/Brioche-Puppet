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
#include "user_input_model.h"
#include "user_input_controller.h"
#include "../thirdparty/McRT-Malloc/include/mcrt_vector.h"
#include "../thirdparty/McRT-Malloc/include/mcrt_string.h"
#include "../thirdparty/McRT-Malloc/include/mcrt_set.h"
#include "../thirdparty/Brioche-Analytic-Rendering-Interface/include/brx_anari.h"

extern bool _internal_platform_get_open_file_name(void *platform_context, size_t filter_count, char const *const *filter_names, char const *const *filter_specs, int &inout_file_type_index, mcrt_string *out_file_name, uint64_t *out_file_timestamp, mcrt_vector<uint8_t> *out_file_data);

extern bool _internal_platform_get_file_timestamp_and_data(char const *file_name, uint64_t *out_file_timestamp, mcrt_vector<uint8_t> *out_file_data);

struct wsi_state_t
{
    bool m_running;
    brx_anari_device *m_anari_device;
    int32_t m_window_width;
    int32_t m_window_height;
    double m_tick_count_resolution;
    uint64_t m_tick_count_previous_frame;
    bool m_ui_view;
    ui_model_t m_ui_model;
    ui_controller_t m_ui_controller;
};

static inline uint64_t _internal_tick_count_per_second();

extern uint64_t _internal_tick_count_now();

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

#if defined(__ANDROID__)
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <sys/socket.h>
#include <android/native_activity.h>
#include "../thirdparty/Brioche-ImGui/imgui.h"
#include "../thirdparty/Brioche-ImGui/backends/imgui_impl_android.h"

static void ANativeActivity_Destroy(ANativeActivity *native_activity);
static void ANativeActivity_WindowFocusChanged(ANativeActivity *native_activity, int hasFocus);
static void ANativeActivity_NativeWindowCreated(ANativeActivity *native_activity, ANativeWindow *native_window);
static void ANativeActivity_NativeWindowResized(ANativeActivity *native_activity, ANativeWindow *native_window);
static void ANativeActivity_NativeWindowRedrawNeeded(ANativeActivity *native_activity, ANativeWindow *native_window);
static void ANativeActivity_NativeWindowDestroyed(ANativeActivity *native_activity, ANativeWindow *native_window);
static void ANativeActivity_InputQueueCreated(ANativeActivity *native_activity, AInputQueue *input_queue);
static void ANativeActivity_InputQueueDestroyed(ANativeActivity *native_activity, AInputQueue *input_queue);

static bool g_this_process_has_inited = false;

static int g_main_thread_looper_draw_callback_fd_read = -1;
static int g_main_thread_looper_draw_callback_fd_write = -1;
static int main_thread_looper_draw_callback(int fd, int, void *);

static pthread_t g_draw_request_thread;
bool volatile g_draw_request_thread_running = false;
static void *draw_request_thread_main(void *);

static int main_thread_looper_input_queue_callback(int fd, int, void *input_queue_void);

wsi_state_t g_wsi_state;

extern "C" JNIEXPORT void ANativeActivity_onCreate(ANativeActivity *native_activity, void *saved_state, size_t saved_state_size)
{
    native_activity->callbacks->onStart = NULL;
    native_activity->callbacks->onResume = NULL;
    native_activity->callbacks->onSaveInstanceState = NULL;
    native_activity->callbacks->onPause = NULL;
    native_activity->callbacks->onStop = NULL;
    native_activity->callbacks->onDestroy = ANativeActivity_Destroy;
    native_activity->callbacks->onWindowFocusChanged = ANativeActivity_WindowFocusChanged;
    native_activity->callbacks->onNativeWindowCreated = ANativeActivity_NativeWindowCreated;
    native_activity->callbacks->onNativeWindowResized = ANativeActivity_NativeWindowResized;
    native_activity->callbacks->onNativeWindowRedrawNeeded = ANativeActivity_NativeWindowRedrawNeeded;
    native_activity->callbacks->onNativeWindowDestroyed = ANativeActivity_NativeWindowDestroyed;
    native_activity->callbacks->onInputQueueCreated = ANativeActivity_InputQueueCreated;
    native_activity->callbacks->onInputQueueDestroyed = ANativeActivity_InputQueueDestroyed;
    native_activity->callbacks->onContentRectChanged = NULL;
    native_activity->callbacks->onConfigurationChanged = NULL;
    native_activity->callbacks->onLowMemory = NULL;

    if (!g_this_process_has_inited)
    {
        g_wsi_state.m_tick_count_resolution = 1.0 / static_cast<double>(tick_count_per_second());
        g_wsi_state.m_tick_count_previous_frame = tick_count_now();

        ui_controller_init(&g_wsi_state.m_ui_controller);

        {
            // Setup Dear ImGui context
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO &io = ImGui::GetIO();

            // Redirect loading/saving of .ini file to our location.
            // Make sure 'g_IniFilename' persists while we use Dear ImGui.
            static mcrt_string g_IniFilename = mcrt_string(native_activity->internalDataPath) + "/imgui.ini";
            io.IniFilename = g_IniFilename.c_str();

            // Setup Dear ImGui style
            ImGui::StyleColorsDark();
            // ImGui::StyleColorsLight();

            // Load Fonts
            // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
            // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
            // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
            // - Read 'docs/FONTS.md' for more instructions and details.
            // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
            // - Android: The TTF files have to be placed into the assets/ directory (android/app/src/main/assets), we use our GetAssetData() helper to retrieve them.

            // We load the default font with increased size to improve readability on many devices with "high" DPI.
            // FIXME: Put some effort into DPI awareness.
            // Important: when calling AddFontFromMemoryTTF(), ownership of font_data is transferred by Dear ImGui by default (deleted is handled by Dear ImGui), unless we set FontDataOwnedByAtlas=false in ImFontConfig
            ImFontConfig font_cfg;
            font_cfg.SizePixels = 22.0f;
            io.Fonts->AddFontDefault(&font_cfg);
            // void* font_data;
            // int font_data_size;
            // ImFont* font;
            // font_data_size = GetAssetData("segoeui.ttf", &font_data);
            // font = io.Fonts->AddFontFromMemoryTTF(font_data, font_data_size, 16.0f);
            // IM_ASSERT(font != nullptr);
            // font_data_size = GetAssetData("DroidSans.ttf", &font_data);
            // font = io.Fonts->AddFontFromMemoryTTF(font_data, font_data_size, 16.0f);
            // IM_ASSERT(font != nullptr);
            // font_data_size = GetAssetData("Roboto-Medium.ttf", &font_data);
            // font = io.Fonts->AddFontFromMemoryTTF(font_data, font_data_size, 16.0f);
            // IM_ASSERT(font != nullptr);
            // font_data_size = GetAssetData("Cousine-Regular.ttf", &font_data);
            // font = io.Fonts->AddFontFromMemoryTTF(font_data, font_data_size, 15.0f);
            // IM_ASSERT(font != nullptr);
            // font_data_size = GetAssetData("ArialUni.ttf", &font_data);
            // font = io.Fonts->AddFontFromMemoryTTF(font_data, font_data_size, 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
            // IM_ASSERT(font != nullptr);

            // Arbitrary scale-up
            // FIXME: Put some effort into DPI awareness
            ImGui::GetStyle().ScaleAllSizes(3.0f);
        }

        g_wsi_state.m_renderer = renderer_create(NULL);

        user_camera_controller_init(&g_wsi_state.m_user_camera_controller);

        // Simulate the following callback on Android:
        // WM_PAINT
        // CVDisplayLinkSetOutputCallback
        // displayLinkWithTarget
        {
            // the looper of the main thread
            ALooper *main_thread_looper = ALooper_forThread();
            assert(main_thread_looper != NULL);

            // Evidently, the real "draw" is slower than the "request"
            // There are no message boundaries for "SOCK_STREAM", and We can read all the data once
            int sv[2];
            int res_socketpair = socketpair(AF_UNIX, SOCK_STREAM, 0, sv);
            assert(0 == res_socketpair);
            g_main_thread_looper_draw_callback_fd_read = sv[0];
            g_main_thread_looper_draw_callback_fd_write = sv[1];

            // the identifier is ignored when callback is not NULL (the ALooper_pollOnce always returns the ALOOPER_POLL_CALLBACK)
            int res_looper_add_fd = ALooper_addFd(main_thread_looper, g_main_thread_looper_draw_callback_fd_read, -1, ALOOPER_EVENT_INPUT, main_thread_looper_draw_callback, NULL);
            assert(1 == res_looper_add_fd);

            // the "draw request" thread
            // TODO: use Load/Store
            g_draw_request_thread_running = false;
            int res_ptread_create = pthread_create(&g_draw_request_thread, NULL, draw_request_thread_main, NULL);
            assert(0 == res_ptread_create);
            while (!g_draw_request_thread_running)
            {
                // pthread_yield();
                sched_yield();
            }
        }

        // Don't worry
        // Single thread
        g_this_process_has_inited = true;
    }
}

// TODO:
// destroy before this process terminates

static void ANativeActivity_Destroy(ANativeActivity *native_activity)
{
}

static void ANativeActivity_WindowFocusChanged(ANativeActivity *native_activity, int hasFocus)
{
}

static void ANativeActivity_NativeWindowCreated(ANativeActivity *native_activity, ANativeWindow *native_window)
{
    ImGui_ImplAndroid_Init(native_window);

    g_wsi_state.m_window_width = ANativeWindow_getWidth(native_window);
    g_wsi_state.m_window_height = ANativeWindow_getHeight(native_window);

    g_wsi_state.m_renderer->attach_window(native_window);
}

static void ANativeActivity_NativeWindowResized(ANativeActivity *native_activity, ANativeWindow *native_window)
{
    g_wsi_state.m_renderer->on_window_resize();
}

static void ANativeActivity_NativeWindowRedrawNeeded(ANativeActivity *native_activity, ANativeWindow *native_window)
{
}

static void ANativeActivity_NativeWindowDestroyed(ANativeActivity *native_activity, ANativeWindow *native_window)
{
    g_wsi_state.m_renderer->detach_window();

    g_wsi_state.m_window_width = -1;
    g_wsi_state.m_window_height = -1;

    ImGui_ImplAndroid_Shutdown();
}

static void ANativeActivity_InputQueueCreated(ANativeActivity *native_activity, AInputQueue *input_queue)
{
    // the looper of the main thread
    ALooper *main_thread_looper = ALooper_forThread();
    assert(NULL != main_thread_looper);

    // the identifier is ignored when callback is not NULL (the ALooper_pollOnce always returns the ALOOPER_POLL_CALLBACK)
    AInputQueue_attachLooper(input_queue, main_thread_looper, 0, main_thread_looper_input_queue_callback, input_queue);
}

static void ANativeActivity_InputQueueDestroyed(ANativeActivity *native_activity, AInputQueue *input_queue)
{
    ALooper *looper = ALooper_forThread();
    assert(NULL != looper);

    AInputQueue_detachLooper(input_queue);
}

static int main_thread_looper_draw_callback(int fd, int, void *)
{
    // Evidently, the real "draw" is slower than the "request"
    // There are no message boundaries for "SOCK_STREAM", and we can read all the data once
    {
        uint8_t buf[4096];
        ssize_t res_recv;
        while ((-1 == (res_recv = recv(fd, buf, 4096U, 0))) && (EINTR == errno))
        {
            // pthread_yield();
            sched_yield();
        }
        assert(-1 != res_recv);
    }

    // Draw
    {
        float interval_time;
        uint64_t const tick_count_current_frame = tick_count_now();
        interval_time = static_cast<float>(static_cast<double>(tick_count_current_frame - g_wsi_state.m_tick_count_previous_frame) * g_wsi_state.m_tick_count_resolution);
        g_wsi_state.m_tick_count_previous_frame = tick_count_current_frame;

        // UI
        {
            ImGui_ImplAndroid_NewFrame();
            ImGui::NewFrame();

            ui_simulate(&g_wsi_state.m_ui_controller, &g_wsi_state.m_ui_model);
        }

        // User Camera
        {
            g_wsi_state.m_user_camera_controller.FrameMove(interval_time);

            DirectX::XMFLOAT3 eye_position;
            DirectX::XMStoreFloat3(&eye_position, g_wsi_state.m_user_camera_controller.GetEyePt());

            DirectX::XMFLOAT3 look_at_position;
            DirectX::XMStoreFloat3(&look_at_position, g_wsi_state.m_user_camera_controller.GetLookAtPt());

            DirectX::XMFLOAT3 up_direction;
            DirectX::XMStoreFloat3(&up_direction, g_wsi_state.m_user_camera_controller.GetUp());

            g_wsi_state.m_user_camera_model.m_eye_position[0] = eye_position.x;
            g_wsi_state.m_user_camera_model.m_eye_position[1] = eye_position.y;
            g_wsi_state.m_user_camera_model.m_eye_position[2] = eye_position.z;

            g_wsi_state.m_user_camera_model.m_look_at_position[0] = look_at_position.x;
            g_wsi_state.m_user_camera_model.m_look_at_position[1] = look_at_position.y;
            g_wsi_state.m_user_camera_model.m_look_at_position[2] = look_at_position.z;

            g_wsi_state.m_user_camera_model.m_up_direction[0] = up_direction.x;
            g_wsi_state.m_user_camera_model.m_up_direction[1] = up_direction.y;
            g_wsi_state.m_user_camera_model.m_up_direction[2] = up_direction.z;

            g_wsi_state.m_user_camera_model.m_fov = g_wsi_state.m_user_camera_controller.GetFOV();
            g_wsi_state.m_user_camera_model.m_aspect = static_cast<float>(g_wsi_state.m_window_width) / static_cast<float>(g_wsi_state.m_window_height);
            g_wsi_state.m_user_camera_model.m_near = g_wsi_state.m_user_camera_controller.GetNearClip();
            g_wsi_state.m_user_camera_model.m_far = g_wsi_state.m_user_camera_controller.GetFarClip();
        }
        g_wsi_state.m_renderer->draw(&g_wsi_state.m_ui_model, &g_wsi_state.m_user_camera_model);
    }

    return 1;
}

static void *draw_request_thread_main(void *)
{
    g_draw_request_thread_running = true;

    while (g_draw_request_thread_running)
    {
        // 60 FPS
        constexpr long const nano_second = static_cast<int64_t>(1000000000) / static_cast<int64_t>(60);
        static_assert(nano_second < 1000000000, "");

        // wait
        {
            struct timespec request = {0, nano_second};

            struct timespec remain;
            int res_nanosleep;
            while ((-1 == (res_nanosleep = nanosleep(&request, &remain))) && (EINTR == errno))
            {
                assert(remain.tv_nsec > 0 || remain.tv_sec > 0);
                request = remain;
            }
            assert(0 == res_nanosleep);
        }

        // draw request
        {
            // seven is the luck number
            uint8_t buf[1] = {7};
            ssize_t res_send;
            while ((-1 == (res_send = send(g_main_thread_looper_draw_callback_fd_write, buf, 1U, 0))) && (EINTR == errno))
            {
                // pthread_yield();
                sched_yield();
            }
            assert(1 == res_send);
        }
    }

    return NULL;
}

static int main_thread_looper_input_queue_callback(int fd, int, void *input_queue_void)
{
    AInputQueue *input_queue = static_cast<AInputQueue *>(input_queue_void);

    AInputEvent *input_event;
    while (AInputQueue_getEvent(input_queue, &input_event) >= 0)
    {
        if (0 == AInputQueue_preDispatchEvent(input_queue, input_event))
        {
            ImGui_ImplAndroid_HandleInputEvent(input_event);

            switch (AInputEvent_getType(input_event))
            {
            case AINPUT_EVENT_TYPE_MOTION:
            {
                int combined_action_code_and_pointer_index = AMotionEvent_getAction(input_event);

                int action_code = (combined_action_code_and_pointer_index & AMOTION_EVENT_ACTION_MASK);

                switch (action_code)
                {
                case AMOTION_EVENT_ACTION_DOWN:
                case AMOTION_EVENT_ACTION_UP:
                {
                    // TODO:
                    // g_wsi_state.m_user_camera_controller.HandleKeyDownMessage(mapped_key);
                    // g_wsi_state.m_user_camera_controller.HandleKeyUpMessage(mapped_key);
                }
                break;
                case AMOTION_EVENT_ACTION_POINTER_DOWN:
                case AMOTION_EVENT_ACTION_POINTER_UP:
                {
                    int pointer_index = ((combined_action_code_and_pointer_index & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT);
                    switch (pointer_index)
                    {
                    case 0:
                    {
                        // TODO:
                        // g_wsi_state.m_user_camera_controller.HandleKeyDownMessage(mapped_key);
                        // g_wsi_state.m_user_camera_controller.HandleKeyUpMessage(mapped_key);
                    }
                    break;
                    case 1:
                    {
                        float normalized_x = AMotionEvent_getX(input_event, pointer_index) / static_cast<float>(g_wsi_state.m_window_width);
                        float normalized_y = AMotionEvent_getY(input_event, pointer_index) / static_cast<float>(g_wsi_state.m_window_height);

                        bool left_button = false;
                        bool middle_button = false;
                        bool right_button = false;

                        g_wsi_state.m_user_camera_controller.HandleMouseMoveMessage(normalized_x, normalized_y, left_button, middle_button, right_button);
                    }
                    break;
                    }
                }
                break;
                case AMOTION_EVENT_ACTION_MOVE:
                {
                    size_t pointer_count = AMotionEvent_getPointerCount(input_event);
                    for (size_t pointer_index = 0; pointer_index < 2 && pointer_index < pointer_count; ++pointer_index)
                    {
                        switch (pointer_index)
                        {
                        case 0:
                        {
                            // TODO:
                            // g_wsi_state.m_user_camera_controller.HandleKeyDownMessage(mapped_key);
                            // g_wsi_state.m_user_camera_controller.HandleKeyUpMessage(mapped_key);
                        }
                        break;
                        case 1:
                        {
                            assert(1 == pointer_index);

                            float normalized_x = AMotionEvent_getX(input_event, pointer_index) / static_cast<float>(g_wsi_state.m_window_width);
                            float normalized_y = AMotionEvent_getY(input_event, pointer_index) / static_cast<float>(g_wsi_state.m_window_height);

                            bool left_button = false;
                            bool middle_button = false;
                            bool right_button = true;

                            g_wsi_state.m_user_camera_controller.HandleMouseMoveMessage(normalized_x, normalized_y, left_button, middle_button, right_button);
                        }
                        break;
                        }
                    }
                }
                break;
                }
            }
            break;
            }

            // The app will be "No response" if we don't call AInputQueue_finishEvent and pass the non-zero value for all events which is not pre-dispatched
            constexpr int const handled = 1;
            AInputQueue_finishEvent(input_queue, input_event, handled);
        }
    }

    return 1;
}
#else
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <xcb/xcb.h>
#include <stdio.h>

#include "../thirdparty/Brioche-ImGui/imgui.h"
#include "../thirdparty/Brioche-ImGui/backends/imgui_impl_glut.h"

int main(int argc, char *argv[])
{
    // Vulkan Validation Layer
#ifndef NDEBUG
    {
        // We assume that the "VkLayer_khronos_validation.json" is at the same directory of the executable file
        char dir_name[4096];
        ssize_t res_read_link = readlink("/proc/self/exe", dir_name, sizeof(dir_name) / sizeof(dir_name[0]));
        assert(-1 != res_read_link);

        for (int i = (res_read_link - 1); i > 0; --i)
        {
            if (L'/' == dir_name[i])
            {
                dir_name[i] = L'\0';
                break;
            }
        }

        int res_set_env_vk_layer_path = setenv("VK_LAYER_PATH", dir_name, 1);
        assert(0 == res_set_env_vk_layer_path);

        int res_set_env_ld_library_path = setenv("LD_LIBRARY_PATH", dir_name, 1);
        assert(0 == res_set_env_ld_library_path);
    }
#endif

    wsi_state_t wsi_state = {
        NULL,
        1280,
        720,
        1.0 / static_cast<double>(tick_count_per_second()),
        tick_count_now()};

    ui_controller_init(&wsi_state.m_ui_controller);

    xcb_connection_t *connection = NULL;
    xcb_screen_t *screen = NULL;
    {
        int screen_number;
        connection = xcb_connect(NULL, &screen_number);
        assert(0 == xcb_connection_has_error(connection));

        xcb_setup_t const *setup = xcb_get_setup(connection);

        int i = 0;
        for (xcb_screen_iterator_t screen_iterator = xcb_setup_roots_iterator(setup); screen_iterator.rem > 0; xcb_screen_next(&screen_iterator))
        {
            if (i == screen_number)
            {
                screen = screen_iterator.data;
                break;
            }
            ++i;
        }
    }

    constexpr uint8_t const depth = 32;
    xcb_visualid_t visual_id = -1;
    {
        for (xcb_depth_iterator_t depth_iterator = xcb_screen_allowed_depths_iterator(screen); depth_iterator.rem > 0; xcb_depth_next(&depth_iterator))
        {
            if (depth == depth_iterator.data->depth)
            {
                for (xcb_visualtype_iterator_t visual_iterator = xcb_depth_visuals_iterator(depth_iterator.data); visual_iterator.rem > 0; xcb_visualtype_next(&visual_iterator))
                {
                    if (XCB_VISUAL_CLASS_TRUE_COLOR == visual_iterator.data->_class)
                    {
                        visual_id = visual_iterator.data->visual_id;
                        break;
                    }
                }
                break;
            }
        }
    }

    xcb_colormap_t colormap = 0;
    {
        colormap = xcb_generate_id(connection);

        xcb_void_cookie_t cookie_create_colormap = xcb_create_colormap_checked(connection, XCB_COLORMAP_ALLOC_NONE, colormap, screen->root, visual_id);

        xcb_generic_error_t *error_create_colormap = xcb_request_check(connection, cookie_create_colormap);
        assert(NULL == error_create_colormap);
    }

    xcb_window_t window = 0;
    {
        window = xcb_generate_id(connection);

        // Both "border pixel" and "colormap" are required when the depth is NOT equal to the root window's.
        uint32_t value_mask = XCB_CW_BACK_PIXEL | XCB_CW_BORDER_PIXEL | XCB_CW_BACKING_STORE | XCB_CW_EVENT_MASK | XCB_CW_COLORMAP;

        // we need "XCB_EVENT_MASK_BUTTON_PRESS" and "XCB_EVENT_MASK_BUTTON_RELEASE" to use the "state" of the "motion notify" event
        uint32_t value_list[5] = {screen->black_pixel, 0, XCB_BACKING_STORE_NOT_USEFUL, XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_KEY_RELEASE | XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_BUTTON_RELEASE | XCB_EVENT_MASK_POINTER_MOTION | XCB_EVENT_MASK_STRUCTURE_NOTIFY, colormap};

        xcb_void_cookie_t cookie_create_window = xcb_create_window_checked(connection, depth, window, screen->root, 0, 0, wsi_state.m_window_width, wsi_state.m_window_height, 0, XCB_WINDOW_CLASS_INPUT_OUTPUT, visual_id, value_mask, value_list);

        xcb_generic_error_t *error_create_window = xcb_request_check(connection, cookie_create_window);
        assert(NULL == error_create_window);
    }

    xcb_atom_t atom_wm_protocols = 0;
    xcb_atom_t atom_wm_delete_window = 0;
    {
        xcb_intern_atom_cookie_t cookie_wm_protocols = xcb_intern_atom(connection, 0, 12U, "WM_PROTOCOLS");

        xcb_intern_atom_cookie_t cookie_wm_delete_window = xcb_intern_atom(connection, 0, 16U, "WM_DELETE_WINDOW");

        xcb_generic_error_t *error_intern_atom_reply_wm_protocols;
        xcb_intern_atom_reply_t *reply_wm_protocols = xcb_intern_atom_reply(connection, cookie_wm_protocols, &error_intern_atom_reply_wm_protocols);
        assert(NULL == error_intern_atom_reply_wm_protocols);
        atom_wm_protocols = reply_wm_protocols->atom;
        free(error_intern_atom_reply_wm_protocols);

        xcb_generic_error_t *error_intern_atom_reply_wm_delete_window;
        xcb_intern_atom_reply_t *reply_wm_delete_window = xcb_intern_atom_reply(connection, cookie_wm_delete_window, &error_intern_atom_reply_wm_delete_window);
        assert(NULL == error_intern_atom_reply_wm_delete_window);
        atom_wm_delete_window = reply_wm_delete_window->atom;
        free(error_intern_atom_reply_wm_delete_window);
    }

    {
        xcb_void_cookie_t cookie_change_property_wm_name = xcb_change_property(connection, XCB_PROP_MODE_REPLACE, window, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8 * sizeof(uint8_t), 14, "Brioche Puppet");

        xcb_void_cookie_t cookie_change_property_wm_protocols_delete_window = xcb_change_property_checked(connection, XCB_PROP_MODE_REPLACE, window, atom_wm_protocols, XCB_ATOM_ATOM, 8 * sizeof(uint32_t), sizeof(xcb_atom_t) / sizeof(uint32_t), &atom_wm_delete_window);

        xcb_generic_error_t *error_change_property_net_wm_name = xcb_request_check(connection, cookie_change_property_wm_name);
        assert(NULL == error_change_property_net_wm_name);

        xcb_generic_error_t *error_change_property_wm_protocols_delete_window = xcb_request_check(connection, cookie_change_property_wm_protocols_delete_window);
        assert(NULL == error_change_property_wm_protocols_delete_window);
    }

    {
        {
            IMGUI_CHECKVERSION();

            ImGui::CreateContext();

            ImGuiIO &io = ImGui::GetIO();
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
            // TODO: switch to glfw
            // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

            ImGui::StyleColorsDark();
        }

        ImGui_ImplGLUT_Init();

        ImGui_ImplGLUT_ReshapeFunc(wsi_state.m_window_width, wsi_state.m_window_height);
    }

    {
        struct brx_xcb_connection_T
        {
            xcb_connection_t *m_connection;
            xcb_visualid_t m_visual_id;
        };
        brx_xcb_connection_T brx_xcb_connection = {
            connection,
            visual_id};

        wsi_state.m_renderer = renderer_create(&brx_xcb_connection);

        user_camera_controller_init(&wsi_state.m_user_camera_controller);

        struct brx_xcb_window_T
        {
            xcb_connection_t *m_connection;
            xcb_window_t m_window;
        };

        brx_xcb_window_T brx_xcb_window = {
            connection,
            window};

        wsi_state.m_renderer->attach_window(&brx_xcb_window);
    }

    {
        xcb_void_cookie_t cookie_map_window = xcb_map_window_checked(connection, window);

        xcb_generic_error_t *error_map_window = xcb_request_check(connection, cookie_map_window);
        assert(NULL == error_map_window);
    }

    bool quit = false;
    while (!quit)
    {
        // WSI Event
        {
            xcb_generic_event_t *event;
            while ((event = xcb_poll_for_event(connection)) != NULL)
            {
                // The most significant bit(uint8_t(0X80)) in this code is set if the event was generated from a SendEvent request.
                // https://www.x.org/releases/current/doc/xproto/x11protocol.html#event_format
                switch (event->response_type & (~uint8_t(0X80)))
                {
                case XCB_KEY_PRESS:
                {
                    assert(XCB_KEY_PRESS == (event->response_type & (~uint8_t(0X80))));

                    xcb_key_press_event_t const *const key_press = reinterpret_cast<xcb_key_press_event_t *>(event);

                    bool shift = (0U != (key_press->state & XCB_MOD_MASK_SHIFT));
                    bool ctrl = (0U != (key_press->state & XCB_MOD_MASK_CONTROL));
                    bool alt = (0U != (key_press->state & XCB_MOD_MASK_1));

                    // evdev keycode
                    // https://gitlab.freedesktop.org/xkeyboard-config/xkeyboard-config/-/blob/master/keycodes/evdev
                    // Q
                    constexpr xcb_keycode_t const AD01 = 24;
                    // W
                    constexpr xcb_keycode_t const AD02 = 25;
                    // E
                    constexpr xcb_keycode_t const AD03 = 26;
                    // A
                    constexpr xcb_keycode_t const AC01 = 38;
                    // S
                    constexpr xcb_keycode_t const AC02 = 39;
                    // D
                    constexpr xcb_keycode_t const AC03 = 40;

                    D3DUtil_CameraKeys mapped_key;
                    switch (key_press->detail)
                    {
                    case AD02:
                    {
                        mapped_key = CAM_MOVE_FORWARD;
                    }
                    break;
                    case AC02:
                    {
                        mapped_key = CAM_MOVE_BACKWARD;
                    }
                    break;
                    case AC01:
                    {
                        mapped_key = CAM_STRAFE_LEFT;
                    }
                    break;
                    case AC03:
                    {
                        mapped_key = CAM_STRAFE_RIGHT;
                    }
                    break;
                    case AD01:
                    {
                        mapped_key = CAM_MOVE_UP;
                    }
                    break;
                    case AD03:
                    {
                        mapped_key = CAM_MOVE_DOWN;
                    }
                    break;
                    default:
                    {
                        mapped_key = CAM_UNKNOWN;
                    }
                    }

                    wsi_state.m_user_camera_controller.HandleKeyDownMessage(mapped_key);
                }
                break;
                case XCB_KEY_RELEASE:
                {
                    assert(XCB_KEY_RELEASE == (event->response_type & (~uint8_t(0X80))));

                    xcb_key_release_event_t const *const key_release = reinterpret_cast<xcb_key_release_event_t *>(event);

                    // evdev keycode
                    // https://gitlab.freedesktop.org/xkeyboard-config/xkeyboard-config/-/blob/master/keycodes/evdev
                    // Q
                    constexpr xcb_keycode_t const AD01 = 24;
                    // W
                    constexpr xcb_keycode_t const AD02 = 25;
                    // E
                    constexpr xcb_keycode_t const AD03 = 26;
                    // A
                    constexpr xcb_keycode_t const AC01 = 38;
                    // S
                    constexpr xcb_keycode_t const AC02 = 39;
                    // D
                    constexpr xcb_keycode_t const AC03 = 40;

                    D3DUtil_CameraKeys mapped_key;
                    switch (key_release->detail)
                    {
                    case AD02:
                    {
                        mapped_key = CAM_MOVE_FORWARD;
                    }
                    break;
                    case AC02:
                    {
                        mapped_key = CAM_MOVE_BACKWARD;
                    }
                    break;
                    case AC01:
                    {
                        mapped_key = CAM_STRAFE_LEFT;
                    }
                    break;
                    case AC03:
                    {
                        mapped_key = CAM_STRAFE_RIGHT;
                    }
                    break;
                    case AD01:
                    {
                        mapped_key = CAM_MOVE_UP;
                    }
                    break;
                    case AD03:
                    {
                        mapped_key = CAM_MOVE_DOWN;
                    }
                    break;
                    default:
                    {
                        mapped_key = CAM_UNKNOWN;
                    }
                    }

                    wsi_state.m_user_camera_controller.HandleKeyUpMessage(mapped_key);
                }
                break;
                case XCB_BUTTON_PRESS:
                {
                    assert(XCB_BUTTON_PRESS == (event->response_type & (~uint8_t(0X80))));

                    xcb_button_press_event_t const *const button_press_event = reinterpret_cast<xcb_button_press_event_t *>(event);

                    float window_x = button_press_event->event_x;
                    float window_y = button_press_event->event_y;

                    switch (button_press_event->detail)
                    {
                    case 1:
                    {
                        // Left
                        ImGui_ImplGLUT_MouseFunc(0, true, window_x, window_y);
                    }
                    break;
                    case 2:
                    {
                        // Middle
                        ImGui_ImplGLUT_MouseFunc(2, true, window_x, window_y);
                    }
                    break;
                    case 3:
                    {
                        // Right
                        ImGui_ImplGLUT_MouseFunc(1, true, window_x, window_y);
                    }
                    break;
                    case 4:
                    {
                        // Scroll Up
                        ImGui_ImplGLUT_MouseWheelFunc(true, window_x, window_y);
                    }
                    break;
                    case 5:
                    {
                        // Scroll Down
                        ImGui_ImplGLUT_MouseWheelFunc(false, window_x, window_y);
                    }
                    break;
                    }
                }
                break;
                case XCB_BUTTON_RELEASE:
                {
                    assert(XCB_BUTTON_RELEASE == (event->response_type & (~uint8_t(0X80))));

                    xcb_button_release_event_t const *const button_release_event = reinterpret_cast<xcb_button_release_event_t *>(event);

                    float window_x = button_release_event->event_x;
                    float window_y = button_release_event->event_y;

                    switch (button_release_event->detail)
                    {
                    case 1:
                    {
                        // Left
                        ImGui_ImplGLUT_MouseFunc(0, false, window_x, window_y);
                    }
                    break;
                    case 2:
                    {
                        // Middle
                        ImGui_ImplGLUT_MouseFunc(2, false, window_x, window_y);
                    }
                    break;
                    case 3:
                    {
                        // Right
                        ImGui_ImplGLUT_MouseFunc(1, false, window_x, window_y);
                    }
                    break;
                    case 4:
                    {
                        // Already Handled in Press Event
                    }
                    break;
                    case 5:
                    {
                        // Already Handled in Press Event
                    }
                    break;
                    }
                }
                break;
                case XCB_MOTION_NOTIFY:
                {
                    assert(XCB_MOTION_NOTIFY == (event->response_type & (~uint8_t(0X80))));

                    xcb_motion_notify_event_t const *const motion_notify = reinterpret_cast<xcb_motion_notify_event_t *>(event);

                    float window_x = motion_notify->event_x;
                    float window_y = motion_notify->event_y;

                    ImGui_ImplGLUT_MotionFunc(window_x, window_y);

                    bool left_button = (0U != (motion_notify->state & XCB_EVENT_MASK_BUTTON_1_MOTION));
                    bool middle_button = (0U != (motion_notify->state & XCB_EVENT_MASK_BUTTON_2_MOTION));
                    bool right_button = (0U != (motion_notify->state & XCB_EVENT_MASK_BUTTON_3_MOTION));

                    float normalized_x = static_cast<float>(window_x) / wsi_state.m_window_width;
                    float normalized_y = static_cast<float>(window_y) / wsi_state.m_window_height;

                    wsi_state.m_user_camera_controller.HandleMouseMoveMessage(normalized_x, normalized_y, left_button, middle_button, right_button);
                }
                break;
                case XCB_CONFIGURE_NOTIFY:
                {
                    assert(XCB_CONFIGURE_NOTIFY == (event->response_type & (~uint8_t(0X80))));

                    xcb_configure_notify_event_t *const configure_notify = reinterpret_cast<xcb_configure_notify_event_t *>(event);

                    uint16_t const new_width = configure_notify->width;
                    uint16_t const new_height = configure_notify->height;

                    if (wsi_state.m_window_width != new_width || wsi_state.m_window_height != new_height)
                    {
                        ImGui_ImplGLUT_ReshapeFunc(new_width, new_height);

                        if (new_width > 0 && new_height > 0)
                        {
                            wsi_state.m_renderer->on_window_resize();
                        }

                        wsi_state.m_window_width = new_width;
                        wsi_state.m_window_height = new_height;
                    }
                }
                break;
                case XCB_CLIENT_MESSAGE:
                {
                    assert(XCB_CLIENT_MESSAGE == (event->response_type & (~uint8_t(0X80))));

                    xcb_client_message_event_t *const client_message_event = reinterpret_cast<xcb_client_message_event_t *>(event);
                    assert(client_message_event->type == atom_wm_protocols && client_message_event->data.data32[0] == atom_wm_delete_window && client_message_event->window == window);

                    quit = true;
                }
                break;
                case 0:
                {
                    assert(0 == (event->response_type & (~uint8_t(0X80))));

                    xcb_generic_error_t *error = reinterpret_cast<xcb_generic_error_t *>(event);

                    printf("Error Code: %d Major Code: %d", static_cast<int>(error->error_code), static_cast<int>(error->major_code));
                }
                break;
                }

                free(event);
            }
        }

        // Render
        if (wsi_state.m_window_width > 0 && wsi_state.m_window_height > 0)
        {
            float interval_time;
            {
                uint64_t const tick_count_current_frame = tick_count_now();
                interval_time = static_cast<float>(static_cast<double>(tick_count_current_frame - wsi_state.m_tick_count_previous_frame) * wsi_state.m_tick_count_resolution);
                wsi_state.m_tick_count_previous_frame = tick_count_current_frame;
            }

            // UI
            {
                ImGui_ImplGLUT_NewFrame(interval_time);
                ImGui::NewFrame();

                ui_simulate(&wsi_state.m_ui_controller, &wsi_state.m_ui_model);
            }

            // User Camera
            {
                wsi_state.m_user_camera_controller.FrameMove(interval_time);

                DirectX::XMFLOAT3 eye_position;
                DirectX::XMStoreFloat3(&eye_position, wsi_state.m_user_camera_controller.GetEyePt());

                DirectX::XMFLOAT3 look_at_position;
                DirectX::XMStoreFloat3(&look_at_position, wsi_state.m_user_camera_controller.GetLookAtPt());

                DirectX::XMFLOAT3 up_direction;
                DirectX::XMStoreFloat3(&up_direction, wsi_state.m_user_camera_controller.GetUp());

                wsi_state.m_user_camera_model.m_eye_position[0] = eye_position.x;
                wsi_state.m_user_camera_model.m_eye_position[1] = eye_position.y;
                wsi_state.m_user_camera_model.m_eye_position[2] = eye_position.z;

                wsi_state.m_user_camera_model.m_look_at_position[0] = look_at_position.x;
                wsi_state.m_user_camera_model.m_look_at_position[1] = look_at_position.y;
                wsi_state.m_user_camera_model.m_look_at_position[2] = look_at_position.z;

                wsi_state.m_user_camera_model.m_up_direction[0] = up_direction.x;
                wsi_state.m_user_camera_model.m_up_direction[1] = up_direction.y;
                wsi_state.m_user_camera_model.m_up_direction[2] = up_direction.z;

                wsi_state.m_user_camera_model.m_fov = wsi_state.m_user_camera_controller.GetFOV();
                wsi_state.m_user_camera_model.m_aspect = static_cast<float>(wsi_state.m_window_width) / static_cast<float>(wsi_state.m_window_height);
                wsi_state.m_user_camera_model.m_near = wsi_state.m_user_camera_controller.GetNearClip();
                wsi_state.m_user_camera_model.m_far = wsi_state.m_user_camera_controller.GetFarClip();
            }

            wsi_state.m_renderer->draw(&wsi_state.m_ui_model, &wsi_state.m_user_camera_model);
        }
    }

    {
        xcb_void_cookie_t cookie_free_colormap = xcb_free_colormap_checked(connection, colormap);

        xcb_generic_error_t *error_free_colormap = xcb_request_check(connection, cookie_free_colormap);
        assert(NULL == error_free_colormap);
    }

    wsi_state.m_renderer->detach_window();

    renderer_destroy(wsi_state.m_renderer);

    ImGui_ImplGLUT_Shutdown();

    ImGui::DestroyContext();

    xcb_disconnect(connection);

    return 0;
}

#endif
#else
#error Unknown Platform
#endif

#elif defined(_MSC_VER)

#define NOMINMAX 1
#define WIN32_LEAN_AND_MEAN 1
#include <sdkddkver.h>
#include <Windows.h>
#include <windowsx.h>
#include <cassert>

#include <shobjidl_core.h>
#include <commdlg.h>
static HRESULT STDMETHODCALLTYPE NoRegCoCreate(const __wchar_t *dllName, REFCLSID rclsid, REFIID riid, void **ppv);

// DirectX 12 Agility SDK
extern "C" __declspec(dllexport) UINT const D3D12SDKVersion = 613U;
extern "C" __declspec(dllexport) char const *D3D12SDKPath = ".\\D3D12\\";

// ImGui
#include "../thirdparty/Brioche-ImGui/imgui.h"
#include "../thirdparty/Brioche-ImGui/backends/imgui_impl_win32.h"

static void *_internal_imgui_malloc_wrapper(size_t size, void *user_data);
static void _internal_imgui_free_wrapper(void *ptr, void *user_data);

#include <shellapi.h>
static bool _internal_imgui_platform_open_in_shell(ImGuiContext *, const char *path);

static LRESULT CALLBACK wnd_proc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

#include "../thirdparty/libiconv/include/iconv.h"

#include <algorithm>

#ifndef NDEBUG
extern "C" IMAGE_DOS_HEADER __ImageBase;

int wmain(int argc, wchar_t *argv[], wchar_t *envp[])
{
    HINSTANCE hInstance = reinterpret_cast<HINSTANCE>(&__ImageBase);
#else
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
#endif

    // use high priority on Windows to make the launch faster
    // we do NOT have the access right to do this on Linux
    {
        HANDLE current_process = GetCurrentProcess();

        BOOL res_set_priority_class = SetPriorityClass(current_process, HIGH_PRIORITY_CLASS);
        assert(FALSE != res_set_priority_class);

        HANDLE current_thread = GetCurrentThread();

        BOOL res_set_Thread_priority = SetThreadPriority(current_thread, THREAD_PRIORITY_HIGHEST);
        assert(FALSE != res_set_Thread_priority);
    }

    // ImGui_ImplWin32_EnableDpiAwareness();

    // Vulkan Validation Layer
#if !defined(NDEBUG)
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

    // Initialize
    wsi_state_t wsi_state = {
        true,
        NULL,
        1280,
        720,
        1.0 / static_cast<double>(_internal_tick_count_per_second()),
        _internal_tick_count_now(),
        true};
    ATOM window_class = 0;
    HWND window = NULL;
    {
        {
            WNDCLASSEXW const window_class_create_info = {
                sizeof(WNDCLASSEX),
                CS_OWNDC,
                wnd_proc,
                0,
                sizeof(LONG_PTR),
                hInstance,
                LoadIconW(NULL, IDI_APPLICATION),
                LoadCursorW(NULL, IDC_ARROW),
                (HBRUSH)(COLOR_WINDOW + 1),
                NULL,
                L"Brioche Puppet",
                LoadIconW(NULL, IDI_APPLICATION),
            };
            window_class = RegisterClassExW(&window_class_create_info);
            assert(0 != window_class);
        }

        {
            HWND const desktop_window = GetDesktopWindow();

            constexpr DWORD const dw_style = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME;
            constexpr DWORD const dw_ex_style = WS_EX_APPWINDOW;

            RECT rect;
            {
                HMONITOR const monitor = MonitorFromWindow(desktop_window, MONITOR_DEFAULTTONEAREST);

                MONITORINFOEXW monitor_info;
                monitor_info.cbSize = sizeof(MONITORINFOEXW);
                BOOL res_get_monitor_info = GetMonitorInfoW(monitor, &monitor_info);
                assert(FALSE != res_get_monitor_info);

                rect = RECT{(monitor_info.rcWork.left + monitor_info.rcWork.right) / 2 - wsi_state.m_window_width / 2,
                            (monitor_info.rcWork.bottom + monitor_info.rcWork.top) / 2 - wsi_state.m_window_height / 2,
                            (monitor_info.rcWork.left + monitor_info.rcWork.right) / 2 + wsi_state.m_window_width / 2,
                            (monitor_info.rcWork.bottom + monitor_info.rcWork.top) / 2 + wsi_state.m_window_height / 2};

                BOOL const res_adjust_window_rest = AdjustWindowRectEx(&rect, dw_style, FALSE, dw_ex_style);
                assert(FALSE != res_adjust_window_rest);
            }

            window = CreateWindowExW(dw_ex_style, MAKEINTATOM(window_class), L"Brioche Puppet", dw_style, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, desktop_window, NULL, hInstance, &wsi_state);
            assert(NULL != window);
        }

        {
            IMGUI_CHECKVERSION();

            ImGui::SetAllocatorFunctions(_internal_imgui_malloc_wrapper, _internal_imgui_free_wrapper);

            ImGui::CreateContext();

            ImGuiIO &io = ImGui::GetIO();
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

            ImGui::StyleColorsDark();
        }

        ImGui_ImplWin32_Init(window);

        ImGuiPlatformIO &platform_io = ImGui::GetPlatformIO();
        assert(!platform_io.Platform_OpenInShellFn(NULL, NULL));
        platform_io.Platform_OpenInShellFn = _internal_imgui_platform_open_in_shell;

        wsi_state.m_anari_device = brx_anari_new_device(NULL);

        ui_model_init(wsi_state.m_anari_device, &wsi_state.m_ui_model);

        ui_controller_init(wsi_state.m_anari_device, &wsi_state.m_ui_controller);

        wsi_state.m_anari_device->frame_attach_window(window);

        ShowWindow(window, SW_SHOWDEFAULT);
        {
            BOOL result_update_window = UpdateWindow(window);
            assert(FALSE != result_update_window);
        }
    }

    // Run
    while (wsi_state.m_running)
    {
        MSG msg;
        while (::PeekMessageW(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }

        // Render
        if (wsi_state.m_running && wsi_state.m_window_width > 0 && wsi_state.m_window_height > 0)
        {
            float interval_time;
            {
                uint64_t const tick_count_current_frame = _internal_tick_count_now();
                interval_time = static_cast<float>(static_cast<double>(tick_count_current_frame - wsi_state.m_tick_count_previous_frame) * wsi_state.m_tick_count_resolution);
                wsi_state.m_tick_count_previous_frame = tick_count_current_frame;
            }

            // User Camera
            user_camera_simulate(interval_time, wsi_state.m_anari_device, &wsi_state.m_ui_model, &wsi_state.m_ui_controller);

            // UI
            if (wsi_state.m_ui_view)
            {
                ImGui_ImplWin32_NewFrame();
                ImGui::NewFrame();

                ui_simulate(window, wsi_state.m_anari_device, &wsi_state.m_ui_model, &wsi_state.m_ui_controller);

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
                        assert(false);
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

                            if ((NULL != video_detector) || (NULL != animation_instance))
                            {
                                skeleton_instance->step();
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
                        assert(false);
                    }
                }
            }

            wsi_state.m_anari_device->renderer_render_frame(wsi_state.m_ui_view);
        }
    }

    // Destroy
    {
        wsi_state.m_anari_device->frame_detach_window();

        ui_model_uninit(wsi_state.m_anari_device, &wsi_state.m_ui_model);

        brx_anari_release_device(wsi_state.m_anari_device);
    }

    ImGui_ImplWin32_Shutdown();

    ImGui::DestroyContext();

    DestroyWindow(window);

    UnregisterClassW(MAKEINTATOM(window_class), hInstance);

    return 0;
}

// Forward declare message handler from imgui_impl_win32.cpp
extern "C" IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static LRESULT CALLBACK wnd_proc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    LRESULT const res_imgui = ImGui_ImplWin32_WndProcHandler(hWnd, Msg, wParam, lParam);

    switch (Msg)
    {
    case WM_SIZE:
    {
        wsi_state_t *const wsi_state = reinterpret_cast<wsi_state_t *>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));
        assert(NULL != wsi_state);

        WORD const new_width = LOWORD(lParam);
        WORD const new_height = HIWORD(lParam);

        if (wsi_state->m_window_width != new_width || wsi_state->m_window_height != new_height)
        {
            if (new_width > 0 && new_height > 0)
            {
                wsi_state->m_anari_device->frame_resize_window();
            }
            wsi_state->m_window_width = new_width;
            wsi_state->m_window_height = new_height;
        }
    }
        return 0;
    case WM_ERASEBKGND:
        return 1;
    case WM_KEYDOWN:
    {
        wsi_state_t *const wsi_state = reinterpret_cast<wsi_state_t *>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));
        assert(NULL != wsi_state);

        D3DUtil_CameraKeys mappedKey;
        switch (wParam)
        {
        case 'W':
        {
            mappedKey = CAM_MOVE_FORWARD;
        }
        break;
        case 'S':
        {
            mappedKey = CAM_MOVE_BACKWARD;
        }
        break;
        case 'A':
        {
            mappedKey = CAM_STRAFE_LEFT;
        }
        break;
        case 'D':
        {
            mappedKey = CAM_STRAFE_RIGHT;
        }
        break;
        case 'Q':
        {
            mappedKey = CAM_MOVE_UP;
        }
        break;
        case 'E':
        {
            mappedKey = CAM_MOVE_DOWN;
        }
        break;
        case VK_ESCAPE:
        {
            wsi_state->m_ui_view = (!wsi_state->m_ui_view);
            mappedKey = CAM_UNKNOWN;
        }
        break;
        default:
        {
            mappedKey = CAM_UNKNOWN;
        }
        };

        wsi_state->m_ui_controller.m_first_person_camera.HandleKeyDownMessage(mappedKey);
    }
        return 0;
    case WM_KEYUP:
    {
        wsi_state_t *const wsi_state = reinterpret_cast<wsi_state_t *>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));
        assert(NULL != wsi_state);

        D3DUtil_CameraKeys mappedKey;
        switch (wParam)
        {
        case 'W':
        {
            mappedKey = CAM_MOVE_FORWARD;
        }
        break;
        case 'S':
        {
            mappedKey = CAM_MOVE_BACKWARD;
        }
        break;
        case 'A':
        {
            mappedKey = CAM_STRAFE_LEFT;
        }
        break;
        case 'D':
        {
            mappedKey = CAM_STRAFE_RIGHT;
        }
        break;
        case 'Q':
        {
            mappedKey = CAM_MOVE_UP;
        }
        break;
        case 'E':
        {
            mappedKey = CAM_MOVE_DOWN;
        }
        break;
        default:
        {
            mappedKey = CAM_UNKNOWN;
        }
        };

        wsi_state->m_ui_controller.m_first_person_camera.HandleKeyUpMessage(mappedKey);
    }
        return 0;
    case WM_MOUSEMOVE:
    {
        wsi_state_t *const wsi_state = reinterpret_cast<wsi_state_t *>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));
        assert(NULL != wsi_state);

        int window_x = GET_X_LPARAM(lParam);
        int window_y = GET_Y_LPARAM(lParam);

        float normalized_x = static_cast<float>(static_cast<double>(window_x) / static_cast<double>(wsi_state->m_window_width));
        float normalizedY = static_cast<float>(static_cast<double>(window_y) / static_cast<double>(wsi_state->m_window_height));

        bool leftButton = (0U != (wParam & MK_LBUTTON));
        bool middleButton = (0U != (wParam & MK_MBUTTON));
        bool rightButton = (0U != (wParam & MK_RBUTTON));

        wsi_state->m_ui_controller.m_first_person_camera.HandleMouseMoveMessage(normalized_x, normalizedY, leftButton, middleButton, rightButton);
    }
        return 0;
    case WM_CREATE:
    {
        CREATESTRUCTW *const create_struct = reinterpret_cast<CREATESTRUCTW *>(lParam);

        wsi_state_t *const wsi_state = static_cast<wsi_state_t *>(create_struct->lpCreateParams);
        assert(NULL != wsi_state);

        SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(wsi_state));
    }
        return 0;
    case WM_DESTROY:
    {
        wsi_state_t *const wsi_state = reinterpret_cast<wsi_state_t *>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));
        assert(NULL != wsi_state);

        wsi_state->m_running = false;
    }
        return 0;
    default:
        if (0 != res_imgui)
        {
            return res_imgui;
        }
        else
        {
            return DefWindowProcW(hWnd, Msg, wParam, lParam);
        }
    }
}

static void *_internal_imgui_malloc_wrapper(size_t size, void *)
{
    return mcrt_malloc(size, 16);
}
static void _internal_imgui_free_wrapper(void *ptr, void *)
{
    return mcrt_free(ptr);
}

static HRESULT STDMETHODCALLTYPE NoRegCoCreate(const __wchar_t *dllName, REFCLSID rclsid, REFIID riid, void **ppv)
{
    HMODULE dynamic_library = GetModuleHandleW(dllName);
    assert(NULL != dynamic_library);

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

static bool _internal_imgui_platform_open_in_shell(ImGuiContext *, const char *path)
{
    mcrt_wstring path_utf16;
    {
        assert(path_utf16.empty());

        mcrt_string src_utf8 = path;
        mcrt_wstring &dst_utf16 = path_utf16;

        assert(dst_utf16.empty());

        if (!src_utf8.empty())
        {
            // Allocate the same number of UTF-16 code units as UTF-8 code units. Encoding
            // as UTF-16 should always require the same amount or less code units than the
            // UTF-8 encoding.  Allocate one extra byte for the null terminator though,
            // so that someone calling DstUTF16.data() gets a null terminated string.
            // We resize down later so we don't have to worry that this over allocates.
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

    return (INT_PTR)::ShellExecuteW(NULL, L"open", path_utf16.c_str(), NULL, NULL, SW_SHOWDEFAULT) > 32;
}

static inline bool _internal_platform_get_file_timestamp_and_data(wchar_t const *file_name_utf16, uint64_t *out_file_timestamp, mcrt_vector<uint8_t> *out_file_data);

extern bool _internal_platform_get_open_file_name(void *platform_context, size_t filter_count, char const *const *filter_names, char const *const *filter_specs, int &inout_file_type_index, mcrt_string *out_file_name, uint64_t *out_file_timestamp, mcrt_vector<uint8_t> *out_file_data)
{
    // we may reference https://github.com/mlabbe/nativefiledialog

    mcrt_wstring file_name_utf16;
    {
        HRESULT res_co_initialize_ex = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
        assert(SUCCEEDED(res_co_initialize_ex));

        IFileOpenDialog *file_open_dialog = NULL;
        // HRESULT res_co_create_instance = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&file_open_dialog));
        assert(reinterpret_cast<void *>(7U) != GetOpenFileNameW);
        HRESULT res_co_create_instance = (reinterpret_cast<void *>(7U) != GetOpenFileNameW) ? NoRegCoCreate(L"Comdlg32.dll", CLSID_FileOpenDialog, IID_PPV_ARGS(&file_open_dialog)) : E_FAIL;
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

            for (size_t filter_index = 0U; filter_index < filter_count; ++filter_index)
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
        bool file_timestamp_and_data_found = _internal_platform_get_file_timestamp_and_data(file_name_utf16.c_str(), out_file_timestamp, out_file_data);

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

static inline bool _internal_platform_get_file_timestamp_and_data(wchar_t const *file_name_utf16, uint64_t *out_file_timestamp, mcrt_vector<uint8_t> *out_file_data)
{
    bool file_timestamp_found;
    bool file_data_found;
    {
        HANDLE file = CreateFileW(file_name_utf16, FILE_GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (INVALID_HANDLE_VALUE != file)
        {
            FILETIME last_write_time;
            BOOL res_get_file_time = GetFileTime(file, NULL, NULL, &last_write_time);
            if ((FALSE != res_get_file_time))
            {
                (*out_file_timestamp) = ((static_cast<uint64_t>(last_write_time.dwHighDateTime) << 32ULL) | static_cast<uint64_t>(last_write_time.dwLowDateTime));
                file_timestamp_found = true;
            }
            else
            {
                assert(0);
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
                                assert(0);
                                out_file_data->clear();
                                file_data_found = false;
                            }
                        }
                        else
                        {
                            assert(0);
                            out_file_data->clear();
                            file_data_found = false;
                        }
                    }
                    else
                    {
                        assert(0);
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
                assert(0);
                assert(out_file_data->empty());
                file_data_found = false;
            }

            BOOL res_close_handle = CloseHandle(file);
            assert(FALSE != res_close_handle);
        }
        else
        {
            assert(ERROR_FILE_NOT_FOUND == GetLastError());
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

extern bool _internal_platform_get_file_timestamp_and_data(char const *file_name, uint64_t *out_file_timestamp, mcrt_vector<uint8_t> *out_file_data)
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

    return _internal_platform_get_file_timestamp_and_data(file_name_utf16.c_str(), out_file_timestamp, out_file_data);
}

#else
#error Unknown Compiler
#endif

#if defined(__GNUC__)

#include <time.h>

extern uint64_t tick_count_per_second()
{
    constexpr uint64_t const tick_count_per_second = 1000000000ULL;
    return tick_count_per_second;
}

extern uint64_t tick_count_now()
{
    struct timespec time_monotonic;
    int result_clock_get_time_monotonic = clock_gettime(CLOCK_MONOTONIC, &time_monotonic);
    assert(0 == result_clock_get_time_monotonic);

    uint64_t const tick_count_now = static_cast<uint64_t>(1000000000ULL) * static_cast<uint64_t>(time_monotonic.tv_sec) + static_cast<uint64_t>(time_monotonic.tv_nsec);
    return tick_count_now;
}

#elif defined(_MSC_VER)

#define NOMINMAX 1
#define WIN32_LEAN_AND_MEAN 1
#include <sdkddkver.h>
#include <Windows.h>

static inline uint64_t _internal_tick_count_per_second()
{
    LARGE_INTEGER int64_frequency;
    BOOL result_query_performance_frequency = QueryPerformanceFrequency(&int64_frequency);
    assert(NULL != result_query_performance_frequency);

    uint64_t const tick_count_per_second = static_cast<uint64_t>(int64_frequency.QuadPart);
    return tick_count_per_second;
}

extern uint64_t _internal_tick_count_now()
{
    LARGE_INTEGER int64_performance_count;
    BOOL result_query_performance_counter = QueryPerformanceCounter(&int64_performance_count);
    assert(NULL != result_query_performance_counter);

    uint64_t const tick_count_now = static_cast<uint64_t>(int64_performance_count.QuadPart);
    return tick_count_now;
}

#else
#error Unknown Compiler
#endif
