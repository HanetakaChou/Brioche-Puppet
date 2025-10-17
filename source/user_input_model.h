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

#ifndef _USER_INPUT_MODEL_H_
#define _USER_INPUT_MODEL_H_ 1

#include "../thirdparty/McRT-Malloc/include/mcrt_vector.h"
#include "../thirdparty/McRT-Malloc/include/mcrt_string.h"
#include "../thirdparty/McRT-Malloc/include/mcrt_unordered_map.h"
#include "../thirdparty/Brioche-Analytic-Rendering-Interface/include/brx_anari.h"
#include "../thirdparty/Brioche-Motion/include/brx_motion.h"

static constexpr uint64_t const INVALID_TIMESTAMP = 0XFFFFFFFFFFFFFFFFU;

struct ui_video_capture_model_t
{
    brx_motion_video_capture *m_video_capture;
};

struct ui_asset_motion_model_t
{
    mcrt_vector<brx_motion_animation *> m_animations;
};

struct ui_asset_model_model_t
{
    mcrt_vector<brx_anari_surface_group *> m_surface_groups;
    mcrt_vector<brx_motion_skeleton *> m_skeletons;
};

struct ui_asset_image_model_t
{
    bool m_force_srgb;
    brx_anari_image *m_image;
};

struct ui_video_detector_model_t
{
    mcrt_string m_name;
    mcrt_string m_video_capture;
    brx_motion_video_detector *m_video_detector;
};

struct ui_instance_motion_model_t
{
    mcrt_string m_name;
    mcrt_string m_asset_motion;
    uint32_t m_animation_index;
    brx_motion_animation_instance *m_animation_instance;
};

struct ui_instance_model_model_t
{
    mcrt_string m_name;
    mcrt_string m_asset_model;
    uint32_t m_surface_group_index;
    brx_anari_surface_group_instance *m_surface_group_instance;
    brx_motion_skeleton_instance *m_skeleton_instance;
    uint64_t m_video_detector;
    uint64_t m_instance_motion;
    float m_model_transform_rotation_roll;
    float m_model_transform_rotation_pitch;
    float m_model_transform_rotation_yaw;
    float m_model_transform_translation_x;
    float m_model_transform_translation_y;
    float m_model_transform_translation_z;
};

struct ui_model_t
{
    mcrt_unordered_map<mcrt_string, ui_video_capture_model_t> m_video_captures;
    mcrt_unordered_map<mcrt_string, ui_asset_motion_model_t> m_asset_motions;
    mcrt_unordered_map<mcrt_string, ui_asset_model_model_t> m_asset_models;
    mcrt_unordered_map<mcrt_string, ui_asset_image_model_t> m_asset_images;
    mcrt_unordered_map<uint64_t, ui_video_detector_model_t> m_video_detectors;
    mcrt_unordered_map<uint64_t, ui_instance_motion_model_t> m_instance_motions;
    mcrt_unordered_map<uint64_t, ui_instance_model_model_t> m_instance_models;
};

extern void ui_model_init(brx_anari_device *device, ui_model_t *out_user_camera_model);

extern void ui_model_uninit(brx_anari_device *device, ui_model_t *out_user_camera_model);

#endif