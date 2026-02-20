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

#include "user_input_model.h"
#if defined(__GNUC__)
// GCC or CLANG
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#include <DirectXMath.h>
#include <DirectXCollision.h>
#pragma GCC diagnostic pop
#elif defined(_MSC_VER)
// MSVC or CLANG-CL
#include <DirectXMath.h>
#include <DirectXCollision.h>
#else
#error Unknown Compiler
#endif
#include "../thirdparty/Brioche-Shader-Language/include/brx_reversed_z.h"
#include <algorithm>
#include <cassert>

extern void ui_model_init(brx_anari_device *device, ui_model_t *ui_model)
{
    // Camera
    {
        // [PerspectiveCamera.constructor](https://github.com/KhronosGroup/glTF-Sample-Renderer/blob/main/source/gltf/camera.js#L236)
        constexpr float const fovy = DirectX::XM_PIDIV4;
        constexpr float const near = 0.01F;
        // TODO: change "far plane" based on the AABB
        constexpr float const far = 1000.0F;

        // [UserCamera.resetView](https://github.com/KhronosGroup/glTF-Sample-Renderer/blob/main/source/gltf/user_camera.js#L251)
        DirectX::BoundingBox scene_bounding_box;

        // TODO:
        scene_bounding_box.Center.x = 0.0F;
        scene_bounding_box.Center.y = 1.0F;
        scene_bounding_box.Center.z = 0.0F;
        scene_bounding_box.Extents.x = 1.0F;
        scene_bounding_box.Extents.y = 1.0F;
        scene_bounding_box.Extents.z = 1.0F;

        DirectX::XMMATRIX projection_transform = brx_DirectX_Math_Matrix_PerspectiveFovRH_ReversedZ(fovy, 1.0F, near, far);

        DirectX::XMFLOAT2 axis_length(scene_bounding_box.Extents.x, scene_bounding_box.Extents.y);
        DirectX::XMVECTOR zoom = DirectX::XMVector2TransformNormal(DirectX::XMLoadFloat2(&axis_length), projection_transform);
        float zoom_x = DirectX::XMVectorGetX(zoom);
        float zoom_y = DirectX::XMVectorGetY(zoom);
        float distance = std::max(zoom_x, zoom_y);

        // glTF
        // RH
        // Right -X
        // Up +Y
        // Forward +Z

        DirectX::XMFLOAT3 target = scene_bounding_box.Center;

        DirectX::XMVECTOR look_at_position = DirectX::XMLoadFloat3(&target);

        DirectX::XMFLOAT3 const look_direction(0.0F, 0.0F, -1.0F);

        DirectX::XMFLOAT3 eye_position;
        DirectX::XMStoreFloat3(&eye_position, DirectX::XMVectorSubtract(look_at_position, DirectX::XMVectorScale(DirectX::XMLoadFloat3(&look_direction), distance)));

        ui_model->m_camera_position = brx_anari_vec3{eye_position.x, eye_position.y, eye_position.z};
        ui_model->m_camera_direction = brx_anari_vec3{look_direction.x, look_direction.y, look_direction.z};
        ui_model->m_camera_up = brx_anari_vec3{0.0F, 1.0F, 0.0F};
        ui_model->m_camera_fovy = fovy;
        ui_model->m_camera_near = near;
        ui_model->m_camera_far = far;
    }

    // Renderer
    {
        ui_model->m_renderer_background_r = 0.0F;
        ui_model->m_renderer_background_g = 1.0F;
        ui_model->m_renderer_background_b = 0.0F;
        ui_model->m_renderer_style = BRX_ANARI_RENDERER_STYLE_TOON_SHADING;
        ui_model->m_renderer_toon_shading_first_shade_color_step = 0.8;
        ui_model->m_renderer_toon_shading_first_shade_color_feather = 0.0001;
        ui_model->m_renderer_toon_shading_second_shade_color_step = 0.5;
        ui_model->m_renderer_toon_shading_second_shade_color_feather = 0.0001;
        ui_model->m_renderer_toon_shading_base_color = 0.7843138;
        ui_model->m_renderer_toon_shading_first_shade_color = 0.49411768;
        ui_model->m_renderer_toon_shading_second_shade_color = 0.19607845;
        ui_model->m_renderer_toon_shading_rim_light_power = 0.1;
        ui_model->m_renderer_toon_shading_rim_light_inside_mask = 0.0001;
    }

    ui_model->m_physics_ragdoll_quality = BRX_MOTION_PHYSICS_RAGDOLL_QUALITY_LOW;

    // Directional
    {
        ui_model->m_directional_lighting_visible = true;
        ui_model->m_directional_lighting_color_r = 1.0F;
        ui_model->m_directional_lighting_color_g = 1.0F;
        ui_model->m_directional_lighting_color_b = 1.0F;
        ui_model->m_directional_lighting_irradiance = 1.0F;
        // Rembrandt Lighting
        // zenith 45 degree
        // azimuth 45 degree
        ui_model->m_directional_lighting_direction_x = 0.5F;
        ui_model->m_directional_lighting_direction_y = -std::sqrt(0.5F);
        ui_model->m_directional_lighting_direction_z = -0.5F;
    }

    // Quad
    {
        ui_model->m_quad_light_enable_debug_renderer = true;
    }

    // HDRI
    {
        device->hdri_light_set_up(brx_anari_vec3{0.0F, 1.0F, 0.0F});
        device->hdri_light_set_direction(brx_anari_vec3{0.0F, 0.0F, -1.0F});
        ui_model->m_hdri_light_enable_skybox_renderer = true;
    }

    assert(ui_model->m_video_captures.empty());
    assert(ui_model->m_asset_motions.empty());
    assert(ui_model->m_asset_models.empty());
    assert(ui_model->m_asset_images.empty());
    assert(ui_model->m_video_detectors.empty());
    assert(ui_model->m_instance_motions.empty());
    assert(ui_model->m_motion_receivers.empty());
    assert(ui_model->m_instance_models.empty());
    assert(ui_model->m_area_lightings.empty());
}

extern void ui_model_uninit(brx_anari_device *device, ui_model_t *ui_model)
{
    ui_model->m_area_lightings.clear();

    for (auto &instance_model : ui_model->m_instance_models)
    {
        if (NULL != instance_model.second.m_surface_group_instance)
        {
            device->world_release_surface_group_instance(instance_model.second.m_surface_group_instance);
            instance_model.second.m_surface_group_instance = NULL;
        }
        else
        {
            assert(false);
        }

        if (NULL != instance_model.second.m_skeleton_instance)
        {
            brx_motion_destroy_skeleton_instance(instance_model.second.m_skeleton_instance);
            instance_model.second.m_skeleton_instance = NULL;
        }
        else
        {
            assert(INVALID_TIMESTAMP == instance_model.second.m_morph_video_detector);
            assert(INVALID_TIMESTAMP == instance_model.second.m_morph_instance_motion);
            assert(INVALID_TIMESTAMP == instance_model.second.m_joint_video_detector);
            assert(INVALID_TIMESTAMP == instance_model.second.m_joint_instance_motion);
        }
    }
    ui_model->m_instance_models.clear();

    for (auto &motion_receiver : ui_model->m_motion_receivers)
    {
        if (NULL != motion_receiver.second.m_motion_receiver)
        {
            brx_motion_destroy_motion_receiver(motion_receiver.second.m_motion_receiver);
            motion_receiver.second.m_motion_receiver = NULL;
        }
        else
        {
            assert(false);
        }
    }
    ui_model->m_motion_receivers.clear();

    for (auto &instance_motion : ui_model->m_instance_motions)
    {
        if (NULL != instance_motion.second.m_animation_instance)
        {
            brx_motion_destroy_animation_instance(instance_motion.second.m_animation_instance);
            instance_motion.second.m_animation_instance = NULL;
        }
        else
        {
            assert(false);
        }
    }
    ui_model->m_instance_motions.clear();

    for (auto &video_detector : ui_model->m_video_detectors)
    {
        if (NULL != video_detector.second.m_video_detector)
        {
            brx_motion_destroy_video_detector(video_detector.second.m_video_detector);
            video_detector.second.m_video_detector = NULL;
        }
        else
        {
            assert(false);
        }
    }
    ui_model->m_video_detectors.clear();

    for (auto &asset_image : ui_model->m_asset_images)
    {
        if (NULL != asset_image.second.m_image)
        {
            device->release_image(asset_image.second.m_image);
            asset_image.second.m_image = NULL;
        }
        else
        {
            assert(false);
        }
    }
    ui_model->m_asset_images.clear();

    for (auto &asset_model : ui_model->m_asset_models)
    {
        assert(!asset_model.second.m_surface_groups.empty());
        assert(!asset_model.second.m_skeletons.empty());
        assert(asset_model.second.m_surface_groups.size() == asset_model.second.m_skeletons.size());

        for (brx_anari_surface_group *const surface_group : asset_model.second.m_surface_groups)
        {
            if (NULL != surface_group)
            {
                device->release_surface_group(surface_group);
            }
            else
            {
                assert(false);
            }
        }
        asset_model.second.m_surface_groups.clear();

        for (brx_motion_skeleton *const skeleton : asset_model.second.m_skeletons)
        {
            if (NULL != skeleton)
            {
                brx_motion_destroy_skeleton(skeleton);
            }
            else
            {
                // TODO
                // assert(false);
            }
        }
        asset_model.second.m_skeletons.clear();
    }
    ui_model->m_asset_models.clear();

    for (auto &asset_motion : ui_model->m_asset_motions)
    {
        assert(!asset_motion.second.m_animations.empty());

        for (brx_motion_animation *const animation : asset_motion.second.m_animations)
        {
            if (NULL != animation)
            {
                brx_motion_destroy_animation(animation);
            }
            else
            {
                assert(false);
            }
        }
        asset_motion.second.m_animations.clear();
    }
    ui_model->m_asset_motions.clear();

    for (auto &video_capture : ui_model->m_video_captures)
    {
        if (NULL != video_capture.second.m_video_capture)
        {
            brx_motion_destroy_video_capture(video_capture.second.m_video_capture);
            video_capture.second.m_video_capture = NULL;
        }
        else
        {
            assert(false);
        }
    }
    ui_model->m_video_captures.clear();
}
