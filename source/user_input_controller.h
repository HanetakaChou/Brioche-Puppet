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

#ifndef _USER_INPUT_CONTROLLER_H_
#define _USER_INPUT_CONTROLLER_H_ 1

#include "user_input_model.h"
#include "../thirdparty/DXUT/Optional/DXUTcamera.h"
#include "../thirdparty/Brioche-Analytic-Rendering-Interface/include/brx_anari.h"
#include "../thirdparty/Brioche-Motion/include/brx_motion.h"

enum VIDEO_CAPTURE_TYPE
{
    VIDEO_CAPTURE_TYPE_INVALID = -1,
    VIDEO_CAPTURE_TYPE_MIN = 0,
    VIDEO_CAPTURE_TYPE_CAMERA = 0,
    VIDEO_CAPTURE_TYPE_FILE = 1,
    VIDEO_CAPTURE_TYPE_MAX = 1,
    VIDEO_CAPTURE_TYPE_COUNT = 2
};

enum ANIMATION_INPUT_TYPE
{
    ANIMATION_INPUT_TYPE_MIN = 0,
    ANIMATION_INPUT_TYPE_INVALID = 0,
    ANIMATION_INPUT_TYPE_VIDEO_DETECTOR = 1,
    ANIMATION_INPUT_TYPE_INSTANCE_MOTION = 2,
    ANIMATION_INPUT_TYPE_MAX = 2,
    ANIMATION_INPUT_TYPE_COUNT = 3
};

static constexpr int const INVALID_INT_INDEX = -1;

static constexpr uint64_t const INVALID_TIMESTAMP = 0XFFFFFFFFFFFFFFFFU;

struct ui_instance_model_controller_t
{
    ANIMATION_INPUT_TYPE m_animation_input_type;
};


struct ui_controller_t
{
    int m_language_index;

    VIDEO_CAPTURE_TYPE m_new_video_capture_type;
    int m_new_video_capture_camera_index;
    int m_new_video_capture_camera_resolution_index;
    int m_new_video_capture_camera_fps_index;
    int m_new_video_capture_get_open_file_name_file_type_index;
    mcrt_string m_tree_view_selected_video_capture;

    int m_import_asset_motion_get_open_file_name_file_type_index;
    mcrt_string m_tree_view_selected_asset_motion;

    int m_import_asset_model_get_open_file_name_file_type_index;
    mcrt_string m_tree_view_selected_asset_model;

    bool m_import_asset_image_force_srgb;
    int m_import_asset_image_get_open_file_name_file_type_index;
    mcrt_string m_tree_view_selected_asset_image;

    mcrt_vector<char> m_new_video_detector_name;
    mcrt_string m_new_video_detector_selected_video_capture;
    uint32_t m_new_video_detector_face_count;
    uint32_t m_new_video_detector_pose_count;
    bool m_new_video_detector_force_gpu;
    bool m_new_video_detector_enable_debug_renderer;
    uint64_t m_tree_view_selected_video_detector;

    mcrt_vector<char> m_new_instance_motion_name;
    mcrt_string m_new_instance_motion_selected_asset_motion;
    int m_new_instance_motion_selected_animation_index;
    uint64_t m_tree_view_selected_instance_motion;

    mcrt_vector<char> m_new_instance_model_name;
    mcrt_string m_new_instance_model_selected_asset_model;
    int m_new_instance_model_selected_surface_group_index;
    ANIMATION_INPUT_TYPE m_new_instance_model_animation_input_type;
    uint64_t m_new_instance_model_selected_video_detector;
    uint32_t m_new_instance_model_selected_video_detector_face_index;
    uint32_t m_new_instance_model_selected_video_detector_pose_index;
    uint64_t m_new_instance_model_selected_instance_motion;
    float m_new_instance_model_transform_rotation_roll;
    float m_new_instance_model_transform_rotation_pitch;
    float m_new_instance_model_transform_rotation_yaw;
    float m_new_instance_model_transform_translation_x;
    float m_new_instance_model_transform_translation_y;
    float m_new_instance_model_transform_translation_z;
    uint64_t m_tree_view_selected_instance_model;

    mcrt_unordered_map<uint64_t, ui_instance_model_controller_t> m_instance_controllers;
};

struct user_camera_controller_t
{
    CDXUTFirstPersonCamera m_first_person_camera;
};

extern void ui_controller_init(ui_controller_t *ui_controller);

extern void ui_simulate(void *platform_context, brx_anari_device *device, ui_model_t *ui_model, ui_controller_t *ui_controller);

extern void user_camera_controller_init(brx_anari_device *device, user_camera_model_t const *user_camera_model, user_camera_controller_t *user_camera_controller);

extern void user_camera_simulate(float interval_time, brx_anari_device *device, user_camera_model_t *user_camera_model, user_camera_controller_t *user_camera_controller);

#endif