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

#include "user_input_controller.h"
#include "../thirdparty/Brioche-ImGui/imgui.h"
#include "../thirdparty/Brioche-Asset-Import/include/brx_asset_import_image.h"
#include "../thirdparty/Brioche-Asset-Import/include/brx_asset_import_scene.h"
#include "../thirdparty/McRT-Malloc/include/mcrt_tick_count.h"
#include "../thirdparty/Brioche-Window-System-Integration/include/brx_wsi.h"
#include "../thirdparty/bitcoin/src/crypto/sha256.h"
#include <cinttypes>
#include <climits>
#include <cstring>
#include <cstdio>
#include <algorithm>

static_assert(BRX_ANARI_UINT32_INDEX_INVALID == BRX_ASSET_IMPORT_UINT32_INDEX_INVALID, "");

static inline BRX_ANARI_MORPH_TARGET_NAME const wrap(BRX_ASSET_IMPORT_MORPH_TARGET_NAME const morph_target_name)
{
    static_assert(static_cast<uint32_t>(BRX_ANARI_MORPH_TARGET_NAME_MMD_BROW_HAPPY) == static_cast<uint32_t>(BRX_ASSET_IMPORT_MORPH_TARGET_NAME_MMD_BROW_HAPPY), "");
    static_assert(static_cast<uint32_t>(BRX_ANARI_MORPH_TARGET_NAME_MMD_BROW_ANGRY) == static_cast<uint32_t>(BRX_ASSET_IMPORT_MORPH_TARGET_NAME_MMD_BROW_ANGRY), "");
    static_assert(static_cast<uint32_t>(BRX_ANARI_MORPH_TARGET_NAME_MMD_BROW_SAD) == static_cast<uint32_t>(BRX_ASSET_IMPORT_MORPH_TARGET_NAME_MMD_BROW_SAD), "");
    static_assert(static_cast<uint32_t>(BRX_ANARI_MORPH_TARGET_NAME_MMD_BROW_SURPRISED) == static_cast<uint32_t>(BRX_ASSET_IMPORT_MORPH_TARGET_NAME_MMD_BROW_SURPRISED), "");
    static_assert(static_cast<uint32_t>(BRX_ANARI_MORPH_TARGET_NAME_MMD_EYE_BLINK) == static_cast<uint32_t>(BRX_ASSET_IMPORT_MORPH_TARGET_NAME_MMD_EYE_BLINK), "");
    static_assert(static_cast<uint32_t>(BRX_ANARI_MORPH_TARGET_NAME_MMD_EYE_BLINK_L) == static_cast<uint32_t>(BRX_ASSET_IMPORT_MORPH_TARGET_NAME_MMD_EYE_BLINK_L), "");
    static_assert(static_cast<uint32_t>(BRX_ANARI_MORPH_TARGET_NAME_MMD_EYE_BLINK_R) == static_cast<uint32_t>(BRX_ASSET_IMPORT_MORPH_TARGET_NAME_MMD_EYE_BLINK_R), "");
    static_assert(static_cast<uint32_t>(BRX_ANARI_MORPH_TARGET_NAME_MMD_EYE_HAPPY) == static_cast<uint32_t>(BRX_ASSET_IMPORT_MORPH_TARGET_NAME_MMD_EYE_HAPPY), "");
    static_assert(static_cast<uint32_t>(BRX_ANARI_MORPH_TARGET_NAME_MMD_EYE_ANGRY) == static_cast<uint32_t>(BRX_ASSET_IMPORT_MORPH_TARGET_NAME_MMD_EYE_ANGRY), "");
    static_assert(static_cast<uint32_t>(BRX_ANARI_MORPH_TARGET_NAME_MMD_EYE_SAD) == static_cast<uint32_t>(BRX_ASSET_IMPORT_MORPH_TARGET_NAME_MMD_EYE_SAD), "");
    static_assert(static_cast<uint32_t>(BRX_ANARI_MORPH_TARGET_NAME_MMD_EYE_SURPRISED) == static_cast<uint32_t>(BRX_ASSET_IMPORT_MORPH_TARGET_NAME_MMD_EYE_SURPRISED), "");
    static_assert(static_cast<uint32_t>(BRX_ANARI_MORPH_TARGET_NAME_MMD_MOUTH_A) == static_cast<uint32_t>(BRX_ASSET_IMPORT_MORPH_TARGET_NAME_MMD_MOUTH_A), "");
    static_assert(static_cast<uint32_t>(BRX_ANARI_MORPH_TARGET_NAME_MMD_MOUTH_I) == static_cast<uint32_t>(BRX_ASSET_IMPORT_MORPH_TARGET_NAME_MMD_MOUTH_I), "");
    static_assert(static_cast<uint32_t>(BRX_ANARI_MORPH_TARGET_NAME_MMD_MOUTH_U) == static_cast<uint32_t>(BRX_ASSET_IMPORT_MORPH_TARGET_NAME_MMD_MOUTH_U), "");
    static_assert(static_cast<uint32_t>(BRX_ANARI_MORPH_TARGET_NAME_MMD_MOUTH_E) == static_cast<uint32_t>(BRX_ASSET_IMPORT_MORPH_TARGET_NAME_MMD_MOUTH_E), "");
    static_assert(static_cast<uint32_t>(BRX_ANARI_MORPH_TARGET_NAME_MMD_MOUTH_O) == static_cast<uint32_t>(BRX_ASSET_IMPORT_MORPH_TARGET_NAME_MMD_MOUTH_O), "");
    static_assert(static_cast<uint32_t>(BRX_ANARI_MORPH_TARGET_NAME_MMD_MOUTH_HAPPY) == static_cast<uint32_t>(BRX_ASSET_IMPORT_MORPH_TARGET_NAME_MMD_MOUTH_HAPPY), "");
    static_assert(static_cast<uint32_t>(BRX_ANARI_MORPH_TARGET_NAME_MMD_MOUTH_ANGRY) == static_cast<uint32_t>(BRX_ASSET_IMPORT_MORPH_TARGET_NAME_MMD_MOUTH_ANGRY), "");
    static_assert(static_cast<uint32_t>(BRX_ANARI_MORPH_TARGET_NAME_MMD_MOUTH_SAD) == static_cast<uint32_t>(BRX_ASSET_IMPORT_MORPH_TARGET_NAME_MMD_MOUTH_SAD), "");
    static_assert(static_cast<uint32_t>(BRX_ANARI_MORPH_TARGET_NAME_MMD_MOUTH_SURPRISED) == static_cast<uint32_t>(BRX_ASSET_IMPORT_MORPH_TARGET_NAME_MMD_MOUTH_SURPRISED), "");
    static_assert(static_cast<uint32_t>(BRX_ANARI_MORPH_TARGET_NAME_MMD_COUNT) == static_cast<uint32_t>(BRX_ASSET_IMPORT_MORPH_TARGET_NAME_MMD_COUNT), "");
    return static_cast<BRX_ANARI_MORPH_TARGET_NAME const>(morph_target_name);
}

static inline brx_anari_surface_vertex_position const *wrap(brx_asset_import_surface_vertex_position const *surface_vertex_position)
{
    static_assert(sizeof(brx_anari_surface_vertex_position) == sizeof(brx_asset_import_surface_vertex_position), "");
    static_assert(offsetof(brx_anari_surface_vertex_position, m_position) == offsetof(brx_asset_import_surface_vertex_position, m_position), "");
    return reinterpret_cast<brx_anari_surface_vertex_position const *>(surface_vertex_position);
}

static inline brx_anari_surface_vertex_varying const *wrap(brx_asset_import_surface_vertex_varying const *surface_vertex_varying)
{
    static_assert(sizeof(brx_anari_surface_vertex_varying) == sizeof(brx_asset_import_surface_vertex_varying), "");
    static_assert(offsetof(brx_anari_surface_vertex_varying, m_normal) == offsetof(brx_asset_import_surface_vertex_varying, m_normal), "");
    static_assert(offsetof(brx_anari_surface_vertex_varying, m_tangent) == offsetof(brx_asset_import_surface_vertex_varying, m_tangent), "");
    static_assert(offsetof(brx_anari_surface_vertex_varying, m_texcoord) == offsetof(brx_asset_import_surface_vertex_varying, m_texcoord), "");
    return reinterpret_cast<brx_anari_surface_vertex_varying const *>(surface_vertex_varying);
}

static inline brx_anari_surface_vertex_blending const *wrap(brx_asset_import_surface_vertex_blending const *surface_vertex_blending)
{
    static_assert(sizeof(brx_anari_surface_vertex_blending) == sizeof(brx_asset_import_surface_vertex_blending), "");
    static_assert(offsetof(brx_anari_surface_vertex_blending, m_indices) == offsetof(brx_asset_import_surface_vertex_blending, m_indices), "");
    static_assert(offsetof(brx_anari_surface_vertex_blending, m_weights) == offsetof(brx_asset_import_surface_vertex_blending, m_weights), "");
    return reinterpret_cast<brx_anari_surface_vertex_blending const *>(surface_vertex_blending);
}

static inline brx_anari_vec3 const wrap(brx_asset_import_vec3 const vec3)
{
    static_assert(sizeof(brx_anari_vec3) == sizeof(brx_asset_import_vec3), "");
    static_assert(offsetof(brx_anari_vec3, m_x) == offsetof(brx_asset_import_vec3, m_x), "");
    static_assert(offsetof(brx_anari_vec3, m_y) == offsetof(brx_asset_import_vec3, m_y), "");
    static_assert(offsetof(brx_anari_vec3, m_z) == offsetof(brx_asset_import_vec3, m_z), "");
    return (*reinterpret_cast<brx_anari_vec3 const *>(&vec3));
}

static inline brx_anari_vec4 const wrap(brx_asset_import_vec4 const vec4)
{
    static_assert(sizeof(brx_anari_vec4) == sizeof(brx_asset_import_vec4), "");
    static_assert(offsetof(brx_anari_vec4, m_x) == offsetof(brx_asset_import_vec4, m_x), "");
    static_assert(offsetof(brx_anari_vec4, m_y) == offsetof(brx_asset_import_vec4, m_y), "");
    static_assert(offsetof(brx_anari_vec4, m_z) == offsetof(brx_asset_import_vec4, m_z), "");
    static_assert(offsetof(brx_anari_vec4, m_w) == offsetof(brx_asset_import_vec4, m_w), "");
    return (*reinterpret_cast<brx_anari_vec4 const *>(&vec4));
}

static_assert(BRX_MOTION_UINT32_INDEX_INVALID == BRX_ASSET_IMPORT_UINT32_INDEX_INVALID, "");

static inline BRX_MOTION_MORPH_TARGET_NAME const *wrap(BRX_ASSET_IMPORT_MORPH_TARGET_NAME const *morph_target_name)
{
    static_assert(static_cast<uint32_t>(BRX_MOTION_MORPH_TARGET_NAME_MMD_BROW_HAPPY) == static_cast<uint32_t>(BRX_ASSET_IMPORT_MORPH_TARGET_NAME_MMD_BROW_HAPPY), "");
    static_assert(static_cast<uint32_t>(BRX_MOTION_MORPH_TARGET_NAME_MMD_BROW_ANGRY) == static_cast<uint32_t>(BRX_ASSET_IMPORT_MORPH_TARGET_NAME_MMD_BROW_ANGRY), "");
    static_assert(static_cast<uint32_t>(BRX_MOTION_MORPH_TARGET_NAME_MMD_BROW_SAD) == static_cast<uint32_t>(BRX_ASSET_IMPORT_MORPH_TARGET_NAME_MMD_BROW_SAD), "");
    static_assert(static_cast<uint32_t>(BRX_MOTION_MORPH_TARGET_NAME_MMD_BROW_SURPRISED) == static_cast<uint32_t>(BRX_ASSET_IMPORT_MORPH_TARGET_NAME_MMD_BROW_SURPRISED), "");
    static_assert(static_cast<uint32_t>(BRX_MOTION_MORPH_TARGET_NAME_MMD_EYE_BLINK) == static_cast<uint32_t>(BRX_ASSET_IMPORT_MORPH_TARGET_NAME_MMD_EYE_BLINK), "");
    static_assert(static_cast<uint32_t>(BRX_MOTION_MORPH_TARGET_NAME_MMD_EYE_BLINK_L) == static_cast<uint32_t>(BRX_ASSET_IMPORT_MORPH_TARGET_NAME_MMD_EYE_BLINK_L), "");
    static_assert(static_cast<uint32_t>(BRX_MOTION_MORPH_TARGET_NAME_MMD_EYE_BLINK_R) == static_cast<uint32_t>(BRX_ASSET_IMPORT_MORPH_TARGET_NAME_MMD_EYE_BLINK_R), "");
    static_assert(static_cast<uint32_t>(BRX_MOTION_MORPH_TARGET_NAME_MMD_EYE_HAPPY) == static_cast<uint32_t>(BRX_ASSET_IMPORT_MORPH_TARGET_NAME_MMD_EYE_HAPPY), "");
    static_assert(static_cast<uint32_t>(BRX_MOTION_MORPH_TARGET_NAME_MMD_EYE_ANGRY) == static_cast<uint32_t>(BRX_ASSET_IMPORT_MORPH_TARGET_NAME_MMD_EYE_ANGRY), "");
    static_assert(static_cast<uint32_t>(BRX_MOTION_MORPH_TARGET_NAME_MMD_EYE_SAD) == static_cast<uint32_t>(BRX_ASSET_IMPORT_MORPH_TARGET_NAME_MMD_EYE_SAD), "");
    static_assert(static_cast<uint32_t>(BRX_MOTION_MORPH_TARGET_NAME_MMD_EYE_SURPRISED) == static_cast<uint32_t>(BRX_ASSET_IMPORT_MORPH_TARGET_NAME_MMD_EYE_SURPRISED), "");
    static_assert(static_cast<uint32_t>(BRX_MOTION_MORPH_TARGET_NAME_MMD_MOUTH_A) == static_cast<uint32_t>(BRX_ASSET_IMPORT_MORPH_TARGET_NAME_MMD_MOUTH_A), "");
    static_assert(static_cast<uint32_t>(BRX_MOTION_MORPH_TARGET_NAME_MMD_MOUTH_I) == static_cast<uint32_t>(BRX_ASSET_IMPORT_MORPH_TARGET_NAME_MMD_MOUTH_I), "");
    static_assert(static_cast<uint32_t>(BRX_MOTION_MORPH_TARGET_NAME_MMD_MOUTH_U) == static_cast<uint32_t>(BRX_ASSET_IMPORT_MORPH_TARGET_NAME_MMD_MOUTH_U), "");
    static_assert(static_cast<uint32_t>(BRX_MOTION_MORPH_TARGET_NAME_MMD_MOUTH_E) == static_cast<uint32_t>(BRX_ASSET_IMPORT_MORPH_TARGET_NAME_MMD_MOUTH_E), "");
    static_assert(static_cast<uint32_t>(BRX_MOTION_MORPH_TARGET_NAME_MMD_MOUTH_O) == static_cast<uint32_t>(BRX_ASSET_IMPORT_MORPH_TARGET_NAME_MMD_MOUTH_O), "");
    static_assert(static_cast<uint32_t>(BRX_MOTION_MORPH_TARGET_NAME_MMD_MOUTH_HAPPY) == static_cast<uint32_t>(BRX_ASSET_IMPORT_MORPH_TARGET_NAME_MMD_MOUTH_HAPPY), "");
    static_assert(static_cast<uint32_t>(BRX_MOTION_MORPH_TARGET_NAME_MMD_MOUTH_ANGRY) == static_cast<uint32_t>(BRX_ASSET_IMPORT_MORPH_TARGET_NAME_MMD_MOUTH_ANGRY), "");
    static_assert(static_cast<uint32_t>(BRX_MOTION_MORPH_TARGET_NAME_MMD_MOUTH_SAD) == static_cast<uint32_t>(BRX_ASSET_IMPORT_MORPH_TARGET_NAME_MMD_MOUTH_SAD), "");
    static_assert(static_cast<uint32_t>(BRX_MOTION_MORPH_TARGET_NAME_MMD_MOUTH_SURPRISED) == static_cast<uint32_t>(BRX_ASSET_IMPORT_MORPH_TARGET_NAME_MMD_MOUTH_SURPRISED), "");
    static_assert(static_cast<uint32_t>(BRX_MOTION_MORPH_TARGET_NAME_MMD_COUNT) == static_cast<uint32_t>(BRX_ASSET_IMPORT_MORPH_TARGET_NAME_MMD_COUNT), "");
    return reinterpret_cast<BRX_MOTION_MORPH_TARGET_NAME const *>(morph_target_name);
}

static inline BRX_MOTION_SKELETON_JOINT_NAME const *wrap(BRX_ASSET_IMPORT_SKELETON_JOINT_NAME const *skeleton_joint_name)
{
    static_assert(static_cast<uint32_t>(BRX_MOTION_SKELETON_JOINT_NAME_MMD_CONTROL_NODE) == static_cast<uint32_t>(BRX_ASSET_IMPORT_SKELETON_JOINT_NAME_MMD_CONTROL_NODE), "");
    return reinterpret_cast<BRX_MOTION_SKELETON_JOINT_NAME const *>(skeleton_joint_name);
}

static inline BRX_MOTION_SKELETON_JOINT_CONSTRAINT_NAME const *wrap(BRX_ASSET_IMPORT_SKELETON_JOINT_CONSTRAINT_NAME const *skeleton_joint_constraint_name)
{
    static_assert(static_cast<uint32_t>(BRX_MOTION_SKELETON_JOINT_CONSTRAINT_NAME_MMD_IK_RIGHT_ANKLE) == static_cast<uint32_t>(BRX_ASSET_IMPORT_SKELETON_JOINT_CONSTRAINT_NAME_MMD_IK_RIGHT_ANKLE), "");
    static_assert(static_cast<uint32_t>(BRX_MOTION_SKELETON_JOINT_CONSTRAINT_NAME_MMD_IK_RIGHT_TOE_TIP) == static_cast<uint32_t>(BRX_ASSET_IMPORT_SKELETON_JOINT_CONSTRAINT_NAME_MMD_IK_RIGHT_TOE_TIP), "");
    static_assert(static_cast<uint32_t>(BRX_MOTION_SKELETON_JOINT_CONSTRAINT_NAME_MMD_IK_LEFT_ANKLE) == static_cast<uint32_t>(BRX_ASSET_IMPORT_SKELETON_JOINT_CONSTRAINT_NAME_MMD_IK_LEFT_ANKLE), "");
    static_assert(static_cast<uint32_t>(BRX_MOTION_SKELETON_JOINT_CONSTRAINT_NAME_MMD_IK_LEFT_TOE_TIP) == static_cast<uint32_t>(BRX_ASSET_IMPORT_SKELETON_JOINT_CONSTRAINT_NAME_MMD_IK_LEFT_TOE_TIP), "");
    return reinterpret_cast<BRX_MOTION_SKELETON_JOINT_CONSTRAINT_NAME const *>(skeleton_joint_constraint_name);
}

static inline brx_motion_rigid_transform const *wrap(brx_asset_import_rigid_transform const *rigid_transform)
{
    static_assert(sizeof(brx_motion_rigid_transform) == sizeof(brx_asset_import_rigid_transform), "");
    static_assert(offsetof(brx_motion_rigid_transform, m_rotation) == offsetof(brx_asset_import_rigid_transform, m_rotation), "");
    static_assert(offsetof(brx_motion_rigid_transform, m_translation) == offsetof(brx_asset_import_rigid_transform, m_translation), "");
    return reinterpret_cast<brx_motion_rigid_transform const *>(rigid_transform);
}

static inline brx_motion_skeleton_joint_constraint const *wrap(brx_asset_import_skeleton_joint_constraint const *skeleton_joint_constraint)
{
    static_assert(sizeof(brx_motion_skeleton_joint_constraint) == sizeof(brx_asset_import_skeleton_joint_constraint), "");
    static_assert(offsetof(brx_motion_skeleton_joint_constraint, m_constraint_type) == offsetof(brx_asset_import_skeleton_joint_constraint, m_constraint_type), "");
    static_assert(offsetof(brx_motion_skeleton_joint_constraint, m_copy_transform) == offsetof(brx_asset_import_skeleton_joint_constraint, m_copy_transform), "");
    static_assert(offsetof(brx_motion_skeleton_joint_constraint, m_copy_transform.m_source_joint_index) == offsetof(brx_asset_import_skeleton_joint_constraint, m_copy_transform.m_source_joint_index), "");
    static_assert(offsetof(brx_motion_skeleton_joint_constraint, m_copy_transform.m_source_weight_count) == offsetof(brx_asset_import_skeleton_joint_constraint, m_copy_transform.m_source_weight_count), "");
    static_assert(offsetof(brx_motion_skeleton_joint_constraint, m_copy_transform.m_source_weights) == offsetof(brx_asset_import_skeleton_joint_constraint, m_copy_transform.m_source_weights), "");
    static_assert(offsetof(brx_motion_skeleton_joint_constraint, m_copy_transform.m_destination_joint_index) == offsetof(brx_asset_import_skeleton_joint_constraint, m_copy_transform.m_destination_joint_index), "");
    static_assert(offsetof(brx_motion_skeleton_joint_constraint, m_copy_transform.m_copy_rotation) == offsetof(brx_asset_import_skeleton_joint_constraint, m_copy_transform.m_copy_rotation), "");
    static_assert(offsetof(brx_motion_skeleton_joint_constraint, m_copy_transform.m_copy_translation) == offsetof(brx_asset_import_skeleton_joint_constraint, m_copy_transform.m_copy_translation), "");
    static_assert(offsetof(brx_motion_skeleton_joint_constraint, m_inverse_kinematics) == offsetof(brx_asset_import_skeleton_joint_constraint, m_inverse_kinematics), "");
    static_assert(offsetof(brx_motion_skeleton_joint_constraint, m_inverse_kinematics.m_ik_end_effector_index) == offsetof(brx_asset_import_skeleton_joint_constraint, m_inverse_kinematics.m_ik_end_effector_index), "");
    static_assert(offsetof(brx_motion_skeleton_joint_constraint, m_inverse_kinematics.m_ik_joint_count) == offsetof(brx_asset_import_skeleton_joint_constraint, m_inverse_kinematics.m_ik_joint_count), "");
    static_assert(offsetof(brx_motion_skeleton_joint_constraint, m_inverse_kinematics.m_ik_joint_indices) == offsetof(brx_asset_import_skeleton_joint_constraint, m_inverse_kinematics.m_ik_joint_indices), "");
    static_assert(offsetof(brx_motion_skeleton_joint_constraint, m_inverse_kinematics.m_target_joint_index) == offsetof(brx_asset_import_skeleton_joint_constraint, m_inverse_kinematics.m_target_joint_index), "");
    static_assert(offsetof(brx_motion_skeleton_joint_constraint, m_inverse_kinematics.m_ik_two_joints_hinge_joint_axis_local_space) == offsetof(brx_asset_import_skeleton_joint_constraint, m_inverse_kinematics.m_ik_two_joints_hinge_joint_axis_local_space), "");
    static_assert(offsetof(brx_motion_skeleton_joint_constraint, m_inverse_kinematics.m_cosine_max_ik_two_joints_hinge_joint_angle) == offsetof(brx_asset_import_skeleton_joint_constraint, m_inverse_kinematics.m_cosine_max_ik_two_joints_hinge_joint_angle), "");
    static_assert(offsetof(brx_motion_skeleton_joint_constraint, m_inverse_kinematics.m_cosine_min_ik_two_joints_hinge_joint_angle) == offsetof(brx_asset_import_skeleton_joint_constraint, m_inverse_kinematics.m_cosine_min_ik_two_joints_hinge_joint_angle), "");
    return reinterpret_cast<brx_motion_skeleton_joint_constraint const *>(skeleton_joint_constraint);
}

static inline brx_motion_physics_rigid_body const *wrap(brx_asset_import_physics_rigid_body const *physics_rigid_body)
{
    static_assert(sizeof(brx_motion_physics_rigid_body) == sizeof(brx_asset_import_physics_rigid_body), "");
    static_assert(offsetof(brx_motion_physics_rigid_body, m_model_space_transform) == offsetof(brx_asset_import_physics_rigid_body, m_model_space_transform), "");
    static_assert(offsetof(brx_motion_physics_rigid_body, m_shape_type) == offsetof(brx_asset_import_physics_rigid_body, m_shape_type), "");
    static_assert(offsetof(brx_motion_physics_rigid_body, m_shape_size) == offsetof(brx_asset_import_physics_rigid_body, m_shape_size), "");
    static_assert(offsetof(brx_motion_physics_rigid_body, m_motion_type) == offsetof(brx_asset_import_physics_rigid_body, m_motion_type), "");
    static_assert(offsetof(brx_motion_physics_rigid_body, m_collision_filter_group) == offsetof(brx_asset_import_physics_rigid_body, m_collision_filter_group), "");
    static_assert(offsetof(brx_motion_physics_rigid_body, m_collision_filter_mask) == offsetof(brx_asset_import_physics_rigid_body, m_collision_filter_mask), "");
    static_assert(offsetof(brx_motion_physics_rigid_body, m_mass) == offsetof(brx_asset_import_physics_rigid_body, m_mass), "");
    static_assert(offsetof(brx_motion_physics_rigid_body, m_linear_damping) == offsetof(brx_asset_import_physics_rigid_body, m_linear_damping), "");
    static_assert(offsetof(brx_motion_physics_rigid_body, m_angular_damping) == offsetof(brx_asset_import_physics_rigid_body, m_angular_damping), "");
    static_assert(offsetof(brx_motion_physics_rigid_body, m_friction) == offsetof(brx_asset_import_physics_rigid_body, m_friction), "");
    static_assert(offsetof(brx_motion_physics_rigid_body, m_restitution) == offsetof(brx_asset_import_physics_rigid_body, m_restitution), "");
    return reinterpret_cast<brx_motion_physics_rigid_body const *>(physics_rigid_body);
}

static inline brx_motion_physics_constraint const *wrap(brx_asset_import_physics_constraint const *physics_constrain)
{
    static_assert(sizeof(brx_motion_physics_constraint) == sizeof(brx_asset_import_physics_constraint), "");
    static_assert(offsetof(brx_motion_physics_constraint, m_rigid_body_a_index) == offsetof(brx_asset_import_physics_constraint, m_rigid_body_a_index), "");
    static_assert(offsetof(brx_motion_physics_constraint, m_rigid_body_b_index) == offsetof(brx_asset_import_physics_constraint, m_rigid_body_b_index), "");
    static_assert(offsetof(brx_motion_physics_constraint, m_constraint_type) == offsetof(brx_asset_import_physics_constraint, m_constraint_type), "");
    static_assert(offsetof(brx_motion_physics_constraint, m_pivot) == offsetof(brx_asset_import_physics_constraint, m_pivot), "");
    static_assert(offsetof(brx_motion_physics_constraint, m_twist_axis) == offsetof(brx_asset_import_physics_constraint, m_twist_axis), "");
    static_assert(offsetof(brx_motion_physics_constraint, m_plane_axis) == offsetof(brx_asset_import_physics_constraint, m_plane_axis), "");
    static_assert(offsetof(brx_motion_physics_constraint, m_normal_axis) == offsetof(brx_asset_import_physics_constraint, m_normal_axis), "");
    static_assert(offsetof(brx_motion_physics_constraint, m_twist_limit) == offsetof(brx_asset_import_physics_constraint, m_twist_limit), "");
    static_assert(offsetof(brx_motion_physics_constraint, m_plane_limit) == offsetof(brx_asset_import_physics_constraint, m_plane_limit), "");
    static_assert(offsetof(brx_motion_physics_constraint, m_normal_limit) == offsetof(brx_asset_import_physics_constraint, m_normal_limit), "");
    return reinterpret_cast<brx_motion_physics_constraint const *>(physics_constrain);
}

static inline brx_motion_ragdoll_direct_mapping const *wrap(brx_asset_import_ragdoll_direct_mapping const *ragdoll_direct_mapping)
{
    static_assert(sizeof(brx_motion_ragdoll_direct_mapping) == sizeof(brx_asset_import_ragdoll_direct_mapping), "");
    static_assert(offsetof(brx_motion_ragdoll_direct_mapping, m_joint_index_a) == offsetof(brx_asset_import_ragdoll_direct_mapping, m_joint_index_a), "");
    static_assert(offsetof(brx_motion_ragdoll_direct_mapping, m_joint_index_b) == offsetof(brx_asset_import_ragdoll_direct_mapping, m_joint_index_b), "");
    static_assert(offsetof(brx_motion_ragdoll_direct_mapping, m_a_to_b_transform_model_space) == offsetof(brx_asset_import_ragdoll_direct_mapping, m_a_to_b_transform_model_space), "");
    return reinterpret_cast<brx_motion_ragdoll_direct_mapping const *>(ragdoll_direct_mapping);
}

extern bool internal_platform_get_open_file_name(uint32_t filter_count, char const *const *filter_names, char const *const *filter_specs, int &inout_file_type_index, mcrt_string *out_file_name, uint64_t *out_file_timestamp, mcrt_vector<uint8_t> *out_file_data);

extern bool internal_platform_get_file_timestamp_and_data(char const *file_name, uint64_t *out_file_timestamp, mcrt_vector<uint8_t> *out_file_data);

static inline brx_anari_image *internal_load_asset_image(uint8_t const *asset_image_url_data, uint32_t const asset_image_url_size, char const *asset_model_directory_name, bool force_srgb, brx_anari_device *device, ui_model_t *ui_model);

static inline brx_anari_image *internal_load_asset_image_file(char const *asset_image_file_name, uint64_t asset_image_file_timestamp, void const *asset_image_file_data_base, size_t asset_image_file_data_size, bool force_srgb, brx_anari_device *device, ui_model_t *ui_model);

static inline brx_anari_image *internal_load_asset_image_data(void const *asset_image_file_data_base, size_t asset_image_file_data_size, bool force_srgb, brx_anari_device *device, ui_model_t *ui_model);

extern void ui_controller_init(brx_anari_device *device, ui_controller_t *ui_controller)
{
    // camera
    {
        brx_anari_vec3 const camera_position = device->camera_get_position();

        brx_anari_vec3 const camera_direction = device->camera_get_direction();

        brx_anari_vec3 const camera_up = device->camera_get_up();

        DirectX::XMFLOAT3 const eye_position(camera_position.m_x, camera_position.m_y, camera_position.m_z);

        DirectX::XMFLOAT3 const eye_direction(camera_direction.m_x, camera_direction.m_y, camera_direction.m_z);

        DirectX::XMFLOAT3 const up_direction(camera_up.m_x, camera_up.m_y, camera_up.m_z);

        ui_controller->m_first_person_camera.SetEyePt(eye_position);

        ui_controller->m_first_person_camera.SetEyeDir(eye_direction);

        ui_controller->m_first_person_camera.SetUpDir(up_direction);
    }

    constexpr uint32_t const MAX_INPUT_TEXT_SIZE = 4096U;

    ui_controller->m_language_index = 0;

    ui_controller->m_show_video_capture_manager = false;
    ui_controller->m_show_asset_motion_manager = false;
    ui_controller->m_show_asset_model_manager = false;
    ui_controller->m_show_asset_image_manager = false;
    ui_controller->m_show_video_detector_manager = false;
    ui_controller->m_show_instance_motion_manager = false;
    ui_controller->m_show_instance_model_manager = false;
    ui_controller->m_show_camera_manager = false;
    ui_controller->m_show_physics_ragdoll_manager = false;
    ui_controller->m_show_window_manager = false;
    ui_controller->m_show_area_lighting_manager = false;
    ui_controller->m_show_environment_lighting_manager = false;
    ui_controller->m_show_global_illumination_manager = false;
    ui_controller->m_show_acknowledgement = false;

    ui_controller->m_new_video_capture_type = VIDEO_CAPTURE_TYPE_INVALID;
    ui_controller->m_new_video_capture_camera_index = 0;
    ui_controller->m_new_video_capture_camera_resolution_index = 0;
    ui_controller->m_new_video_capture_camera_fps_index = 0;
    ui_controller->m_new_video_capture_get_open_file_name_file_type_index = 1;
    assert(ui_controller->m_tree_view_selected_video_capture.empty());

    ui_controller->m_import_asset_motion_get_open_file_name_file_type_index = 1;
    assert(ui_controller->m_tree_view_selected_asset_motion.empty());

    ui_controller->m_import_asset_model_get_open_file_name_file_type_index = 1;
    assert(ui_controller->m_tree_view_selected_asset_model.empty());

    ui_controller->m_import_asset_image_force_srgb = true;
    ui_controller->m_import_asset_image_get_open_file_name_file_type_index = 1;
    assert(ui_controller->m_tree_view_selected_asset_image.empty());

    ui_controller->m_new_video_detector_name.resize(MAX_INPUT_TEXT_SIZE);
    assert(ui_controller->m_new_video_detector_selected_video_capture.empty());
    ui_controller->m_new_video_detector_hand_count = 1U;
    ui_controller->m_new_video_detector_face_count = 1U;
    ui_controller->m_new_video_detector_pose_count = 1U;
    ui_controller->m_new_video_detector_force_gpu = true;
    ui_controller->m_new_video_detector_enable_debug_renderer = false;
    ui_controller->m_tree_view_selected_video_detector = INVALID_TIMESTAMP;

    ui_controller->m_new_instance_motion_name.resize(MAX_INPUT_TEXT_SIZE);
    assert(ui_controller->m_new_instance_motion_selected_asset_motion.empty());
    ui_controller->m_new_instance_motion_selected_animation_index = INVALID_UINT32_INDEX;
    ui_controller->m_tree_view_selected_instance_motion = INVALID_TIMESTAMP;

    ui_controller->m_new_instance_model_name.resize(MAX_INPUT_TEXT_SIZE);
    assert(ui_controller->m_new_instance_model_selected_asset_model.empty());
    ui_controller->m_new_instance_model_selected_surface_group_index = INVALID_UINT32_INDEX;
    ui_controller->m_new_instance_model_animation_input_type = ANIMATION_INPUT_TYPE_INVALID;
    ui_controller->m_new_instance_model_selected_video_detector = INVALID_TIMESTAMP;
    ui_controller->m_new_instance_model_selected_video_detector_face_index = BRX_MOTION_UINT32_INDEX_INVALID;
    ui_controller->m_new_instance_model_selected_video_detector_pose_index = BRX_MOTION_UINT32_INDEX_INVALID;
    ui_controller->m_new_instance_model_selected_instance_motion = INVALID_TIMESTAMP;
    ui_controller->m_new_instance_model_transform_rotation_roll = 0.0F;
    ui_controller->m_new_instance_model_transform_rotation_pitch = 0.0F;
    ui_controller->m_new_instance_model_transform_rotation_yaw = 0.0F;
    ui_controller->m_new_instance_model_transform_translation_x = 0.0F;
    ui_controller->m_new_instance_model_transform_translation_y = 0.0F;
    ui_controller->m_new_instance_model_transform_translation_z = 0.0F;
    ui_controller->m_tree_view_selected_instance_model = INVALID_TIMESTAMP;

    assert(ui_controller->m_instance_controllers.empty());

    ui_controller->m_physics_ragdoll_quality = BRX_MOTION_PHYSICS_RAGDOLL_QUALITY_DISABLED;

    ui_controller->m_new_area_lighting_name.resize(MAX_INPUT_TEXT_SIZE);
    ui_controller->m_new_area_lighting_color_r = 1.0F;
    ui_controller->m_new_area_lighting_color_g = 1.0F;
    ui_controller->m_new_area_lighting_color_b = 1.0F;
    ui_controller->m_new_area_lighting_radiance = 1.0F;
    ui_controller->m_new_area_lighting_position_x = 0.0F;
    ui_controller->m_new_area_lighting_position_y = 1.0F;
    ui_controller->m_new_area_lighting_position_z = -1.0F;
    ui_controller->m_new_area_lighting_edge1_x = -1.0F;
    ui_controller->m_new_area_lighting_edge1_y = 0.0F;
    ui_controller->m_new_area_lighting_edge1_z = 1.0F;
    ui_controller->m_new_area_lighting_edge2_x = 1.0F;
    ui_controller->m_new_area_lighting_edge2_y = 1.0F;
    ui_controller->m_new_area_lighting_edge2_z = 0.0F;
    ui_controller->m_tree_view_selected_area_lighting = INVALID_TIMESTAMP;

    assert(ui_controller->m_hdri_selected_asset_image.empty());
}

extern void user_camera_simulate(float interval_time, brx_anari_device *device, ui_model_t *ui_model, ui_controller_t *ui_controller)
{
    {
        brx_anari_vec3 const camera_position = device->camera_get_position();

        brx_anari_vec3 const camera_direction = device->camera_get_direction();

        brx_anari_vec3 const camera_up = device->camera_get_up();

        DirectX::XMFLOAT3 const eye_position(camera_position.m_x, camera_position.m_y, camera_position.m_z);

        DirectX::XMFLOAT3 const eye_direction(camera_direction.m_x, camera_direction.m_y, camera_direction.m_z);

        DirectX::XMFLOAT3 const up_direction(camera_up.m_x, camera_up.m_y, camera_up.m_z);

        ui_controller->m_first_person_camera.SetEyePt(eye_position);

        ui_controller->m_first_person_camera.SetEyeDir(eye_direction);

        ui_controller->m_first_person_camera.SetUpDir(up_direction);
    }

    ui_controller->m_first_person_camera.FrameMove(interval_time);

    {
        DirectX::XMFLOAT3 eye_position;
        DirectX::XMStoreFloat3(&eye_position, ui_controller->m_first_person_camera.GetEyePt());

        DirectX::XMFLOAT3 eye_direction;
        DirectX::XMStoreFloat3(&eye_direction, ui_controller->m_first_person_camera.GetEyeDir());

        DirectX::XMFLOAT3 up_direction;
        DirectX::XMStoreFloat3(&up_direction, ui_controller->m_first_person_camera.GetUpDir());

        brx_anari_vec3 const camera_position{eye_position.x, eye_position.y, eye_position.z};

        brx_anari_vec3 const camera_direction{eye_direction.x, eye_direction.y, eye_direction.z};

        brx_anari_vec3 const camera_up{up_direction.x, up_direction.y, up_direction.z};

        device->camera_set_position(camera_position);
        device->camera_set_direction(camera_direction);
        device->camera_set_up(camera_up);
    }
}

extern void ui_simulate(brx_anari_device *device, ui_model_t *ui_model, ui_controller_t *ui_controller)
{
    ImGuiIO const &io = ImGui::GetIO();

    constexpr int const LANGUAGE_COUNT = 4;

    constexpr float const ui_x = 32.0F;
    constexpr float const ui_y = 32.0F;

    constexpr float const ui_width = 800.0F;
    constexpr float const ui_height = 600.0F;

    constexpr char const *const help_marker_text = "[ Information ]";

    float const help_marker_position = ImGui::GetFontSize() * 35.0F;

    constexpr float const left_child_width_ratio = 0.5F;

    // constexpr float const table_column_same_line_ratio = 0.35F;

    int ui_index = 1;

    {
        {
            ImGuiViewport const *const main_viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + ui_x * ui_index, main_viewport->WorkPos.y + ui_y * ui_index), ImGuiCond_FirstUseEver);
            ++ui_index;

            ImGui::SetNextWindowSize(ImVec2(ui_width, ui_height), ImGuiCond_FirstUseEver);
        }

        ImGui::Begin("Brioche Puppet (ESC Enable/Disable UI)");

        {
            constexpr char const *const text[LANGUAGE_COUNT] = {
                "Language",
                "言語",
                "語言",
                "语言"};

            ImGui::TextUnformatted(text[ui_controller->m_language_index]);

            ImGui::SameLine();

            constexpr char const *const items[LANGUAGE_COUNT] = {
                "English",
                "日本語",
                "華語",
                "华语"};

            ImGui::Combo("##Language", &ui_controller->m_language_index, items, IM_ARRAYSIZE(items));
        }

        {
            {
                constexpr char const *const text[LANGUAGE_COUNT] = {
                    "Rendering FPS:",
                    "描画 FPS:",
                    "渲染 FPS:",
                    "渲染 FPS:"};
                ImGui::TextUnformatted(text[ui_controller->m_language_index]);
            }

            ImGui::SameLine();

            {
                char fps_text[] = {"18446744073709551615"};
                std::snprintf(fps_text, sizeof(fps_text) / sizeof(fps_text[0]), "%llu", static_cast<long long unsigned>(io.Framerate));
                fps_text[(sizeof(fps_text) / sizeof(fps_text[0])) - 1] = '\0';

                ImGui::TextUnformatted(fps_text);
            }
        }

        {
            if (ImGui::BeginTable("##Show-Manager-Table", 2, ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersInnerV))
            {
                ImGui::TableSetupColumn("##Show-Manager-Table-Property", ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableSetupColumn("##Show-Manager-Table-Value", ImGuiTableColumnFlags_WidthStretch);

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Video Capture Manager",
                        "映像捕捉管理",
                        "視訊捕捉管理",
                        "视频捕捉管理"};
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {
                    ImGui::Checkbox("##Show-Video-Capture-Manager", &ui_controller->m_show_video_capture_manager);
                }

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Asset Motion Manager",
                        "資源行動管理",
                        "資源動作管理",
                        "资源动作管理"};
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {
                    ImGui::Checkbox("##Show-Asset-Motion-Manager", &ui_controller->m_show_asset_motion_manager);
                }

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Asset Model Manager",
                        "資源型式管理",
                        "資源模型管理",
                        "资源模型管理"};
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {
                    ImGui::Checkbox("##Show-Asset-Model-Manager", &ui_controller->m_show_asset_model_manager);
                }

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Asset Image Manager",
                        "資源画像管理",
                        "資源图像管理",
                        "资源圖像管理"};
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {
                    ImGui::Checkbox("##Show-Asset-Image-Manager", &ui_controller->m_show_asset_image_manager);
                }

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Video Detector Manager",
                        "動画検出管理",
                        "視訊檢測管理",
                        "视频检测管理"};
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {
                    ImGui::Checkbox("##Show-Video-Detector-Manager", &ui_controller->m_show_video_detector_manager);
                }

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Instance Motion Manager",
                        "実例行動管理",
                        "實例動作管理",
                        "实例动作管理"};
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {
                    ImGui::Checkbox("##Show-Instance-Motion-Manager", &ui_controller->m_show_instance_motion_manager);
                }

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Instance Model Manager",
                        "実例型式管理",
                        "實例模型管理",
                        "实例模型管理"};
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {
                    ImGui::Checkbox("##Show-Instance-Model-Manager", &ui_controller->m_show_instance_model_manager);
                }

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Camera Manager",
                        "写真機管理",
                        "相機管理",
                        "相机管理"};
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {
                    ImGui::Checkbox("##Show-Camera-Manager", &ui_controller->m_show_camera_manager);
                }

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Physics Ragdoll Manager",
                        "物理布人形管理",
                        "物理布偶管理",
                        "物理布娃娃管理"};
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {
                    ImGui::Checkbox("##Show-Physics-Ragdoll-Manager", &ui_controller->m_show_physics_ragdoll_manager);
                }

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Window Manager",
                        "窓管理",
                        "視窗管理",
                        "窗口管理"};
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {
                    ImGui::Checkbox("##Show-Window-Manager", &ui_controller->m_show_window_manager);
                }

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Area Lighting Manager",
                        "領域照明管理",
                        "區域照明管理",
                        "区域光照管理"};
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {
                    ImGui::Checkbox("##Show-Area-Lighting-Manager", &ui_controller->m_show_area_lighting_manager);
                }

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Environment Lighting Manager",
                        "環境照明管理",
                        "環境照明管理",
                        "环境光照管理"};
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {
                    ImGui::Checkbox("##Show-Environment-Lighting-Manager", &ui_controller->m_show_environment_lighting_manager);
                }

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Global Illumination Manager",
                        "大域照明管理",
                        "全域照明管理",
                        "全局光照管理"};
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {
                    ImGui::Checkbox("##Show-Global-Illumination-Manager", &ui_controller->m_show_global_illumination_manager);
                }

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Acknowledgement",
                        "謝辞",
                        "致謝",
                        "致谢"};
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {
                    ImGui::Checkbox("##Show-Acknowledgement", &ui_controller->m_show_acknowledgement);
                }

                ImGui::EndTable();
            }
        }

        ImGui::End();
    }

    if (ui_controller->m_show_video_capture_manager)
    {
        {
            ImGuiViewport const *const main_viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + ui_x * ui_index, main_viewport->WorkPos.y + ui_y * ui_index), ImGuiCond_FirstUseEver);
            ++ui_index;

            ImGui::SetNextWindowSize(ImVec2(ui_width, ui_height), ImGuiCond_FirstUseEver);
        }

        ImGui::Begin("Video Capture Manager", &ui_controller->m_show_video_capture_manager);

        {
            constexpr char const *const text[LANGUAGE_COUNT] = {
                "Video Capture Manager",
                "映像捕捉管理",
                "視訊捕捉管理",
                "视频捕捉管理"};
            ImGui::TextUnformatted(text[ui_controller->m_language_index]);
        }

        ImGui::Separator();

        {
            constexpr char const *const text[LANGUAGE_COUNT] = {
                "Type",
                "種類",
                "類型",
                "类型"};
            ImGui::TextUnformatted(text[ui_controller->m_language_index]);
        }
        ImGui::SameLine();
        {
            constexpr char const *const items[LANGUAGE_COUNT][VIDEO_CAPTURE_TYPE_COUNT] = {
                {"Camera", "File"},
                {"写真機", "書類"},
                {"相機", "文件"},
                {"相机", "文件"}};

            int selected_type_index = std::min(std::max(VIDEO_CAPTURE_TYPE_MIN, ui_controller->m_new_video_capture_type), VIDEO_CAPTURE_TYPE_MAX);

            ImGui::Combo("##Video-Capture-Manager-New-Video-Capture-Type", &selected_type_index, items[ui_controller->m_language_index], IM_ARRAYSIZE(items[0]));

            ui_controller->m_new_video_capture_type = std::min(std::max(VIDEO_CAPTURE_TYPE_MIN, static_cast<VIDEO_CAPTURE_TYPE>(selected_type_index)), VIDEO_CAPTURE_TYPE_MAX);
        }

        if (VIDEO_CAPTURE_TYPE_CAMERA == ui_controller->m_new_video_capture_type)
        {
            if (ImGui::TreeNodeEx("##Video-Capture-Manager-New-Node", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_NoAutoOpenOnLog))
            {
                if (ImGui::BeginTable("##Video-Capture-Manager-New-Table", 2, ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersInnerV))
                {
                    ImGui::TableSetupColumn("##Video-Capture-Manager-New-Table-Property", ImGuiTableColumnFlags_WidthFixed);
                    ImGui::TableSetupColumn("##Video-Capture-Manager-New-Table-Value", ImGuiTableColumnFlags_WidthStretch);

                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::AlignTextToFramePadding();
                    {
                        constexpr char const *const text[LANGUAGE_COUNT] = {
                            "Camera Index",
                            "写真機索引",
                            "相機索引",
                            "相机索引"};
                        ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                    }
                    ImGui::TableNextColumn();
                    {
                        char camera_index_text[] = {"18446744073709551615"};
                        std::snprintf(&camera_index_text[0], sizeof(camera_index_text) / sizeof(camera_index_text[0]), "%llu", static_cast<long long unsigned>(std::max(0, ui_controller->m_new_video_capture_camera_index)));
                        camera_index_text[(sizeof(camera_index_text) / sizeof(camera_index_text[0]) - 1)] = '\0';

                        ImGui::InputText("##Video-Capture-Manager-New-Video-Capture-Camera-Index", &camera_index_text[0], sizeof(camera_index_text) / sizeof(camera_index_text[0]), ImGuiInputTextFlags_CharsDecimal);

                        ui_controller->m_new_video_capture_camera_index = std::strtoull(camera_index_text, NULL, 10);
                    }

                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::AlignTextToFramePadding();
                    {
                        constexpr char const *const text[LANGUAGE_COUNT] = {
                            "Resolution",
                            "解像度",
                            "解析度",
                            "分辨率"};
                        ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                    }
                    ImGui::TableNextColumn();
                    {
                        constexpr char const *const items[] = {
                            "SD Landscape (640 x 480)",
                            "SD Portrait (480 x 640)",
                            "qHD Landscape (960 x 540)",
                            "qHD Portrait (540 x 960)",
                            "HD Landscape (1280 x 720)",
                            "HD Portrait (720 x 1280)",
                            "HD+ Landscape (1600 x 900)",
                            "HD+ Portrait (900 x 1600)",
                            "FHD Landscape (1920 x 1080)",
                            "FHD Portrait (1080 x 1920)"};

                        int selected_resolution_index = std::min(std::max(0, ui_controller->m_new_video_capture_camera_resolution_index), static_cast<int>(sizeof(items) / sizeof(items[0])) - 1);

                        ImGui::Combo("##Video-Capture-Manager-New-Video-Capture-Camera-Resolution-Index", &selected_resolution_index, items, IM_ARRAYSIZE(items));

                        ui_controller->m_new_video_capture_camera_resolution_index = std::min(std::max(0, selected_resolution_index), static_cast<int>(sizeof(items) / sizeof(items[0])) - 1);
                    }

                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::AlignTextToFramePadding();
                    {
                        {
                            constexpr char const *const text[LANGUAGE_COUNT] = {
                                "Video Capture FPS",
                                "映像捕捉 FPS",
                                "視訊捕捉 FPS",
                                "视频捕捉 FPS"};
                            ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                        }

                        ImGui::SameLine();

                        {
                            ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                            ImGui::TextUnformatted(help_marker_text);
                            ImGui::PopStyleColor();
                            if (ImGui::BeginItemTooltip())
                            {
                                ImGui::PushTextWrapPos(help_marker_position);
                                constexpr char const *const text[LANGUAGE_COUNT] = {
                                    "It is NOT possible for the rendering FPS to be higher than the video capture FPS. But too high video capture FPS may not necessarily benefit rendering FPS.",
                                    "描画 FPS は映像捕捉 FPS 超過不可。但し、映像捕捉 FPS 過高時、描画 FPS 利点必ずしも生じず。",
                                    "渲染 FPS 不可能高於視訊捕捉 FPS。但過高的視訊捕捉 FPS 不一定能有利於渲染 FPS。",
                                    "渲染 FPS 不可能高于视频捕捉 FPS。但过高的视频捕捉 FPS 并不一定能有利于渲染 FPS。"};
                                ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                                ImGui::PopTextWrapPos();
                                ImGui::EndTooltip();
                            }
                        }
                    }
                    ImGui::TableNextColumn();
                    {
                        constexpr char const *const items[] = {
                            "24",
                            "25",
                            "30",
                            "50",
                            "60",
                            "100",
                            "120"};

                        int selected_fps_index = std::min(std::max(0, ui_controller->m_new_video_capture_camera_fps_index), static_cast<int>(sizeof(items) / sizeof(items[0])) - 1);

                        ImGui::Combo("##Video-Capture-Manager-New-Video-Capture-Camera-FPS-Index", &selected_fps_index, items, IM_ARRAYSIZE(items));

                        ui_controller->m_new_video_capture_camera_fps_index = std::min(std::max(0, selected_fps_index), static_cast<int>(sizeof(items) / sizeof(items[0])) - 1);
                    }

                    ImGui::EndTable();
                }

                ImGui::TreePop();
            }

            {
                constexpr char const *const text[LANGUAGE_COUNT] = {
                    "New",
                    "新規作成",
                    "新建",
                    "新建"};
                ImGui::TextUnformatted(text[ui_controller->m_language_index]);
            }

            ImGui::SameLine();

            if (ImGui::Button("N##Video-Capture-Manager-New-Button"))
            {
                mcrt_string video_capture_url;
                {
                    char camera_index_text[] = {"18446744073709551615"};
                    std::snprintf(camera_index_text, sizeof(camera_index_text) / sizeof(camera_index_text[0]), "%llu", static_cast<long long unsigned>(ui_controller->m_new_video_capture_camera_index));
                    camera_index_text[(sizeof(camera_index_text) / sizeof(camera_index_text[0])) - 1] = '\0';

                    video_capture_url = "camera://";
                    video_capture_url += camera_index_text;
                }

                mcrt_string video_capture_identity;
                {
                    video_capture_identity = "00000000000000000000";
                    video_capture_identity += ' ';
                    video_capture_identity += video_capture_url;
                }

                auto const found_video_capture = ui_model->m_video_captures.find(video_capture_identity);
                if (ui_model->m_video_captures.end() == found_video_capture)
                {
                    brx_motion_video_capture *const video_capture = brx_motion_create_video_capture(video_capture_url.c_str());
                    if (NULL != video_capture)
                    {
                        constexpr uint32_t const resolutions[][2] = {
                            {640, 480},
                            {480, 640},
                            {960, 540},
                            {540, 960},
                            {1280, 720},
                            {720, 1280},
                            {1600, 900},
                            {900, 1600},
                            {1920, 1080},
                            {1080, 1920}};

                        int const resolution_index = std::min(std::max(0, ui_controller->m_new_video_capture_camera_resolution_index), static_cast<int>(sizeof(resolutions) / sizeof(resolutions[0]) - 1U));

                        video_capture->set_resolution(resolutions[resolution_index][0], resolutions[resolution_index][1]);

                        constexpr uint32_t const FPSs[] = {
                            24,
                            25,
                            30,
                            50,
                            60,
                            100,
                            120};

                        int const fps_index = std::min(std::max(0, ui_controller->m_new_video_capture_camera_fps_index), static_cast<int>(sizeof(FPSs) / sizeof(FPSs[0]) - 1U));

                        video_capture->set_fps(FPSs[fps_index]);

                        ui_model->m_video_captures.insert(found_video_capture, std::pair<mcrt_string, ui_video_capture_model_t>{video_capture_identity, ui_video_capture_model_t{video_capture}});
                    }
                }
            }
        }
        else if (VIDEO_CAPTURE_TYPE_FILE == ui_controller->m_new_video_capture_type)
        {
            {
                constexpr char const *const text[LANGUAGE_COUNT] = {
                    "Import",
                    "導入",
                    "導入",
                    "导入"};
                ImGui::TextUnformatted(text[ui_controller->m_language_index]);
            }

            ImGui::SameLine();

            if (ImGui::Button("O##Video-Capture-Manager-Import-Button"))
            {
                bool video_capture_file_open;
                mcrt_string video_capture_file_name;
                uint64_t video_capture_file_timestamp;
                {
                    constexpr size_t const video_capture_filter_count = 2;

                    constexpr char const *video_capture_filter_names[video_capture_filter_count] = {
                        "All Files",
                        "Video"};

                    // https://developer.mozilla.org/en-US/docs/Web/Media/Guides/Formats/Video_codecs

                    constexpr char const *video_capture_filter_specs[video_capture_filter_count] = {
                        "*",
                        "*.mp4;*.mkv;*.webm;*.avi"};

                    video_capture_file_open = internal_platform_get_open_file_name(video_capture_filter_count, video_capture_filter_names, video_capture_filter_specs, ui_controller->m_new_video_capture_get_open_file_name_file_type_index, &video_capture_file_name, &video_capture_file_timestamp, NULL);
                }

                if (video_capture_file_open)
                {
                    mcrt_string video_capture_url;
                    {
                        video_capture_url = "file://";
                        video_capture_url += video_capture_file_name;
                    }

                    mcrt_string video_capture_identity;
                    {
                        char video_capture_file_timestamp_text[] = {"18446744073709551615"};
                        std::snprintf(video_capture_file_timestamp_text, sizeof(video_capture_file_timestamp_text) / sizeof(video_capture_file_timestamp_text[0]), "%020llu", static_cast<long long unsigned>(video_capture_file_timestamp));
                        video_capture_file_timestamp_text[(sizeof(video_capture_file_timestamp_text) / sizeof(video_capture_file_timestamp_text[0])) - 1] = '\0';

                        video_capture_identity += video_capture_file_timestamp_text;
                        video_capture_identity += ' ';
                        video_capture_identity += video_capture_url;
                    }

                    auto const found_video_capture = ui_model->m_video_captures.find(video_capture_identity);
                    if (ui_model->m_video_captures.end() == found_video_capture)
                    {
                        brx_motion_video_capture *const video_capture = brx_motion_create_video_capture(video_capture_url.c_str());
                        if (NULL != video_capture)
                        {
                            ui_model->m_video_captures.insert(found_video_capture, std::pair<mcrt_string, ui_video_capture_model_t>{video_capture_identity, ui_video_capture_model_t{video_capture}});
                        }
                    }
                }
            }
        }
        else
        {
            assert(false);
        }

        ImGui::Separator();

        {
            {
                constexpr char const *const text[LANGUAGE_COUNT] = {
                    "Delete",
                    "削除",
                    "刪除",
                    "删除"};
                ImGui::TextUnformatted(text[ui_controller->m_language_index]);
            }

            ImGui::SameLine();

            if (ImGui::Button("X##Video-Capture-Manager-Delete-Button"))
            {
                auto const found_video_capture = ui_model->m_video_captures.find(ui_controller->m_tree_view_selected_video_capture);
                if (ui_model->m_video_captures.end() != found_video_capture)
                {
                    if (NULL != found_video_capture->second.m_video_capture)
                    {
                        brx_motion_destroy_video_capture(found_video_capture->second.m_video_capture);
                    }

                    else
                    {
                        assert(false);
                    }

                    ui_model->m_video_captures.erase(found_video_capture);
                    ui_controller->m_tree_view_selected_video_capture.clear();
                }
            }
        }

        ImGui::Separator();

        if (ImGui::BeginChild("##Video-Capture-Manager-Left-Child", ImVec2(ui_width * left_child_width_ratio, 0.0F), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX))
        {
            ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::PushItemFlag(ImGuiItemFlags_NoNavDefaultFocus, true);
            ImGuiTextFilter text_filter;
            {
                constexpr char const *const hint[LANGUAGE_COUNT] = {
                    "Search",
                    "検索",
                    "檢索",
                    "检索"};
                if (ImGui::InputTextWithHint("##Video-Capture-Manager-Left-Child-Text-Filter", hint[ui_controller->m_language_index], text_filter.InputBuf, IM_ARRAYSIZE(text_filter.InputBuf), ImGuiInputTextFlags_EscapeClearsAll))
                {
                    text_filter.Build();
                }
            }

            ImGui::PopItemFlag();

            if (ImGui::BeginTable("##Video-Capture-Manager-Left-Child-Table", 1, ImGuiTableFlags_RowBg))
            {
                for (auto const &video_capture : ui_model->m_video_captures)
                {
                    if (text_filter.PassFilter(video_capture.first.c_str()))
                    {
                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();

                        ImGuiTreeNodeFlags const flags = ((ui_controller->m_tree_view_selected_video_capture != video_capture.first)) ? ImGuiTreeNodeFlags_Leaf : (ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Selected);

                        bool const node_open = ImGui::TreeNodeEx(video_capture.first.c_str(), flags);

                        if (ImGui::IsItemFocused())
                        {
                            ui_controller->m_tree_view_selected_video_capture = video_capture.first;
                        }

                        if (node_open)
                        {
                            ImGui::TreePop();
                        }
                    }
                }

                ImGui::EndTable();
            }
        }
        ImGui::EndChild();

        auto const found_video_capture = ui_model->m_video_captures.find(ui_controller->m_tree_view_selected_video_capture);

        if (ui_model->m_video_captures.end() != found_video_capture)
        {
            ImGui::SameLine();

            ImGui::BeginGroup();

            if (BRX_MOTION_VIDEO_CAPTURE_TYPE_CAMERA == found_video_capture->second.m_video_capture->get_type())
            {
                mcrt_string camera_index_text;
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Camera Index: ",
                        "写真機索引: ",
                        "相機索引: ",
                        "相机索引: "};
                    camera_index_text = text[ui_controller->m_language_index];
                    camera_index_text += (found_video_capture->first.c_str() + 30);
                }

                ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                ImGui::TextUnformatted(camera_index_text.c_str());
                ImGui::PopStyleColor();
            }
            else if (BRX_MOTION_VIDEO_CAPTURE_TYPE_FILE == found_video_capture->second.m_video_capture->get_type())
            {
                mcrt_string timestamp_text = "Timestamp: ";
                mcrt_string directory_name;
                mcrt_string file_name;
                {
                    size_t const timestamp_text_end_pos = found_video_capture->first.find(' ');
                    size_t const directory_name_end_pos = found_video_capture->first.find_last_of("/\\");
                    if ((mcrt_string::npos != timestamp_text_end_pos) && (mcrt_string::npos != directory_name_end_pos) && ((timestamp_text_end_pos + 7U) < directory_name_end_pos))
                    {
                        timestamp_text += found_video_capture->first.substr(0U, timestamp_text_end_pos);
                        directory_name = found_video_capture->first.substr((timestamp_text_end_pos + 7U) + 1U, ((directory_name_end_pos - (timestamp_text_end_pos + 7U)) - 1U));
                        file_name = found_video_capture->first.substr(directory_name_end_pos + 1U);
                    }
                    else
                    {
                        assert(false);
                        timestamp_text = "N/A";
                        directory_name = "N/A";
                        file_name = "N/A";
                    }
                }

                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                    ImGui::TextUnformatted(file_name.c_str());
                    ImGui::PopStyleColor();
                }

                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                    ImGui::TextUnformatted(directory_name.c_str());
                    ImGui::PopStyleColor();
                }

                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                    ImGui::TextUnformatted(timestamp_text.c_str());
                    ImGui::PopStyleColor();
                }
            }
            else
            {
                assert(false);
            }

            ImGui::Separator();

            if (ImGui::BeginTable("##Video-Capture-Manager-Right-Group-Table", 2, ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersInnerV))
            {
                ImGui::TableSetupColumn("##Video-Capture-Manager-Right-Group-Table-Property", ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableSetupColumn("##Video-Capture-Manager-Right-Group-Table-Value", ImGuiTableColumnFlags_WidthStretch);

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Backend:",
                        "後端:",
                        "後端:",
                        "后端:"};
                    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                    ImGui::PopStyleColor();
                }
                ImGui::TableNextColumn();
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                    ImGui::TextUnformatted(found_video_capture->second.m_video_capture->get_backend_name());
                    ImGui::PopStyleColor();
                }

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Resolution",
                        "解像度",
                        "解析度",
                        "分辨率"};
                    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                    ImGui::PopStyleColor();
                }
                ImGui::TableNextColumn();
                {
                    mcrt_string resolution_text;
                    {

                        char width_text[] = {"18446744073709551615"};
                        std::snprintf(width_text, sizeof(width_text) / sizeof(width_text[0]), "%llu", static_cast<long long unsigned>(found_video_capture->second.m_video_capture->get_width()));
                        width_text[(sizeof(width_text) / sizeof(width_text[0])) - 1] = '\0';

                        char height_text[] = {"18446744073709551615"};
                        std::snprintf(height_text, sizeof(height_text) / sizeof(height_text[0]), "%llu", static_cast<long long unsigned>(found_video_capture->second.m_video_capture->get_height()));
                        height_text[(sizeof(height_text) / sizeof(height_text[0])) - 1] = '\0';

                        resolution_text = width_text;
                        resolution_text += " X ";
                        resolution_text += height_text;
                    }

                    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                    ImGui::TextUnformatted(resolution_text.c_str());
                    ImGui::PopStyleColor();
                }

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    {
                        constexpr char const *const text[LANGUAGE_COUNT] = {
                            "Video Capture FPS",
                            "映像捕捉 FPS",
                            "視訊捕捉 FPS",
                            "视频捕捉 FPS"};
                        ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                        ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                        ImGui::PopStyleColor();
                    }

                    ImGui::SameLine();

                    {
                        ImGui::TextUnformatted(help_marker_text);
                        if (ImGui::BeginItemTooltip())
                        {
                            ImGui::PushTextWrapPos(help_marker_position);
                            constexpr char const *const text[LANGUAGE_COUNT] = {
                                "It is NOT possible for the rendering FPS to be higher than the video capture FPS. But too high video capture FPS may not necessarily benefit rendering FPS.",
                                "描画 FPS は映像捕捉 FPS 超過不可。但し、映像捕捉 FPS 過高時、描画 FPS 利点必ずしも生じず。",
                                "渲染 FPS 不可能高於視訊捕捉 FPS。但過高的視訊捕捉 FPS 不一定能有利於渲染 FPS。",
                                "渲染 FPS 不可能高于视频捕捉 FPS。但过高的视频捕捉 FPS 并不一定能有利于渲染 FPS。"};
                            ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                            ImGui::PopTextWrapPos();
                            ImGui::EndTooltip();
                        }
                    }
                }
                ImGui::TableNextColumn();
                {
                    char fps_text[] = {"18446744073709551615"};
                    std::snprintf(fps_text, sizeof(fps_text) / sizeof(fps_text[0]), "%llu", static_cast<long long unsigned>(found_video_capture->second.m_video_capture->get_fps()));
                    fps_text[(sizeof(fps_text) / sizeof(fps_text[0])) - 1] = '\0';

                    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                    ImGui::TextUnformatted(fps_text);
                    ImGui::PopStyleColor();
                }

                ImGui::EndTable();
            }

            ImGui::EndGroup();
        }

        ImGui::End();
    }

    if (ui_controller->m_show_asset_motion_manager)
    {
        {
            ImGuiViewport const *const main_viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + ui_x * ui_index, main_viewport->WorkPos.y + ui_y * ui_index), ImGuiCond_FirstUseEver);
            ++ui_index;

            ImGui::SetNextWindowSize(ImVec2(ui_width, ui_height), ImGuiCond_FirstUseEver);
        }

        ImGui::Begin("Asset Motion Manager", &ui_controller->m_show_asset_motion_manager);

        {
            constexpr char const *const text[LANGUAGE_COUNT] = {
                "Asset Motion Manager",
                "資源行動管理",
                "資源動作管理",
                "资源动作管理"};
            ImGui::TextUnformatted(text[ui_controller->m_language_index]);
        }

        ImGui::Separator();

        {
            constexpr char const *const text[LANGUAGE_COUNT] = {
                "Import",
                "導入",
                "導入",
                "导入"};
            ImGui::TextUnformatted(text[ui_controller->m_language_index]);

            ImGui::SameLine();

            if (ImGui::Button("O##Asset-Motion-Manager-Import"))
            {
                bool asset_motion_file_open;
                mcrt_string asset_motion_file_name;
                uint64_t asset_motion_file_timestamp;
                mcrt_vector<uint8_t> asset_motion_file_data;
                {
                    constexpr size_t const asset_motion_filter_count = 2;

                    // TODO
                    // "glTF Binary"    "*.glb;*.vrma"
                    // "glTF Separate"  "*.gltf"

                    constexpr char const *const asset_motion_filter_names[asset_motion_filter_count] = {
                        "All Files",
                        "MMD Motion Data"};

                    constexpr char const *const asset_motion_filter_specs[asset_motion_filter_count] = {
                        "*",
                        "*.vmd"};

                    asset_motion_file_open = internal_platform_get_open_file_name(asset_motion_filter_count, asset_motion_filter_names, asset_motion_filter_specs, ui_controller->m_import_asset_motion_get_open_file_name_file_type_index, &asset_motion_file_name, &asset_motion_file_timestamp, &asset_motion_file_data);
                }

                if (asset_motion_file_open)
                {
                    assert(!asset_motion_file_name.empty());
                    assert(0U != asset_motion_file_timestamp);
                    assert(!asset_motion_file_data.empty());

                    mcrt_string asset_motion_file_identity;
                    {
                        char asset_motion_file_timestamp_text[] = {"18446744073709551615"};
                        std::snprintf(asset_motion_file_timestamp_text, sizeof(asset_motion_file_timestamp_text) / sizeof(asset_motion_file_timestamp_text[0]), "%020llu", static_cast<long long unsigned>(asset_motion_file_timestamp));
                        asset_motion_file_timestamp_text[(sizeof(asset_motion_file_timestamp_text) / sizeof(asset_motion_file_timestamp_text[0])) - 1] = '\0';

                        asset_motion_file_identity += asset_motion_file_timestamp_text;
                        asset_motion_file_identity += ' ';
                        asset_motion_file_identity += asset_motion_file_name;
                    }

                    auto const found_asset_motion = ui_model->m_asset_motions.find(asset_motion_file_identity);

                    if (ui_model->m_asset_motions.end() == found_asset_motion)
                    {
                        brx_asset_import_scene *const asset_import_scene = brx_asset_import_create_scene_from_memory(asset_motion_file_data.data(), asset_motion_file_data.size());
                        if (NULL != asset_import_scene)
                        {
                            bool success = true;
                            uint32_t const animation_count = asset_import_scene->get_animation_count();
                            mcrt_vector<brx_motion_animation *> animations(static_cast<size_t>(animation_count), NULL);
                            for (uint32_t animation_index = 0U; animation_index < animation_count; ++animation_index)
                            {
                                brx_asset_import_animation const *const asset_import_animation = asset_import_scene->get_animation(animation_index);

                                brx_motion_animation *const motion_animation = brx_motion_create_animation(asset_import_animation->get_frame_count(), asset_import_animation->get_weight_channel_count(), wrap(asset_import_animation->get_weight_channel_names()), asset_import_animation->get_weights(), asset_import_animation->get_rigid_transform_channel_count(), wrap(asset_import_animation->get_rigid_transform_channel_names()), wrap(asset_import_animation->get_rigid_transforms()), asset_import_animation->get_switch_channel_count(), wrap(asset_import_animation->get_switch_channel_names()), asset_import_animation->get_switches());

                                if (NULL != motion_animation)
                                {
                                    animations[animation_index] = motion_animation;
                                }
                                else
                                {
                                    success = false;
                                    break;
                                }
                            }

                            if (success)
                            {
                                for (brx_motion_animation *const animation : animations)
                                {
                                    assert(NULL != animation);
                                }

                                ui_model->m_asset_motions.insert(found_asset_motion, std::pair<mcrt_string, ui_asset_motion_model_t>{std::move(asset_motion_file_identity), ui_asset_motion_model_t{std::move(animations)}});
                            }
                            else
                            {
                                assert(false);

                                for (brx_motion_animation *const animation : animations)
                                {
                                    if (NULL != animation)
                                    {
                                        brx_motion_destroy_animation(animation);
                                    }
                                }

                                animations.clear();
                            }

                            brx_asset_import_destroy_scene(asset_import_scene);
                        }
                        else
                        {
                            assert(false);
                        }
                    }
                    else
                    {
                        assert(!found_asset_motion->second.m_animations.empty());
                    }
                }
            }
        }

        ImGui::Separator();

        {
            constexpr char const *const text[LANGUAGE_COUNT] = {
                "Delete",
                "削除",
                "刪除",
                "删除"};
            ImGui::TextUnformatted(text[ui_controller->m_language_index]);

            ImGui::SameLine();

            if (ImGui::Button("X##Asset-Motion-Manager-Delete"))
            {
                auto const found_asset_motion = ui_model->m_asset_motions.find(ui_controller->m_tree_view_selected_asset_motion);
                if (ui_model->m_asset_motions.end() != found_asset_motion)
                {
                    assert(!found_asset_motion->second.m_animations.empty());

                    for (brx_motion_animation *const animation : found_asset_motion->second.m_animations)
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
                    found_asset_motion->second.m_animations.clear();

                    ui_model->m_asset_motions.erase(found_asset_motion);
                    ui_controller->m_tree_view_selected_asset_motion.clear();
                }
            }
        }

        ImGui::Separator();

        if (ImGui::BeginChild("##Asset-Motion-Manager-Left-Child", ImVec2(ui_width * left_child_width_ratio, 0.0F), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX))
        {
            ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::PushItemFlag(ImGuiItemFlags_NoNavDefaultFocus, true);
            ImGuiTextFilter text_filter;
            {
                constexpr char const *const hint[LANGUAGE_COUNT] = {
                    "Search",
                    "検索",
                    "檢索",
                    "检索"};
                if (ImGui::InputTextWithHint("##Asset-Motion-Manager-Left-Child-Text-Filter", hint[ui_controller->m_language_index], text_filter.InputBuf, IM_ARRAYSIZE(text_filter.InputBuf), ImGuiInputTextFlags_EscapeClearsAll))
                {
                    text_filter.Build();
                }
            }

            ImGui::PopItemFlag();

            if (ImGui::BeginTable("##Asset-Motion-Manager-Left-Child-Table", 1, ImGuiTableFlags_RowBg))
            {
                for (auto const &asset_motion : ui_model->m_asset_motions)
                {
                    if (text_filter.PassFilter(asset_motion.first.c_str()))
                    {
                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();

                        ImGuiTreeNodeFlags const flags = ((ui_controller->m_tree_view_selected_asset_motion != asset_motion.first)) ? ImGuiTreeNodeFlags_Leaf : (ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Selected);

                        bool const node_open = ImGui::TreeNodeEx(asset_motion.first.c_str(), flags);

                        if (ImGui::IsItemFocused())
                        {
                            ui_controller->m_tree_view_selected_asset_motion = asset_motion.first;
                        }

                        if (node_open)
                        {
                            ImGui::TreePop();
                        }
                    }
                }

                ImGui::EndTable();
            }
        }
        ImGui::EndChild();

        auto const found_asset_motion = ui_model->m_asset_motions.find(ui_controller->m_tree_view_selected_asset_motion);

        if (ui_model->m_asset_motions.end() != found_asset_motion)
        {
            ImGui::SameLine();

            ImGui::BeginGroup();

            mcrt_string timestamp_text = "Timestamp: ";
            mcrt_string directory_name;
            mcrt_string file_name;
            {
                size_t const timestamp_text_end_pos = found_asset_motion->first.find(' ');
                size_t const directory_name_end_pos = found_asset_motion->first.find_last_of("/\\");
                if ((mcrt_string::npos != timestamp_text_end_pos) && (mcrt_string::npos != directory_name_end_pos) && (timestamp_text_end_pos < directory_name_end_pos))
                {
                    timestamp_text += found_asset_motion->first.substr(0U, timestamp_text_end_pos);
                    directory_name = found_asset_motion->first.substr(timestamp_text_end_pos + 1U, ((directory_name_end_pos - timestamp_text_end_pos) - 1U));
                    file_name = found_asset_motion->first.substr(directory_name_end_pos + 1U);
                }
                else
                {
                    assert(false);
                    timestamp_text = "N/A";
                    directory_name = "N/A";
                    file_name = "N/A";
                }
            }

            {
                ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                ImGui::TextUnformatted(file_name.c_str());
                ImGui::PopStyleColor();
            }

            {
                ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                ImGui::TextUnformatted(directory_name.c_str());
                ImGui::PopStyleColor();
            }

            {
                ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                ImGui::TextUnformatted(timestamp_text.c_str());
                ImGui::PopStyleColor();
            }

            ImGui::Separator();

            if (ImGui::BeginTable("##Asset-Motion-Manager-Right-Group-Table", 2, ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersInnerV))
            {
                ImGui::TableSetupColumn("##Asset-Motion-Manager-Right-Group-Table-Property", ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableSetupColumn("##Asset-Motion-Manager-Right-Group-Table-Value", ImGuiTableColumnFlags_WidthStretch);

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Animation Count",
                        "動作数",
                        "動畫數",
                        "动画数"};
                    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                    ImGui::PopStyleColor();
                }
                ImGui::TableNextColumn();
                {
                    assert(!found_asset_motion->second.m_animations.empty());
                    uint32_t const animation_count = static_cast<uint32_t>(found_asset_motion->second.m_animations.size());

                    char animation_count_text[] = {"18446744073709551615"};
                    std::snprintf(animation_count_text, sizeof(animation_count_text) / sizeof(animation_count_text[0]), "%llu", static_cast<long long unsigned>(animation_count));
                    animation_count_text[(sizeof(animation_count_text) / sizeof(animation_count_text[0])) - 1] = '\0';

                    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                    ImGui::TextUnformatted(animation_count_text);
                    ImGui::PopStyleColor();
                }

                ImGui::EndTable();
            }

            ImGui::EndGroup();
        }

        ImGui::End();
    }

    if (ui_controller->m_show_asset_model_manager)
    {
        {
            ImGuiViewport const *const main_viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + ui_x * ui_index, main_viewport->WorkPos.y + ui_y * ui_index), ImGuiCond_FirstUseEver);
            ++ui_index;

            ImGui::SetNextWindowSize(ImVec2(ui_width, ui_height), ImGuiCond_FirstUseEver);
        }

        ImGui::Begin("Asset Model Manager", &ui_controller->m_show_asset_model_manager);

        {
            constexpr char const *const text[LANGUAGE_COUNT] = {
                "Asset Model Manager",
                "資源型式管理",
                "資源模型管理",
                "资源模型管理"};
            ImGui::TextUnformatted(text[ui_controller->m_language_index]);
        }

        ImGui::Separator();

        {
            constexpr char const *const text[LANGUAGE_COUNT] = {
                "Import",
                "導入",
                "導入",
                "导入"};
            ImGui::TextUnformatted(text[ui_controller->m_language_index]);

            ImGui::SameLine();

            if (ImGui::Button("O##Asset-Model-Manager-Import"))
            {
                bool asset_model_file_open;
                mcrt_string asset_model_file_name;
                uint64_t asset_model_file_timestamp;
                mcrt_vector<uint8_t> asset_model_file_data;
                {
                    constexpr size_t const asset_model_filter_count = 3;

                    constexpr char const *const asset_model_filter_names[asset_model_filter_count] = {
                        "All Files",
                        "MMD Model Data",
                        "VRM Model Data"};

                    constexpr char const *const asset_model_filter_specs[asset_model_filter_count] = {
                        "*",
                        "*.pmx",
                        "*.vrm;*.glb"};

                    asset_model_file_open = internal_platform_get_open_file_name(asset_model_filter_count, asset_model_filter_names, asset_model_filter_specs, ui_controller->m_import_asset_model_get_open_file_name_file_type_index, &asset_model_file_name, &asset_model_file_timestamp, &asset_model_file_data);
                }

                if (asset_model_file_open)
                {
                    assert(!asset_model_file_name.empty());
                    assert(0U != asset_model_file_timestamp);
                    assert(!asset_model_file_data.empty());

                    mcrt_string asset_model_file_identity;
                    {
                        char asset_model_file_timestamp_text[] = {"18446744073709551615"};
                        std::snprintf(asset_model_file_timestamp_text, sizeof(asset_model_file_timestamp_text) / sizeof(asset_model_file_timestamp_text[0]), "%020llu", static_cast<long long unsigned>(asset_model_file_timestamp));
                        asset_model_file_timestamp_text[(sizeof(asset_model_file_timestamp_text) / sizeof(asset_model_file_timestamp_text[0])) - 1] = '\0';

                        asset_model_file_identity += asset_model_file_timestamp_text;
                        asset_model_file_identity += ' ';
                        asset_model_file_identity += asset_model_file_name;
                    }

                    auto const found_asset_model = ui_model->m_asset_models.find(asset_model_file_identity);

                    if (ui_model->m_asset_models.end() == found_asset_model)
                    {
                        brx_asset_import_scene *const asset_import_scene = brx_asset_import_create_scene_from_memory(asset_model_file_data.data(), asset_model_file_data.size());
                        if (NULL != asset_import_scene)
                        {
                            mcrt_string asset_model_directory_name;
                            {
                                size_t directory_name_end_pos = asset_model_file_name.find_last_of("/\\");
                                if (mcrt_string::npos != directory_name_end_pos)
                                {
                                    asset_model_directory_name = asset_model_file_name.substr(0U, directory_name_end_pos);
                                }
                                else
                                {
                                    asset_model_directory_name = ".";
                                }
                            }

                            bool success = true;
                            uint32_t const surface_group_count = asset_import_scene->get_surface_group_count();
                            mcrt_vector<brx_anari_surface_group *> surface_groups(static_cast<size_t>(surface_group_count), NULL);
                            mcrt_vector<brx_motion_skeleton *> skeletons(static_cast<size_t>(surface_group_count), NULL);
                            for (uint32_t surface_group_index = 0U; surface_group_index < surface_group_count; ++surface_group_index)
                            {
                                brx_asset_import_surface_group const *const asset_import_surface_group = asset_import_scene->get_surface_group(surface_group_index);

                                uint32_t const surface_count = asset_import_surface_group->get_surface_count();

                                uint32_t const animation_skeleton_joint_count = asset_import_surface_group->get_animation_skeleton_joint_count();

                                bool const skin = (animation_skeleton_joint_count > 0U);

                                brx_anari_surface_group *anari_surface_group = NULL;
                                {
                                    mcrt_vector<BRX_ANARI_SURFACE> surfaces(static_cast<size_t>(surface_count));

                                    for (uint32_t surface_index = 0U; surface_index < surface_count; ++surface_index)
                                    {
                                        brx_asset_import_surface const *const asset_import_surface = asset_import_surface_group->get_surface(surface_index);

                                        surfaces[surface_index].m_vertex_count = asset_import_surface->get_vertex_count();
                                        surfaces[surface_index].m_vertex_positions = wrap(asset_import_surface->get_vertex_positions());
                                        surfaces[surface_index].m_vertex_varyings = wrap(asset_import_surface->get_vertex_varyings());
                                        surfaces[surface_index].m_vertex_blendings = wrap(asset_import_surface->get_vertex_blendings());
                                        assert((NULL != surfaces[surface_index].m_vertex_blendings) == skin);
                                        for (uint32_t morph_target_name_index = 0U; morph_target_name_index < BRX_ANARI_MORPH_TARGET_NAME_MMD_COUNT; ++morph_target_name_index)
                                        {
                                            surfaces[surface_index].m_morph_targets_vertex_positions[morph_target_name_index] = NULL;
                                            surfaces[surface_index].m_morph_targets_vertex_varyings[morph_target_name_index] = NULL;
                                        }
                                        for (uint32_t morph_target_index = 0U; morph_target_index < asset_import_surface->get_morph_target_count(); ++morph_target_index)
                                        {
                                            BRX_ANARI_MORPH_TARGET_NAME const morph_target_name = wrap(asset_import_surface->get_morph_target_name(morph_target_index));
                                            uint32_t const morph_target_name_index = morph_target_name;
                                            surfaces[surface_index].m_morph_targets_vertex_positions[morph_target_name_index] = wrap(asset_import_surface->get_morph_target_vertex_positions(morph_target_index));
                                            surfaces[surface_index].m_morph_targets_vertex_varyings[morph_target_name_index] = wrap(asset_import_surface->get_morph_target_vertex_varyings(morph_target_index));
                                        }
                                        surfaces[surface_index].m_index_count = asset_import_surface->get_index_count();
                                        surfaces[surface_index].m_indices = asset_import_surface->get_indices();
                                        surfaces[surface_index].m_is_double_sided = asset_import_surface->get_is_double_sided();
                                        surfaces[surface_index].m_emissive_image = internal_load_asset_image(asset_import_surface->get_emissive_image_url_data(), asset_import_surface->get_emissive_image_url_size(), asset_model_directory_name.c_str(), false, device, ui_model);
                                        surfaces[surface_index].m_emissive_factor = wrap(asset_import_surface->get_emissive_factor());
                                        surfaces[surface_index].m_normal_image = internal_load_asset_image(asset_import_surface->get_normal_image_url_data(), asset_import_surface->get_normal_image_url_size(), asset_model_directory_name.c_str(), false, device, ui_model);
                                        surfaces[surface_index].m_normal_scale = asset_import_surface->get_normal_scale();
                                        surfaces[surface_index].m_base_color_image = internal_load_asset_image(asset_import_surface->get_base_color_image_url_data(), asset_import_surface->get_base_color_image_url_size(), asset_model_directory_name.c_str(), true, device, ui_model);
                                        surfaces[surface_index].m_base_color_factor = wrap(asset_import_surface->get_base_color_factor());
                                        surfaces[surface_index].m_metallic_roughness_image = internal_load_asset_image(asset_import_surface->get_metallic_roughness_image_url_data(), asset_import_surface->get_metallic_roughness_image_url_size(), asset_model_directory_name.c_str(), false, device, ui_model);
                                        surfaces[surface_index].m_metallic_factor = asset_import_surface->get_metallic_factor();
                                        surfaces[surface_index].m_roughness_factor = asset_import_surface->get_roughness_factor();
                                    }

                                    assert(NULL == anari_surface_group);
                                    anari_surface_group = device->new_surface_group(surface_count, surfaces.data());
                                }

                                if (skin)
                                {
                                    brx_motion_skeleton *const motion_skeleton = brx_motion_create_skeleton(animation_skeleton_joint_count, wrap(asset_import_surface_group->get_animation_skeleton_joint_names()), asset_import_surface_group->get_animation_skeleton_joint_parent_indices(), wrap(asset_import_surface_group->get_animation_skeleton_joint_transforms_bind_pose_local_space()), asset_import_surface_group->get_animation_skeleton_joint_constraint_count(), wrap(asset_import_surface_group->get_animation_skeleton_joint_constraint_names()), wrap(asset_import_surface_group->get_animation_skeleton_joint_constraints()), asset_import_surface_group->get_ragdoll_skeleton_rigid_body_count(), wrap(asset_import_surface_group->get_ragdoll_skeleton_rigid_bodies()), asset_import_surface_group->get_ragdoll_skeleton_constraint_count(), wrap(asset_import_surface_group->get_ragdoll_skeleton_constraints()), asset_import_surface_group->get_animation_to_ragdoll_direct_mapping_count(), wrap(asset_import_surface_group->get_animation_to_ragdoll_direct_mappings()), asset_import_surface_group->get_ragdoll_to_animation_direct_mapping_count(), wrap(asset_import_surface_group->get_ragdoll_to_animation_direct_mappings()));

                                    if ((NULL != anari_surface_group) && (NULL != motion_skeleton))
                                    {
                                        surface_groups[surface_group_index] = anari_surface_group;
                                        skeletons[surface_group_index] = motion_skeleton;
                                    }
                                    else
                                    {
                                        success = false;
                                        break;
                                    }
                                }
                                else
                                {
                                    if (NULL != anari_surface_group)
                                    {
                                        surface_groups[surface_group_index] = anari_surface_group;
                                        skeletons[surface_group_index] = NULL;
                                    }
                                    else
                                    {
                                        success = false;
                                        break;
                                    }
                                }
                            }

                            if (success)
                            {
                                ui_model->m_asset_models.insert(found_asset_model, std::pair<mcrt_string, ui_asset_model_model_t>{std::move(asset_model_file_identity), ui_asset_model_model_t{std::move(surface_groups), std::move(skeletons)}});
                            }
                            else
                            {
                                assert(false);

                                for (brx_anari_surface_group *const surface_group : surface_groups)
                                {
                                    if (NULL != surface_group)
                                    {
                                        device->release_surface_group(surface_group);
                                    }
                                }
                                surface_groups.clear();

                                for (brx_motion_skeleton *const skeleton : skeletons)
                                {
                                    if (NULL != skeleton)
                                    {
                                        brx_motion_destroy_skeleton(skeleton);
                                    }
                                }
                                skeletons.clear();
                            }

                            brx_asset_import_destroy_scene(asset_import_scene);
                        }
                        else
                        {
                            assert(false);
                        }
                    }
                    else
                    {
                        assert(!found_asset_model->second.m_surface_groups.empty());
                        assert(!found_asset_model->second.m_skeletons.empty());
                        assert(found_asset_model->second.m_surface_groups.size() == found_asset_model->second.m_skeletons.size());
                    }
                }
            }
        }

        ImGui::Separator();

        {
            constexpr char const *const text[LANGUAGE_COUNT] = {
                "Delete",
                "削除",
                "刪除",
                "删除"};
            ImGui::TextUnformatted(text[ui_controller->m_language_index]);

            ImGui::SameLine();

            if (ImGui::Button("X##Asset-Model-Manager-Delete"))
            {
                auto const found_asset_model = ui_model->m_asset_models.find(ui_controller->m_tree_view_selected_asset_model);
                if (ui_model->m_asset_models.end() != found_asset_model)
                {
                    assert(!found_asset_model->second.m_surface_groups.empty());
                    assert(!found_asset_model->second.m_skeletons.empty());
                    assert(found_asset_model->second.m_surface_groups.size() == found_asset_model->second.m_skeletons.size());

                    for (brx_anari_surface_group *const surface_group : found_asset_model->second.m_surface_groups)
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
                    found_asset_model->second.m_surface_groups.clear();

                    for (brx_motion_skeleton *const skeleton : found_asset_model->second.m_skeletons)
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
                    found_asset_model->second.m_skeletons.clear();

                    ui_model->m_asset_models.erase(found_asset_model);
                    ui_controller->m_tree_view_selected_asset_model.clear();
                }
            }
        }

        ImGui::Separator();

        if (ImGui::BeginChild("##Asset-Model-Manager-Left-Child", ImVec2(ui_width * left_child_width_ratio, 0.0F), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX))
        {
            ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::PushItemFlag(ImGuiItemFlags_NoNavDefaultFocus, true);
            ImGuiTextFilter text_filter;
            {
                constexpr char const *const hint[LANGUAGE_COUNT] = {
                    "Search",
                    "検索",
                    "檢索",
                    "检索"};
                if (ImGui::InputTextWithHint("##Asset-Model-Manager-Left-Child-Text-Filter", hint[ui_controller->m_language_index], text_filter.InputBuf, IM_ARRAYSIZE(text_filter.InputBuf), ImGuiInputTextFlags_EscapeClearsAll))
                {
                    text_filter.Build();
                }
            }

            ImGui::PopItemFlag();

            if (ImGui::BeginTable("##Asset-Model-Manager-Left-Child-Table", 1, ImGuiTableFlags_RowBg))
            {
                for (auto const &asset_model : ui_model->m_asset_models)
                {
                    if (text_filter.PassFilter(asset_model.first.c_str()))
                    {
                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();

                        ImGuiTreeNodeFlags const flags = ((ui_controller->m_tree_view_selected_asset_model != asset_model.first)) ? ImGuiTreeNodeFlags_Leaf : (ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Selected);

                        bool const node_open = ImGui::TreeNodeEx(asset_model.first.c_str(), flags);

                        if (ImGui::IsItemFocused())
                        {
                            ui_controller->m_tree_view_selected_asset_model = asset_model.first;
                        }

                        if (node_open)
                        {
                            ImGui::TreePop();
                        }
                    }
                }

                ImGui::EndTable();
            }
        }
        ImGui::EndChild();

        auto const found_asset_model = ui_model->m_asset_models.find(ui_controller->m_tree_view_selected_asset_model);

        if (ui_model->m_asset_models.end() != found_asset_model)
        {
            ImGui::SameLine();

            ImGui::BeginGroup();

            mcrt_string timestamp_text = "Timestamp: ";
            mcrt_string directory_name;
            mcrt_string file_name;
            {
                size_t const timestamp_text_end_pos = found_asset_model->first.find(' ');
                size_t const directory_name_end_pos = found_asset_model->first.find_last_of("/\\");
                if ((mcrt_string::npos != timestamp_text_end_pos) && (mcrt_string::npos != directory_name_end_pos) && (timestamp_text_end_pos < directory_name_end_pos))
                {
                    timestamp_text += found_asset_model->first.substr(0U, timestamp_text_end_pos);
                    directory_name = found_asset_model->first.substr(timestamp_text_end_pos + 1U, ((directory_name_end_pos - timestamp_text_end_pos) - 1U));
                    file_name = found_asset_model->first.substr(directory_name_end_pos + 1U);
                }
                else
                {
                    assert(false);
                    timestamp_text = "N/A";
                    directory_name = "N/A";
                    file_name = "N/A";
                }
            }

            {
                ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                ImGui::TextUnformatted(file_name.c_str());
                ImGui::PopStyleColor();
            }

            {
                ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                ImGui::TextUnformatted(directory_name.c_str());
                ImGui::PopStyleColor();
            }

            {
                ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                ImGui::TextUnformatted(timestamp_text.c_str());
                ImGui::PopStyleColor();
            }

            ImGui::Separator();

            if (ImGui::BeginTable("##Asset-Model-Manager-Right-Group-Table", 2, ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersInnerV))
            {
                ImGui::TableSetupColumn("##Asset-Model-Manager-Right-Group-Table-Property", ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableSetupColumn("##Asset-Model-Manager-Right-Group-Table-Value", ImGuiTableColumnFlags_WidthStretch);

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Surface Group Count",
                        "表面群数",
                        "表面組數",
                        "表面组数"};
                    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                    ImGui::PopStyleColor();
                }
                ImGui::TableNextColumn();
                {
                    assert(!found_asset_model->second.m_surface_groups.empty());
                    assert(!found_asset_model->second.m_skeletons.empty());
                    uint32_t const surface_group_count = static_cast<uint32_t>(found_asset_model->second.m_surface_groups.size());
                    assert(surface_group_count == found_asset_model->second.m_skeletons.size());

                    char surface_group_count_text[] = {"18446744073709551615"};
                    std::snprintf(surface_group_count_text, sizeof(surface_group_count_text) / sizeof(surface_group_count_text[0]), "%llu", static_cast<long long unsigned>(surface_group_count));
                    surface_group_count_text[(sizeof(surface_group_count_text) / sizeof(surface_group_count_text[0])) - 1] = '\0';

                    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                    ImGui::TextUnformatted(surface_group_count_text);
                    ImGui::PopStyleColor();
                }

                ImGui::EndTable();
            }

            ImGui::EndGroup();
        }

        ImGui::End();
    }

    if (ui_controller->m_show_asset_image_manager)
    {
        {
            ImGuiViewport const *const main_viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + ui_x * ui_index, main_viewport->WorkPos.y + ui_y * ui_index), ImGuiCond_FirstUseEver);
            ++ui_index;

            ImGui::SetNextWindowSize(ImVec2(ui_width, ui_height), ImGuiCond_FirstUseEver);
        }

        ImGui::Begin("Asset Image Manager", &ui_controller->m_show_asset_image_manager);

        {
            constexpr char const *const text[LANGUAGE_COUNT] = {
                "Asset Image Manager",
                "資源画像管理",
                "資源图像管理",
                "资源圖像管理"};
            ImGui::TextUnformatted(text[ui_controller->m_language_index]);
        }

        ImGui::Separator();

        if (ImGui::TreeNodeEx("##Asset-Image-Manager-New-Node", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_NoAutoOpenOnLog))
        {
            if (ImGui::BeginTable("##Asset-Image-Manager-New-Table", 2, ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersInnerV))
            {
                ImGui::TableSetupColumn("##Asset-Image-Manager-New-Table-Property", ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableSetupColumn("##Asset-Image-Manager-New-Table-Value", ImGuiTableColumnFlags_WidthStretch);

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Force SRGB",
                        "強制 SRGB",
                        "強制 SRGB",
                        "强制 SRGB"};
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {
                    ImGui::Checkbox("##Asset-Image-Manager-Force-SRGB", &ui_controller->m_import_asset_image_force_srgb);
                }

                ImGui::EndTable();
            }

            ImGui::TreePop();
        }

        {
            constexpr char const *const text[LANGUAGE_COUNT] = {
                "Import",
                "導入",
                "導入",
                "导入"};
            ImGui::TextUnformatted(text[ui_controller->m_language_index]);

            ImGui::SameLine();

            if (ImGui::Button("O##Asset-Image-Manager-Import-Button"))
            {
                bool asset_image_file_open;
                mcrt_string asset_image_file_name;
                uint64_t asset_image_file_timestamp;
                mcrt_vector<uint8_t> asset_image_file_data;
                {
                    constexpr size_t const asset_image_filter_count = 6;

                    constexpr char const *const asset_image_filter_names[asset_image_filter_count] = {
                        "All Files",
                        "Web Picture",
                        "Portable Network Graphics",
                        "Joint Photographic Expert Group",
                        "Truevision Graphics Adapter",
                        "Extended Dynamic Range"};

                    constexpr char const *const asset_image_filter_specs[asset_image_filter_count] = {
                        "*",
                        "*.webp",
                        "*.png",
                        "*.jpg;*.jpeg",
                        "*.tga",
                        "*.exr"};

                    asset_image_file_open = internal_platform_get_open_file_name(asset_image_filter_count, asset_image_filter_names, asset_image_filter_specs, ui_controller->m_import_asset_image_get_open_file_name_file_type_index, &asset_image_file_name, &asset_image_file_timestamp, &asset_image_file_data);
                }

                if (asset_image_file_open)
                {
                    assert(!asset_image_file_name.empty());
                    assert(0U != asset_image_file_timestamp);
                    assert(!asset_image_file_data.empty());

                    brx_anari_image *const load_anari_image = internal_load_asset_image_file(asset_image_file_name.c_str(), asset_image_file_timestamp, asset_image_file_data.data(), asset_image_file_data.size(), ui_controller->m_import_asset_image_force_srgb, device, ui_model);
                    assert(NULL != load_anari_image);
                }
            }
        }

        ImGui::Separator();

        {
            constexpr char const *const text[LANGUAGE_COUNT] = {
                "Delete",
                "削除",
                "刪除",
                "删除"};
            ImGui::TextUnformatted(text[ui_controller->m_language_index]);

            ImGui::SameLine();

            if (ImGui::Button("X##Asset-Image-Manager-Delete"))
            {
                auto const found_asset_image = ui_model->m_asset_images.find(ui_controller->m_tree_view_selected_asset_image);
                if (ui_model->m_asset_images.end() != found_asset_image)
                {
                    if (NULL != found_asset_image->second.m_image)
                    {
                        device->release_image(found_asset_image->second.m_image);
                        found_asset_image->second.m_image = NULL;
                    }
                    else
                    {
                        assert(false);
                    }

                    ui_model->m_asset_images.erase(found_asset_image);
                    ui_controller->m_tree_view_selected_asset_image.clear();
                }
            }
        }

        ImGui::Separator();

        if (ImGui::BeginChild("##Asset-Image-Manager-Left-Child", ImVec2(ui_width * left_child_width_ratio, 0.0F), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX))
        {
            ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::PushItemFlag(ImGuiItemFlags_NoNavDefaultFocus, true);
            ImGuiTextFilter text_filter;
            {
                constexpr char const *const hint[LANGUAGE_COUNT] = {
                    "Search",
                    "検索",
                    "檢索",
                    "检索"};
                if (ImGui::InputTextWithHint("##Asset-Image-Manager-Left-Child-Text-Filter", hint[ui_controller->m_language_index], text_filter.InputBuf, IM_ARRAYSIZE(text_filter.InputBuf), ImGuiInputTextFlags_EscapeClearsAll))
                {
                    text_filter.Build();
                }
            }
            ImGui::PopItemFlag();

            if (ImGui::BeginTable("##Asset-Image-Manager-Left-Child-Table", 1, ImGuiTableFlags_RowBg))
            {
                for (auto const &asset_image : ui_model->m_asset_images)
                {
                    if (text_filter.PassFilter(asset_image.first.c_str()))
                    {
                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();

                        ImGuiTreeNodeFlags const flags = ((ui_controller->m_tree_view_selected_asset_image != asset_image.first)) ? ImGuiTreeNodeFlags_Leaf : (ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Selected);

                        bool const node_open = ImGui::TreeNodeEx(asset_image.first.c_str(), flags);

                        if (ImGui::IsItemFocused())
                        {
                            ui_controller->m_tree_view_selected_asset_image = asset_image.first;
                        }

                        if (node_open)
                        {
                            ImGui::TreePop();
                        }
                    }
                }

                ImGui::EndTable();
            }
        }
        ImGui::EndChild();

        auto const found_asset_image = ui_model->m_asset_images.find(ui_controller->m_tree_view_selected_asset_image);

        if (ui_model->m_asset_images.end() != found_asset_image)
        {
            ImGui::SameLine();

            ImGui::BeginGroup();

            if (0 == std::strncmp(found_asset_image->first.c_str(), "file", 4))
            {
                mcrt_string timestamp_text = "Timestamp: ";
                mcrt_string directory_name = "Directory: ";
                mcrt_string file_name = "File: ";
                {
                    size_t const timestamp_text_end_pos = found_asset_image->first.find(' ', 5U);
                    size_t const directory_name_end_pos = found_asset_image->first.find_last_of("/\\");
                    if ((mcrt_string::npos != timestamp_text_end_pos) && (mcrt_string::npos != directory_name_end_pos) && (5U < timestamp_text_end_pos) && (timestamp_text_end_pos < directory_name_end_pos))
                    {
                        timestamp_text += found_asset_image->first.substr(5U, (timestamp_text_end_pos - 5U));
                        directory_name += found_asset_image->first.substr(timestamp_text_end_pos + 1U, ((directory_name_end_pos - timestamp_text_end_pos) - 1U));
                        file_name += found_asset_image->first.substr(directory_name_end_pos + 1U);
                    }
                    else
                    {
                        assert(false);
                        timestamp_text = "N/A";
                        directory_name = "N/A";
                        file_name = "N/A";
                    }
                }

                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                    ImGui::TextUnformatted(file_name.c_str());
                    ImGui::PopStyleColor();
                }

                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                    ImGui::TextUnformatted(directory_name.c_str());
                    ImGui::PopStyleColor();
                }

                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                    ImGui::TextUnformatted(timestamp_text.c_str());
                    ImGui::PopStyleColor();
                }
            }
            else if (0 == std::strncmp(found_asset_image->first.c_str(), "data", 4))
            {
                mcrt_string size_text = "Size: ";
                mcrt_string hash_text = "Hash: ";
                {
                    size_t const size_text_end_pos = found_asset_image->first.find(' ', 5U);
                    if ((mcrt_string::npos != size_text_end_pos) && (5U < size_text_end_pos))
                    {
                        size_text += found_asset_image->first.substr(5U, (size_text_end_pos - 5U));
                        hash_text += found_asset_image->first.substr(size_text_end_pos + 1U);
                    }
                    else
                    {
                        assert(false);
                        size_text = "N/A";
                        hash_text = "N/A";
                    }
                }

                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                    ImGui::TextUnformatted(hash_text.c_str());
                    ImGui::PopStyleColor();
                }

                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                    ImGui::TextUnformatted(size_text.c_str());
                    ImGui::PopStyleColor();
                }
            }
            else
            {
                assert(false);

                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                    ImGui::TextUnformatted("Unknown Source");
                    ImGui::PopStyleColor();
                }
            }

            ImGui::Separator();

            if (ImGui::BeginTable("##Asset-Image-Manager-Right-Group-Table", 2, ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersInnerV))
            {
                ImGui::TableSetupColumn("##Asset-Image-Manager-Right-Group-Table-Property", ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableSetupColumn("##Asset-Image-Manager-Right-Group-Table-Value", ImGuiTableColumnFlags_WidthStretch);

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Force SRGB",
                        "強制 SRGB",
                        "強制 SRGB",
                        "强制 SRGB"};
                    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                    ImGui::PopStyleColor();
                }
                ImGui::TableNextColumn();
                {
                    if (found_asset_image->second.m_force_srgb)
                    {
                        constexpr char const *const text[LANGUAGE_COUNT] = {
                            "Enable",
                            "有効",
                            "啟用",
                            "启用"};

                        ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                        ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                        ImGui::PopStyleColor();
                    }
                    else
                    {
                        char const *text[LANGUAGE_COUNT] = {
                            "Disable",
                            "無効",
                            "停用",
                            "停用"};

                        ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                        ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                        ImGui::PopStyleColor();
                    }
                }

                ImGui::EndTable();
            }

            ImGui::EndGroup();
        }

        ImGui::End();
    }

    if (ui_controller->m_show_video_detector_manager)
    {
        {
            ImGuiViewport const *const main_viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + ui_x * ui_index, main_viewport->WorkPos.y + ui_y * ui_index), ImGuiCond_FirstUseEver);
            ++ui_index;

            ImGui::SetNextWindowSize(ImVec2(ui_width, ui_height), ImGuiCond_FirstUseEver);
        }

        ImGui::Begin("Video Detector Manager", &ui_controller->m_show_video_detector_manager);

        {
            constexpr char const *const text[LANGUAGE_COUNT] = {
                "Video Detector Manager",
                "動画検出管理",
                "視訊檢測管理",
                "视频检测管理"};
            ImGui::TextUnformatted(text[ui_controller->m_language_index]);
        }

        ImGui::Separator();

        if (ImGui::TreeNodeEx("##Video-Detector-Manager-New-Node", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_NoAutoOpenOnLog))
        {
            if (ImGui::BeginTable("##Video-Detector-Manager-New-Table", 2, ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersInnerV))
            {
                ImGui::TableSetupColumn("##Video-Detector-Manager-New-Table-Property", ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableSetupColumn("##Video-Detector-Manager-New-Table-Value", ImGuiTableColumnFlags_WidthStretch);

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Name",
                        "名称",
                        "名稱",
                        "名称"};
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {
                    constexpr char const *const hint[LANGUAGE_COUNT] = {
                        "New Video Detector Name",
                        "新規動画検出名前",
                        "新建視訊檢測名稱",
                        "新建视频检测名称"};
                    ImGui::InputTextWithHint("##Video-Detector-Manager-New-Instance-Motion-Name", hint[ui_controller->m_language_index], ui_controller->m_new_video_detector_name.data(), ui_controller->m_new_video_detector_name.size());
                }

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Video Capture",
                        "動画捕獲",
                        "視訊捕捉",
                        "视频捕捉"};
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {
                    constexpr char const *const label = "##Instance-Motion-Manager-Select-Asset-Motion";

                    if (!ui_model->m_video_captures.empty())
                    {
                        mcrt_vector<mcrt_string> item_strings(static_cast<size_t>(ui_model->m_video_captures.size()));
                        mcrt_vector<char const *> items(static_cast<size_t>(ui_model->m_video_captures.size()));
                        int selected_combo_index = 0;
                        {
                            size_t combo_index = 0U;
                            for (auto const &video_capture : ui_model->m_video_captures)
                            {
                                item_strings[combo_index] = video_capture.first;
                                items[combo_index] = item_strings[combo_index].c_str();

                                if (ui_controller->m_new_video_detector_selected_video_capture == video_capture.first)
                                {
                                    assert(0 == selected_combo_index);
                                    assert((1U + combo_index) < static_cast<size_t>(INT_MAX));
                                    selected_combo_index = static_cast<int>(combo_index);
                                }

                                ++combo_index;
                            }
                        }

                        ImGui::Combo(label, &selected_combo_index, items.data(), items.size());

                        if (selected_combo_index >= 0)
                        {
                            ui_controller->m_new_video_detector_selected_video_capture = std::move(item_strings[selected_combo_index]);
                        }
                        else
                        {
                            assert(false);
                        }
                    }
                    else
                    {

                        char const *const items[LANGUAGE_COUNT][1] = {
                            {"Disable"},
                            {"無効"},
                            {"停用"},
                            {"停用"}};

                        int selected_combo_index = 0;

                        ImGui::Combo(label, &selected_combo_index, items[ui_controller->m_language_index], IM_ARRAYSIZE(items[ui_controller->m_language_index]));

                        assert(0 == selected_combo_index);

                        ui_controller->m_new_video_detector_selected_video_capture.clear();
                    }
                }

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Maximum Hand Count",
                        "人手最大数",
                        "最大人手數量",
                        "最大人手数量"};

                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {
                    int hand_count = std::min(std::max(0U, ui_controller->m_new_video_detector_hand_count), 5U);

                    ImGui::SliderInt("##Video-Detector-Manager-New-Video-Detector-Hand-Count", &hand_count, 0, 5);

                    ui_controller->m_new_video_detector_hand_count = std::min(std::max(0, hand_count), 5);
                }

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Maximum Face Count",
                        "人顔最大数",
                        "最大人臉數量",
                        "最大人脸数量"};

                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {
                    int face_count = std::min(std::max(0U, ui_controller->m_new_video_detector_face_count), 5U);

                    ImGui::SliderInt("##Video-Detector-Manager-New-Video-Detector-Face-Count", &face_count, 0, 5);

                    ui_controller->m_new_video_detector_face_count = std::min(std::max(0, face_count), 5);
                }

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Maximum Pose Count",
                        "姿勢最大数",
                        "最大姿勢數量",
                        "最大姿勢数量"};

                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {
                    int pose_count = std::min(std::max(0U, ui_controller->m_new_video_detector_pose_count), 5U);

                    ImGui::SliderInt("##Video-Detector-Manager-New-Video-Detector-Pose-Count", &pose_count, 0, 5);

                    ui_controller->m_new_video_detector_pose_count = std::min(std::max(0, pose_count), 5);
                }

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *text[LANGUAGE_COUNT] = {
                        "Force GPU",
                        "強制 GPU",
                        "強制 GPU",
                        "强制 GPU"};

                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {
                    ImGui::Checkbox("##Video-Detector-Manager-New-Video-Detector-Force-GPU", &ui_controller->m_new_video_detector_force_gpu);
                }
                ImGui::SameLine();
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                    ImGui::TextUnformatted(help_marker_text);
                    ImGui::PopStyleColor();
                    if (ImGui::BeginItemTooltip())
                    {
                        ImGui::PushTextWrapPos(help_marker_position);
                        constexpr char const *const text[LANGUAGE_COUNT] = {
                            "GPU inference does NOT necessarily guarantee better performance.",
                            "GPU推論は必ずしも性能向上を保証せず。",
                            "GPU推理并不一定能保證更好的性能。",
                            "GPU推理并不一定能保证更好的性能。"};
                        ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                        ImGui::PopTextWrapPos();
                        ImGui::EndTooltip();
                    }
                }

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *text[LANGUAGE_COUNT] = {
                        "Enable Debug Renderer",
                        "調試描画器有効",
                        "啟用調試器渲染",
                        "启用调试器渲染"};

                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {
                    ImGui::Checkbox("##Video-Detector-Manager-New-Video-Detector-Enable-Debug-Renderer", &ui_controller->m_new_video_detector_enable_debug_renderer);
                }

                ImGui::EndTable();
            }

            ImGui::TreePop();
        }

        {
            {
                constexpr char const *const text[LANGUAGE_COUNT] = {
                    "New",
                    "新規作成",
                    "新建",
                    "新建"};

                ImGui::TextUnformatted(text[ui_controller->m_language_index]);
            }

            ImGui::SameLine();

            if (ImGui::Button("N##Video-Detector-Manager-New-Button"))
            {
                mcrt_string const selected_video_capture = ui_controller->m_new_video_detector_selected_video_capture;

                auto const found_video_capture = ui_model->m_video_captures.find(selected_video_capture);

                if (ui_model->m_video_captures.end() != found_video_capture)
                {
                    brx_motion_video_capture *const video_capture = found_video_capture->second.m_video_capture;

                    if (NULL != video_capture)
                    {
                        uint32_t const hand_count = ui_controller->m_new_video_detector_hand_count;

                        uint32_t const face_count = ui_controller->m_new_video_detector_face_count;

                        uint32_t const pose_count = ui_controller->m_new_video_detector_pose_count;

                        bool const force_gpu = ui_controller->m_new_video_detector_force_gpu;

                        bool const enable_debug_renderer = ui_controller->m_new_video_detector_enable_debug_renderer;

                        mcrt_string const name = ui_controller->m_new_video_detector_name.data();

                        brx_motion_video_detector *const video_detector = brx_motion_create_video_detector(hand_count, face_count, pose_count, force_gpu, video_capture);
                        if (NULL != video_detector)
                        {
                            uint64_t const timestamp = mcrt_tick_count_now();

                            mcrt_string video_detector_identity;
                            {
                                char video_detector_timestamp_text[] = {"18446744073709551615"};
                                std::snprintf(video_detector_timestamp_text, sizeof(video_detector_timestamp_text) / sizeof(video_detector_timestamp_text[0]), "%020llu", static_cast<long long unsigned>(timestamp));
                                video_detector_timestamp_text[(sizeof(video_detector_timestamp_text) / sizeof(video_detector_timestamp_text[0])) - 1] = '\0';

                                video_detector_identity += video_detector_timestamp_text;
                                video_detector_identity += ' ';
                                video_detector_identity += name;
                            }

                            video_detector->set_enable_debug_renderer(enable_debug_renderer, video_detector_identity.c_str());

                            // should always NOT already exist in practice
                            assert(ui_model->m_video_detectors.end() == ui_model->m_video_detectors.find(timestamp));

                            ui_model->m_video_detectors.insert(ui_model->m_video_detectors.end(), std::pair<uint64_t, ui_video_detector_model_t>{timestamp, ui_video_detector_model_t{name, selected_video_capture, video_detector}});
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
        }

        ImGui::Separator();

        {
            constexpr char const *const text[LANGUAGE_COUNT] = {
                "Delete",
                "削除",
                "刪除",
                "删除"};
            ImGui::TextUnformatted(text[ui_controller->m_language_index]);

            ImGui::SameLine();

            if (ImGui::Button("X##Video-Detector-Manager-Delete"))
            {
                auto const found_video_detector = ui_model->m_video_detectors.find(ui_controller->m_tree_view_selected_video_detector);
                if (ui_model->m_video_detectors.end() != found_video_detector)
                {
                    if (NULL != found_video_detector->second.m_video_detector)
                    {
                        brx_motion_destroy_video_detector(found_video_detector->second.m_video_detector);
                        found_video_detector->second.m_video_detector = NULL;
                    }
                    else
                    {
                        assert(false);
                    }

                    ui_model->m_video_detectors.erase(found_video_detector);
                    ui_controller->m_tree_view_selected_video_detector = INVALID_TIMESTAMP;
                }
            }
        }

        ImGui::Separator();

        if (ImGui::BeginChild("##Video-Detector-Manager-Left-Child", ImVec2(ui_width * left_child_width_ratio, 0.0F), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX))
        {
            ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::PushItemFlag(ImGuiItemFlags_NoNavDefaultFocus, true);
            ImGuiTextFilter text_filter;
            {
                constexpr char const *const hint[LANGUAGE_COUNT] = {
                    "Search",
                    "検索",
                    "檢索",
                    "检索"};
                if (ImGui::InputTextWithHint("##Video-Video-Manager-Left-Child-Text-Filter", hint[ui_controller->m_language_index], text_filter.InputBuf, IM_ARRAYSIZE(text_filter.InputBuf), ImGuiInputTextFlags_EscapeClearsAll))
                {
                    text_filter.Build();
                }
            }

            ImGui::PopItemFlag();

            if (ImGui::BeginTable("##Video-Video-Manager-Left-Child-Table", 1, ImGuiTableFlags_RowBg))
            {
                for (auto const &video_detector : ui_model->m_video_detectors)
                {
                    mcrt_string video_detector_identity;
                    {
                        char video_detector_timestamp_text[] = {"18446744073709551615"};
                        std::snprintf(video_detector_timestamp_text, sizeof(video_detector_timestamp_text) / sizeof(video_detector_timestamp_text[0]), "%020llu", static_cast<long long unsigned>(video_detector.first));
                        video_detector_timestamp_text[(sizeof(video_detector_timestamp_text) / sizeof(video_detector_timestamp_text[0])) - 1] = '\0';

                        video_detector_identity += video_detector_timestamp_text;
                        video_detector_identity += ' ';
                        video_detector_identity += video_detector.second.m_name;
                    }

                    if (text_filter.PassFilter(video_detector_identity.c_str()))
                    {
                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();

                        ImGuiTreeNodeFlags const flags = ((ui_controller->m_tree_view_selected_video_detector != video_detector.first)) ? ImGuiTreeNodeFlags_Leaf : (ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Selected);

                        bool const node_open = ImGui::TreeNodeEx(video_detector_identity.c_str(), flags);

                        if (ImGui::IsItemFocused())
                        {
                            ui_controller->m_tree_view_selected_video_detector = video_detector.first;
                        }

                        if (node_open)
                        {
                            ImGui::TreePop();
                        }
                    }
                }

                ImGui::EndTable();
            }
        }
        ImGui::EndChild();

        auto const found_video_detector = ui_model->m_video_detectors.find(ui_controller->m_tree_view_selected_video_detector);

        if (ui_model->m_video_detectors.end() != found_video_detector)
        {
            ImGui::SameLine();

            ImGui::BeginGroup();

            {
                ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                ImGui::TextUnformatted(found_video_detector->second.m_name.c_str());
                ImGui::PopStyleColor();
            }

            ImGui::Separator();
            {
                {
                    size_t const timestamp_text_end_pos = found_video_detector->second.m_video_capture.find(' ');
                    if (mcrt_string::npos != timestamp_text_end_pos)
                    {
                        if ((((timestamp_text_end_pos + 9U) + 1U) < found_video_detector->second.m_video_capture.length()) && ('c' == found_video_detector->second.m_video_capture[timestamp_text_end_pos + 1U]) && ('a' == found_video_detector->second.m_video_capture[timestamp_text_end_pos + 2U]) && ('m' == found_video_detector->second.m_video_capture[timestamp_text_end_pos + 3U]) && ('e' == found_video_detector->second.m_video_capture[timestamp_text_end_pos + 4U]) && ('r' == found_video_detector->second.m_video_capture[timestamp_text_end_pos + 5U]) && ('a' == found_video_detector->second.m_video_capture[timestamp_text_end_pos + 6U]) && (':' == found_video_detector->second.m_video_capture[timestamp_text_end_pos + 7U]) && ('/' == found_video_detector->second.m_video_capture[timestamp_text_end_pos + 8U]) && ('/' == found_video_detector->second.m_video_capture[timestamp_text_end_pos + 9U]))
                        {
                            mcrt_string camera_index_text;
                            {
                                constexpr char const *const text[LANGUAGE_COUNT] = {
                                    "Camera Index: ",
                                    "写真機索引: ",
                                    "相機索引: ",
                                    "相机索引: "};
                                camera_index_text = text[ui_controller->m_language_index];
                                camera_index_text += (found_video_detector->second.m_video_capture.c_str() + ((timestamp_text_end_pos + 9U) + 1U));
                            }

                            ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                            ImGui::TextUnformatted(camera_index_text.c_str());
                            ImGui::PopStyleColor();
                        }
                        else if ((((timestamp_text_end_pos + 7U) + 1U) < found_video_detector->second.m_video_capture.length()) && ('f' == found_video_detector->second.m_video_capture[timestamp_text_end_pos + 1U]) && ('i' == found_video_detector->second.m_video_capture[timestamp_text_end_pos + 2U]) && ('l' == found_video_detector->second.m_video_capture[timestamp_text_end_pos + 3U]) && ('e' == found_video_detector->second.m_video_capture[timestamp_text_end_pos + 4U]) && (':' == found_video_detector->second.m_video_capture[timestamp_text_end_pos + 5U]) && ('/' == found_video_detector->second.m_video_capture[timestamp_text_end_pos + 6U]) && ('/' == found_video_detector->second.m_video_capture[timestamp_text_end_pos + 7U]))
                        {
                            mcrt_string timestamp_text = "Timestamp: ";
                            mcrt_string directory_name;
                            mcrt_string file_name;
                            size_t const directory_name_end_pos = found_video_detector->second.m_video_capture.find_last_of("/\\");
                            if ((mcrt_string::npos != timestamp_text_end_pos) && (mcrt_string::npos != directory_name_end_pos) && ((timestamp_text_end_pos + 7U) < directory_name_end_pos))
                            {
                                timestamp_text += found_video_detector->second.m_video_capture.substr(0U, timestamp_text_end_pos);
                                directory_name = found_video_detector->second.m_video_capture.substr((timestamp_text_end_pos + 7U) + 1U, ((directory_name_end_pos - (timestamp_text_end_pos + 7U)) - 1U));
                                file_name = found_video_detector->second.m_video_capture.substr(directory_name_end_pos + 1U);
                            }
                            else
                            {
                                assert(false);
                                timestamp_text = "N/A";
                                directory_name = "N/A";
                                file_name = "N/A";
                            }

                            {
                                ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                                ImGui::TextUnformatted(file_name.c_str());
                                ImGui::PopStyleColor();
                            }

                            {
                                ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                                ImGui::TextUnformatted(directory_name.c_str());
                                ImGui::PopStyleColor();
                            }

                            {
                                ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                                ImGui::TextUnformatted(timestamp_text.c_str());
                                ImGui::PopStyleColor();
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

            ImGui::Separator();

            if (ImGui::BeginTable("##Video-Detector-Manager-Right-Group-Table", 2, ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersInnerV))
            {
                ImGui::TableSetupColumn("##Video-Detector-Manager-Right-Group-Table-Property", ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableSetupColumn("##Video-Detector-Manager-Right-Group-Table-Value", ImGuiTableColumnFlags_WidthStretch);

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Maximum Hand Count",
                        "人手最大数",
                        "最大人手數量",
                        "最大人手数量"};
                    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                    ImGui::PopStyleColor();
                }

                ImGui::TableNextColumn();
                {
                    assert(NULL != found_video_detector->second.m_video_detector);

                    char hand_count_timestamp_text[] = {"18446744073709551615"};
                    std::snprintf(hand_count_timestamp_text, sizeof(hand_count_timestamp_text) / sizeof(hand_count_timestamp_text[0]), "%llu", static_cast<long long unsigned>(found_video_detector->second.m_video_detector->get_hand_count()));
                    hand_count_timestamp_text[(sizeof(hand_count_timestamp_text) / sizeof(hand_count_timestamp_text[0])) - 1] = '\0';

                    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                    ImGui::TextUnformatted(hand_count_timestamp_text);
                    ImGui::PopStyleColor();
                }

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Maximum Face Count",
                        "人顔最大数",
                        "最大人臉數量",
                        "最大人脸数量"};
                    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                    ImGui::PopStyleColor();
                }

                ImGui::TableNextColumn();
                {
                    assert(NULL != found_video_detector->second.m_video_detector);

                    char face_count_timestamp_text[] = {"18446744073709551615"};
                    std::snprintf(face_count_timestamp_text, sizeof(face_count_timestamp_text) / sizeof(face_count_timestamp_text[0]), "%llu", static_cast<long long unsigned>(found_video_detector->second.m_video_detector->get_face_count()));
                    face_count_timestamp_text[(sizeof(face_count_timestamp_text) / sizeof(face_count_timestamp_text[0])) - 1] = '\0';

                    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                    ImGui::TextUnformatted(face_count_timestamp_text);
                    ImGui::PopStyleColor();
                }

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Maximum Pose Count",
                        "姿勢最大数",
                        "最大姿勢數量",
                        "最大姿勢数量"};
                    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                    ImGui::PopStyleColor();
                }

                ImGui::TableNextColumn();
                {
                    assert(NULL != found_video_detector->second.m_video_detector);

                    char pose_count_timestamp_text[] = {"18446744073709551615"};
                    std::snprintf(pose_count_timestamp_text, sizeof(pose_count_timestamp_text) / sizeof(pose_count_timestamp_text[0]), "%llu", static_cast<long long unsigned>(found_video_detector->second.m_video_detector->get_pose_count()));
                    pose_count_timestamp_text[(sizeof(pose_count_timestamp_text) / sizeof(pose_count_timestamp_text[0])) - 1] = '\0';

                    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                    ImGui::TextUnformatted(pose_count_timestamp_text);
                    ImGui::PopStyleColor();
                }

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *text[LANGUAGE_COUNT] = {
                        "GPU",
                        "GPU",
                        "GPU",
                        "GPU"};
                    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                    ImGui::PopStyleColor();
                }

                ImGui::TableNextColumn();
                {
                    assert(NULL != found_video_detector->second.m_video_detector);

                    if (found_video_detector->second.m_video_detector->get_enable_gpu())
                    {
                        constexpr char const *const text[LANGUAGE_COUNT] = {
                            "Enable",
                            "有効",
                            "啟用",
                            "启用"};
                        ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                        ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                        ImGui::PopStyleColor();
                    }
                    else
                    {
                        char const *text[LANGUAGE_COUNT] = {
                            "Disable",
                            "無効",
                            "停用",
                            "停用"};
                        ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                        ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                        ImGui::PopStyleColor();
                    }
                }

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *text[LANGUAGE_COUNT] = {
                        "Enable Debug Renderer",
                        "Debug Renderer 有効",
                        "啟用調試渲染",
                        "启用调试渲染"};
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }

                ImGui::TableNextColumn();
                {
                    assert(NULL != found_video_detector->second.m_video_detector);

                    bool enable_debug_renderer = found_video_detector->second.m_video_detector->get_enable_debug_renderer();

                    ImGui::Checkbox("##Video-Detector-Manager-Right-Group-Table-Value-Enable-Debug-Renderer", &enable_debug_renderer);

                    mcrt_string video_detector_identity;
                    {
                        char video_detector_timestamp_text[] = {"18446744073709551615"};
                        std::snprintf(video_detector_timestamp_text, sizeof(video_detector_timestamp_text) / sizeof(video_detector_timestamp_text[0]), "%020llu", static_cast<long long unsigned>(found_video_detector->first));
                        video_detector_timestamp_text[(sizeof(video_detector_timestamp_text) / sizeof(video_detector_timestamp_text[0])) - 1] = '\0';

                        video_detector_identity += video_detector_timestamp_text;
                        video_detector_identity += ' ';
                        video_detector_identity += found_video_detector->second.m_name;
                    }

                    found_video_detector->second.m_video_detector->set_enable_debug_renderer(enable_debug_renderer, video_detector_identity.c_str());
                }

                ImGui::EndTable();
            }

            ImGui::EndGroup();
        }

        ImGui::End();
    }

    if (ui_controller->m_show_instance_motion_manager)
    {
        {
            ImGuiViewport const *const main_viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + ui_x * ui_index, main_viewport->WorkPos.y + ui_y * ui_index), ImGuiCond_FirstUseEver);
            ++ui_index;

            ImGui::SetNextWindowSize(ImVec2(ui_width, ui_height), ImGuiCond_FirstUseEver);
        }

        ImGui::Begin("Instance Motion Manager", &ui_controller->m_show_instance_motion_manager);

        {
            constexpr char const *const text[LANGUAGE_COUNT] = {
                "Instance Motion Manager",
                "実例行動管理",
                "實例動作管理",
                "实例动作管理"};
            ImGui::TextUnformatted(text[ui_controller->m_language_index]);
        }

        ImGui::Separator();

        if (ImGui::TreeNodeEx("##Instance-Motion-Manager-New-Node", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_NoAutoOpenOnLog))
        {
            if (ImGui::BeginTable("##Instance-Motion-Manager-New-Table", 2, ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersInnerV))
            {
                ImGui::TableSetupColumn("##Instance-Motion-Manager-New-Table-Property", ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableSetupColumn("##Instance-Motion-Manager-New-Table-Value", ImGuiTableColumnFlags_WidthStretch);

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Name",
                        "名称",
                        "名稱",
                        "名称"};
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {
                    constexpr char const *const hint[LANGUAGE_COUNT] = {
                        "New Instance Motion Name",
                        "新規実例行動名前",
                        "新建實例動作名稱",
                        "新建实例动作名称"};
                    ImGui::InputTextWithHint("##Instance-Motion-Manager-New-Instance-Motion-Name", hint[ui_controller->m_language_index], ui_controller->m_new_instance_motion_name.data(), ui_controller->m_new_instance_motion_name.size());
                }

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Asset Motion",
                        "資源行動",
                        "資源動作",
                        "资源动作"};
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {

                    constexpr char const *const label = "##Instance-Motion-Manager-Select-Asset-Motion";

                    if (!ui_model->m_asset_motions.empty())
                    {

                        mcrt_vector<mcrt_string> item_strings(static_cast<size_t>(ui_model->m_asset_motions.size()));
                        mcrt_vector<char const *> items(static_cast<size_t>(ui_model->m_asset_motions.size()));
                        int selected_combo_index = 0;
                        {
                            size_t combo_index = 0U;
                            for (auto const &asset_motion : ui_model->m_asset_motions)
                            {
                                item_strings[combo_index] = asset_motion.first;
                                items[combo_index] = item_strings[combo_index].c_str();

                                if (ui_controller->m_new_instance_motion_selected_asset_motion == asset_motion.first)
                                {
                                    assert(0 == selected_combo_index);
                                    assert((1U + combo_index) < static_cast<size_t>(INT_MAX));
                                    selected_combo_index = static_cast<int>(combo_index);
                                }

                                ++combo_index;
                            }
                        }

                        ImGui::Combo(label, &selected_combo_index, items.data(), items.size());

                        if (selected_combo_index >= 0)
                        {
                            ui_controller->m_new_instance_motion_selected_asset_motion = std::move(item_strings[selected_combo_index]);
                        }
                        else
                        {
                            assert(false);
                        }
                    }
                    else
                    {
                        char const *const items[LANGUAGE_COUNT][1] = {
                            {"Disable"},
                            {"無効"},
                            {"停用"},
                            {"停用"}};

                        int selected_combo_index = 0;

                        ImGui::Combo(label, &selected_combo_index, items[ui_controller->m_language_index], IM_ARRAYSIZE(items[ui_controller->m_language_index]));

                        assert(0 == selected_combo_index);

                        ui_controller->m_new_instance_motion_selected_asset_motion.clear();
                    }
                }

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Animation Index",
                        "動作索引",
                        "動畫索引",
                        "动画索引"};

                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {
                    auto const found_asset_motion = ui_model->m_asset_motions.find(ui_controller->m_new_instance_motion_selected_asset_motion);

                    constexpr char const *const label = "##Instance-Motion-Manager-Select-Animation-Index";

                    if (ui_model->m_asset_motions.end() != found_asset_motion)
                    {
                        assert(!found_asset_motion->second.m_animations.empty());
                        uint32_t const animation_count = static_cast<uint32_t>(found_asset_motion->second.m_animations.size());

                        if (animation_count > 0U)
                        {
                            mcrt_vector<uint32_t> item_values(static_cast<size_t>(animation_count));
                            mcrt_vector<mcrt_string> item_strings(static_cast<size_t>(animation_count));
                            mcrt_vector<char const *> items(static_cast<size_t>(animation_count));
                            int selected_combo_index = 0;
                            {
                                uint32_t combo_index = 0U;

                                for (uint32_t animation_index = 0U; animation_index < animation_count; ++animation_index)
                                {
                                    item_values[combo_index] = animation_index;

                                    char item_value_text[] = {"18446744073709551615"};
                                    std::snprintf(item_value_text, sizeof(item_value_text) / sizeof(item_value_text[0]), "%llu", static_cast<long long unsigned>(item_values[combo_index]));
                                    item_value_text[(sizeof(item_value_text) / sizeof(item_value_text[0])) - 1] = '\0';

                                    item_strings[combo_index] = item_value_text;
                                    items[combo_index] = item_strings[combo_index].c_str();

                                    if (ui_controller->m_new_instance_motion_selected_animation_index == item_values[combo_index])
                                    {
                                        assert(0 == selected_combo_index);
                                        assert(static_cast<size_t>(1U + combo_index) < static_cast<size_t>(INT_MAX));
                                        selected_combo_index = static_cast<int>(combo_index);
                                    }

                                    ++combo_index;
                                }
                            }

                            ImGui::Combo(label, &selected_combo_index, items.data(), items.size());

                            if (selected_combo_index >= 0)
                            {
                                ui_controller->m_new_instance_motion_selected_animation_index = item_values[selected_combo_index];
                            }
                            else
                            {
                                assert(false);
                            }
                        }
                        else
                        {
                            assert(false);

                            char const *const items[LANGUAGE_COUNT][1] = {
                                {"Disable"},
                                {"無効"},
                                {"停用"},
                                {"停用"}};

                            int selected_combo_index = 0;

                            ImGui::Combo(label, &selected_combo_index, items[ui_controller->m_language_index], IM_ARRAYSIZE(items[ui_controller->m_language_index]));

                            assert(0 == selected_combo_index);

                            ui_controller->m_new_instance_motion_selected_animation_index = BRX_MOTION_UINT32_INDEX_INVALID;
                        }
                    }
                    else
                    {
                        char const *const items[LANGUAGE_COUNT][1] = {
                            {"Disable"},
                            {"無効"},
                            {"停用"},
                            {"停用"}};

                        int selected_combo_index = 0;

                        ImGui::Combo(label, &selected_combo_index, items[ui_controller->m_language_index], IM_ARRAYSIZE(items[ui_controller->m_language_index]));

                        assert(0 == selected_combo_index);

                        ui_controller->m_new_instance_motion_selected_animation_index = BRX_MOTION_UINT32_INDEX_INVALID;
                    }
                }

                ImGui::EndTable();
            }

            ImGui::TreePop();
        }

        {
            {
                constexpr char const *const text[LANGUAGE_COUNT] = {
                    "New",
                    "新規作成",
                    "新建",
                    "新建"};

                ImGui::TextUnformatted(text[ui_controller->m_language_index]);
            }

            ImGui::SameLine();

            if (ImGui::Button("N##Instance-Motion-Manager-New"))
            {
                mcrt_string const selected_asset_motion = ui_controller->m_new_instance_motion_selected_asset_motion;

                int const selected_animation_index = ui_controller->m_new_instance_motion_selected_animation_index;

                auto const found_asset_motion = ui_model->m_asset_motions.find(selected_asset_motion);

                if ((ui_model->m_asset_motions.end() != found_asset_motion) && (selected_animation_index < found_asset_motion->second.m_animations.size()))
                {
                    brx_motion_animation *const motion_animation = found_asset_motion->second.m_animations[selected_animation_index];

                    if (NULL != motion_animation)
                    {
                        brx_motion_animation_instance *const motion_animation_instance = brx_motion_create_animation_instance(motion_animation);
                        if (NULL != motion_animation_instance)
                        {
                            uint64_t const timestamp = mcrt_tick_count_now();

                            // should always NOT already exist in practice
                            assert(ui_model->m_instance_motions.end() == ui_model->m_instance_motions.find(timestamp));

                            ui_model->m_instance_motions.insert(ui_model->m_instance_motions.end(), std::pair<uint64_t, ui_instance_motion_model_t>{timestamp, ui_instance_motion_model_t{ui_controller->m_new_instance_motion_name.data(), selected_asset_motion, static_cast<uint32_t>(selected_animation_index), motion_animation_instance}});
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
        }

        ImGui::Separator();

        {
            constexpr char const *const text[LANGUAGE_COUNT] = {
                "Delete",
                "削除",
                "刪除",
                "删除"};
            ImGui::TextUnformatted(text[ui_controller->m_language_index]);

            ImGui::SameLine();

            if (ImGui::Button("X##Instance-Motion-Manager-Delete"))
            {
                auto const found_instance_motion = ui_model->m_instance_motions.find(ui_controller->m_tree_view_selected_instance_motion);
                if (ui_model->m_instance_motions.end() != found_instance_motion)
                {
                    if (NULL != found_instance_motion->second.m_animation_instance)
                    {
                        brx_motion_destroy_animation_instance(found_instance_motion->second.m_animation_instance);
                        found_instance_motion->second.m_animation_instance = NULL;
                    }
                    else
                    {
                        assert(false);
                    }

                    ui_model->m_instance_motions.erase(found_instance_motion);
                    ui_controller->m_tree_view_selected_instance_motion = INVALID_TIMESTAMP;
                }
            }
        }

        ImGui::Separator();

        if (ImGui::BeginChild("##Instance-Motion-Manager-Left-Child", ImVec2(ui_width * left_child_width_ratio, 0.0F), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX))
        {
            ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::PushItemFlag(ImGuiItemFlags_NoNavDefaultFocus, true);
            ImGuiTextFilter text_filter;
            {
                constexpr char const *const hint[LANGUAGE_COUNT] = {
                    "Search",
                    "検索",
                    "檢索",
                    "检索"};
                if (ImGui::InputTextWithHint("##Instance-Motion-Manager-Left-Child-Text-Filter", hint[ui_controller->m_language_index], text_filter.InputBuf, IM_ARRAYSIZE(text_filter.InputBuf), ImGuiInputTextFlags_EscapeClearsAll))
                {
                    text_filter.Build();
                }
            }

            ImGui::PopItemFlag();

            if (ImGui::BeginTable("##Instance-Motion-Manager-Left-Child-Table", 1, ImGuiTableFlags_RowBg))
            {
                for (auto const &instance_motion : ui_model->m_instance_motions)
                {
                    mcrt_string instance_motion_identity;
                    {
                        char instance_motion_timestamp_text[] = {"18446744073709551615"};
                        std::snprintf(instance_motion_timestamp_text, sizeof(instance_motion_timestamp_text) / sizeof(instance_motion_timestamp_text[0]), "%020llu", static_cast<long long unsigned>(instance_motion.first));
                        instance_motion_timestamp_text[(sizeof(instance_motion_timestamp_text) / sizeof(instance_motion_timestamp_text[0])) - 1] = '\0';

                        instance_motion_identity += instance_motion_timestamp_text;
                        instance_motion_identity += ' ';
                        instance_motion_identity += instance_motion.second.m_name;
                    }

                    if (text_filter.PassFilter(instance_motion_identity.c_str()))
                    {
                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();

                        ImGuiTreeNodeFlags const flags = ((ui_controller->m_tree_view_selected_instance_motion != instance_motion.first)) ? ImGuiTreeNodeFlags_Leaf : (ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Selected);

                        bool const node_open = ImGui::TreeNodeEx(instance_motion_identity.c_str(), flags);

                        if (ImGui::IsItemFocused())
                        {
                            ui_controller->m_tree_view_selected_instance_motion = instance_motion.first;
                        }

                        if (node_open)
                        {
                            ImGui::TreePop();
                        }
                    }
                }

                ImGui::EndTable();
            }
        }
        ImGui::EndChild();

        auto const found_instance_motion = ui_model->m_instance_motions.find(ui_controller->m_tree_view_selected_instance_motion);

        if (ui_model->m_instance_motions.end() != found_instance_motion)
        {
            ImGui::SameLine();

            ImGui::BeginGroup();

            {
                ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                ImGui::TextUnformatted(found_instance_motion->second.m_name.c_str());
                ImGui::PopStyleColor();
            }

            ImGui::Separator();
            {
                mcrt_string timestamp_text = "Timestamp: ";
                mcrt_string directory_name;
                mcrt_string file_name;
                {
                    size_t const timestamp_text_end_pos = found_instance_motion->second.m_asset_motion.find(' ');
                    size_t const directory_name_end_pos = found_instance_motion->second.m_asset_motion.find_last_of("/\\");
                    if ((mcrt_string::npos != timestamp_text_end_pos) && (mcrt_string::npos != directory_name_end_pos) && (timestamp_text_end_pos < directory_name_end_pos))
                    {
                        timestamp_text += found_instance_motion->second.m_asset_motion.substr(0U, timestamp_text_end_pos);
                        directory_name = found_instance_motion->second.m_asset_motion.substr(timestamp_text_end_pos + 1U, ((directory_name_end_pos - timestamp_text_end_pos) - 1U));
                        file_name = found_instance_motion->second.m_asset_motion.substr(directory_name_end_pos + 1U);
                    }
                    else
                    {
                        assert(false);
                        timestamp_text = "N/A";
                        directory_name = "N/A";
                        file_name = "N/A";
                    }
                }

                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                    ImGui::TextUnformatted(file_name.c_str());
                    ImGui::PopStyleColor();
                }

                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                    ImGui::TextUnformatted(directory_name.c_str());
                    ImGui::PopStyleColor();
                }

                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                    ImGui::TextUnformatted(timestamp_text.c_str());
                    ImGui::PopStyleColor();
                }
            }

            {
                mcrt_string animation_index_text;
                {
                    constexpr char const *const text1[LANGUAGE_COUNT] = {
                        "Animation Index: ",
                        "Animation 索引: ",
                        "動畫索引: ",
                        "动画索引: "};

                    animation_index_text += text1[ui_controller->m_language_index];

                    char text2[] = {"18446744073709551615"};
                    std::snprintf(text2, sizeof(text2) / sizeof(text2[0]), "%llu", static_cast<long long unsigned>(found_instance_motion->second.m_animation_index));
                    text2[(sizeof(text2) / sizeof(text2[0])) - 1] = '\0';

                    animation_index_text += text2;
                }

                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                    ImGui::TextUnformatted(animation_index_text.c_str());
                    ImGui::PopStyleColor();
                }
            }

            ImGui::Separator();

            if (ImGui::BeginTable("##Instance-Motion-Manager-Right-Group-Table", 2, ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersInnerV))
            {
                ImGui::TableSetupColumn("##Instance-Motion-Manager-Right-Group-Table-Property", ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableSetupColumn("##Instance-Motion-Manager-Right-Group-Table-Value", ImGuiTableColumnFlags_WidthStretch);

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Frame Index",
                        "帧索引",
                        "帧索引",
                        "帧索引"};
                    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                    ImGui::PopStyleColor();
                }
                ImGui::TableNextColumn();
                {
                    assert(NULL != found_instance_motion->second.m_animation_instance);

                    char frame_index_text[] = {"18446744073709551615"};
                    std::snprintf(frame_index_text, sizeof(frame_index_text) / sizeof(frame_index_text[0]), "%llu", static_cast<long long unsigned>(found_instance_motion->second.m_animation_instance->get_frame_index()));
                    frame_index_text[(sizeof(frame_index_text) / sizeof(frame_index_text[0])) - 1] = '\0';

                    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                    ImGui::TextUnformatted(frame_index_text);
                    ImGui::PopStyleColor();
                }

                ImGui::EndTable();
            }

            ImGui::EndGroup();
        }

        ImGui::End();
    }

    if (ui_controller->m_show_instance_model_manager)
    {
        {
            ImGuiViewport const *const main_viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + ui_x * ui_index, main_viewport->WorkPos.y + ui_y * ui_index), ImGuiCond_FirstUseEver);
            ++ui_index;

            ImGui::SetNextWindowSize(ImVec2(ui_width, ui_height), ImGuiCond_FirstUseEver);
        }

        ImGui::Begin("Instance Model Manager", &ui_controller->m_show_instance_model_manager);

        {
            constexpr char const *const text[LANGUAGE_COUNT] = {
                "Instance Model Manager",
                "実例型式管理",
                "實例模型管理",
                "实例模型管理"};
            ImGui::TextUnformatted(text[ui_controller->m_language_index]);
        }

        ImGui::Separator();

        if (ImGui::TreeNodeEx("##Instance-Model-Manager-New-Node", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_NoAutoOpenOnLog))
        {
            if (ImGui::BeginTable("##Instance-Model-Manager-New-Table", 2, ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersInnerV))
            {
                ImGui::TableSetupColumn("##Instance-Model-Manager-New-Table-Property", ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableSetupColumn("##Instance-Model-Manager-New-Table-Value", ImGuiTableColumnFlags_WidthStretch);

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Name",
                        "名称",
                        "名稱",
                        "名称"};
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {
                    constexpr char const *const hint[LANGUAGE_COUNT] = {
                        "New Instance Model Name",
                        "新規実例型式名前",
                        "新建實例模型名稱",
                        "新建实例模型名称"};
                    ImGui::InputTextWithHint("##Instance-Model-Manager-New-Instance-Model-Name", hint[ui_controller->m_language_index], ui_controller->m_new_instance_model_name.data(), ui_controller->m_new_instance_model_name.size());
                }

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Asset Model",
                        "資源型式",
                        "資源模型",
                        "资源模型"};
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {
                    constexpr char const *const label = "##Instance-Model-Manager-Select-Asset-Model";

                    if (!ui_model->m_asset_models.empty())
                    {
                        mcrt_vector<mcrt_string> item_strings(static_cast<size_t>(ui_model->m_asset_models.size()));
                        mcrt_vector<char const *> items(static_cast<size_t>(ui_model->m_asset_models.size()));
                        int selected_combo_index = 0;
                        {
                            size_t combo_index = 0U;
                            for (auto const &asset_model : ui_model->m_asset_models)
                            {
                                item_strings[combo_index] = asset_model.first;
                                items[combo_index] = item_strings[combo_index].c_str();

                                if (ui_controller->m_new_instance_model_selected_asset_model == asset_model.first)
                                {
                                    assert(0 == selected_combo_index);
                                    assert((1U + combo_index) < static_cast<size_t>(INT_MAX));
                                    selected_combo_index = static_cast<int>(combo_index);
                                }

                                ++combo_index;
                            }
                        }

                        ImGui::Combo(label, &selected_combo_index, items.data(), items.size());

                        if (selected_combo_index >= 0)
                        {
                            ui_controller->m_new_instance_model_selected_asset_model = std::move(item_strings[selected_combo_index]);
                        }
                        else
                        {
                            assert(false);
                        }
                    }
                    else
                    {
                        char const *const items[LANGUAGE_COUNT][1] = {
                            {"Disable"},
                            {"無効"},
                            {"停用"},
                            {"停用"}};

                        int selected_combo_index = 0;

                        ImGui::Combo(label, &selected_combo_index, items[ui_controller->m_language_index], IM_ARRAYSIZE(items[ui_controller->m_language_index]));

                        assert(0 == selected_combo_index);

                        ui_controller->m_new_instance_model_selected_asset_model.clear();
                    }
                }

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Surface Group Index",
                        "表面 Group 索引",
                        "表面組索引",
                        "表面组索引"};

                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();

                {
                    auto const found_asset_model = ui_model->m_asset_models.find(ui_controller->m_new_instance_model_selected_asset_model);

                    constexpr char const *const label = "##Instance-Model-Manager-Select-Surface-Group-Index";

                    if (ui_model->m_asset_models.end() != found_asset_model)
                    {
                        assert(!found_asset_model->second.m_surface_groups.empty());
                        assert(!found_asset_model->second.m_skeletons.empty());
                        uint32_t const surface_group_count = static_cast<uint32_t>(found_asset_model->second.m_surface_groups.size());
                        assert(surface_group_count == found_asset_model->second.m_skeletons.size());

                        if (surface_group_count > 0U)
                        {
                            mcrt_vector<uint32_t> item_values(static_cast<size_t>(surface_group_count));
                            mcrt_vector<mcrt_string> item_strings(static_cast<size_t>(surface_group_count));
                            mcrt_vector<char const *> items(static_cast<size_t>(surface_group_count));
                            int selected_combo_index = 0;
                            {
                                uint32_t combo_index = 0U;

                                for (uint32_t surface_group_index = 0U; surface_group_index < surface_group_count; ++surface_group_index)
                                {
                                    item_values[combo_index] = surface_group_index;

                                    char item_value_text[] = {"18446744073709551615"};
                                    std::snprintf(item_value_text, sizeof(item_value_text) / sizeof(item_value_text[0]), "%llu", static_cast<long long unsigned>(item_values[combo_index]));
                                    item_value_text[(sizeof(item_value_text) / sizeof(item_value_text[0])) - 1] = '\0';

                                    item_strings[combo_index] = item_value_text;
                                    items[combo_index] = item_strings[combo_index].c_str();

                                    if (ui_controller->m_new_instance_model_selected_surface_group_index == item_values[combo_index])
                                    {
                                        assert(0 == selected_combo_index);
                                        assert(static_cast<size_t>(1U + combo_index) < static_cast<size_t>(INT_MAX));
                                        selected_combo_index = static_cast<int>(combo_index);
                                    }

                                    ++combo_index;
                                }
                            }

                            ImGui::Combo(label, &selected_combo_index, items.data(), items.size());

                            if (selected_combo_index >= 0)
                            {
                                ui_controller->m_new_instance_model_selected_surface_group_index = item_values[selected_combo_index];
                            }
                            else
                            {
                                assert(false);
                            }
                        }
                        else
                        {
                            assert(false);

                            char const *const items[LANGUAGE_COUNT][1] = {
                                {"Disable"},
                                {"無効"},
                                {"停用"},
                                {"停用"}};

                            int selected_combo_index = 0;

                            ImGui::Combo(label, &selected_combo_index, items[ui_controller->m_language_index], IM_ARRAYSIZE(items[ui_controller->m_language_index]));

                            assert(0 == selected_combo_index);

                            ui_controller->m_new_instance_model_selected_surface_group_index = INVALID_UINT32_INDEX;
                        }
                    }
                    else
                    {
                        char const *const items[LANGUAGE_COUNT][1] = {
                            {"Disable"},
                            {"無効"},
                            {"停用"},
                            {"停用"}};

                        int selected_combo_index = 0;

                        ImGui::Combo(label, &selected_combo_index, items[ui_controller->m_language_index], IM_ARRAYSIZE(items[ui_controller->m_language_index]));

                        assert(0 == selected_combo_index);

                        ui_controller->m_new_instance_model_selected_surface_group_index = INVALID_UINT32_INDEX;
                    }
                }

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Animation Input Type",
                        "動作入力種類",
                        "動畫輸入類型",
                        "动画输入类型"};

                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {
                    constexpr char const *const items[LANGUAGE_COUNT][ANIMATION_INPUT_TYPE_COUNT] = {
                        {"Disable", "Video Detector", "Instance Motion"},
                        {"無効", "動画検出", "実例行動"},
                        {"停用", "視訊檢測", "實例動作"},
                        {"停用", "视频检测", "实例动作"}};

                    int animation_input_type = std::min(std::max(ANIMATION_INPUT_TYPE_MIN, ui_controller->m_new_instance_model_animation_input_type), ANIMATION_INPUT_TYPE_MAX);

                    ImGui::Combo("##Instance-Model-Manager-Animation-Input-Type", &animation_input_type, items[ui_controller->m_language_index], IM_ARRAYSIZE(items[0]));

                    ui_controller->m_new_instance_model_animation_input_type = std::min(std::max(ANIMATION_INPUT_TYPE_MIN, static_cast<ANIMATION_INPUT_TYPE>(animation_input_type)), ANIMATION_INPUT_TYPE_MAX);
                }

                if (ANIMATION_INPUT_TYPE_VIDEO_DETECTOR == ui_controller->m_new_instance_model_animation_input_type)
                {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::AlignTextToFramePadding();
                    {
                        constexpr char const *const text[LANGUAGE_COUNT] = {
                            "Video Detector",
                            "動画検出",
                            "視訊檢測",
                            "视频检测"};

                        ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                    }
                    ImGui::TableNextColumn();
                    {
                        mcrt_vector<uint64_t> item_values(static_cast<size_t>(ui_model->m_video_detectors.size() + 1U));
                        mcrt_vector<mcrt_string> item_strings(static_cast<size_t>(ui_model->m_video_detectors.size() + 1U));
                        mcrt_vector<char const *> items(static_cast<size_t>(ui_model->m_video_detectors.size() + 1U));
                        int selected_combo_index = 0;
                        {
                            size_t combo_index = 0U;

                            {
                                item_values[combo_index] = INVALID_TIMESTAMP;

                                char const *const text[LANGUAGE_COUNT] = {
                                    "Disable",
                                    "無効",
                                    "停用",
                                    "停用"};

                                item_strings[combo_index] = text[ui_controller->m_language_index];

                                items[combo_index] = item_strings[combo_index].c_str();

                                if (ui_controller->m_new_instance_model_selected_video_detector == item_values[combo_index])
                                {
                                    assert(0 == selected_combo_index);
                                    assert(0 == combo_index);
                                }

                                ++combo_index;
                            }

                            for (auto const &video_detector : ui_model->m_video_detectors)
                            {
                                item_values[combo_index] = video_detector.first;

                                item_strings[combo_index].clear();
                                {
                                    char item_value_text[] = {"18446744073709551615"};
                                    std::snprintf(item_value_text, sizeof(item_value_text) / sizeof(item_value_text[0]), "%020llu", static_cast<long long unsigned>(item_values[combo_index]));
                                    item_value_text[(sizeof(item_value_text) / sizeof(item_value_text[0])) - 1] = '\0';

                                    item_strings[combo_index] += item_value_text;
                                    item_strings[combo_index] += ' ';
                                    item_strings[combo_index] += video_detector.second.m_name;
                                }

                                items[combo_index] = item_strings[combo_index].c_str();

                                if (ui_controller->m_new_instance_model_selected_video_detector == item_values[combo_index])
                                {
                                    assert(0 == selected_combo_index);
                                    assert((1U + combo_index) < static_cast<size_t>(INT_MAX));
                                    selected_combo_index = static_cast<int>(combo_index);
                                    assert(INVALID_TIMESTAMP == ui_controller->m_new_instance_model_selected_instance_motion);
                                }

                                ++combo_index;
                            }
                        }

                        ImGui::Combo("##Instance-Model-Manager-Select-Video-Detector", &selected_combo_index, items.data(), items.size());

                        if (selected_combo_index >= 0)
                        {
                            ui_controller->m_new_instance_model_selected_video_detector = item_values[selected_combo_index];
                            ui_controller->m_new_instance_model_selected_instance_motion = INVALID_TIMESTAMP;
                        }
                        else
                        {
                            assert(false);
                        }
                    }

                    {
                        auto const found_video_detector = ui_model->m_video_detectors.find(ui_controller->m_new_instance_model_selected_video_detector);

                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        ImGui::AlignTextToFramePadding();
                        {
                            constexpr char const *const text[LANGUAGE_COUNT] = {
                                "Face Index",
                                "人顔索引",
                                "人臉索引",
                                "人脸索引"};

                            ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                        }
                        ImGui::TableNextColumn();
                        {
                            constexpr char const *const label = "##Instance-Model-Manager-Selected-Video-Detector-Face-Index";

                            if (ui_model->m_video_detectors.end() != found_video_detector)
                            {
                                assert(NULL != found_video_detector->second.m_video_detector);

                                uint32_t const face_count = found_video_detector->second.m_video_detector->get_face_count();

                                if (face_count > 0U)
                                {
                                    mcrt_vector<uint32_t> item_values(static_cast<size_t>(face_count));
                                    mcrt_vector<mcrt_string> item_strings(static_cast<size_t>(face_count));
                                    mcrt_vector<char const *> items(static_cast<size_t>(face_count));
                                    int selected_combo_index = 0;
                                    {
                                        uint32_t combo_index = 0U;

                                        for (uint32_t face_index = 0U; face_index < face_count; ++face_index)
                                        {
                                            item_values[combo_index] = face_index;

                                            char item_value_text[] = {"18446744073709551615"};
                                            std::snprintf(item_value_text, sizeof(item_value_text) / sizeof(item_value_text[0]), "%llu", static_cast<long long unsigned>(item_values[combo_index]));
                                            item_value_text[(sizeof(item_value_text) / sizeof(item_value_text[0])) - 1] = '\0';

                                            item_strings[combo_index] = item_value_text;
                                            items[combo_index] = item_strings[combo_index].c_str();

                                            if (ui_controller->m_new_instance_model_selected_video_detector_face_index == item_values[combo_index])
                                            {
                                                assert(0 == selected_combo_index);
                                                assert(static_cast<size_t>(1U + combo_index) < static_cast<size_t>(INT_MAX));
                                                selected_combo_index = static_cast<int>(combo_index);
                                            }

                                            ++combo_index;
                                        }
                                    }

                                    ImGui::Combo(label, &selected_combo_index, items.data(), items.size());

                                    if (selected_combo_index >= 0)
                                    {
                                        ui_controller->m_new_instance_model_selected_video_detector_face_index = item_values[selected_combo_index];
                                    }
                                    else
                                    {
                                        assert(false);
                                    }
                                }
                                else
                                {
                                    char const *const items[LANGUAGE_COUNT][1] = {
                                        {"Disable"},
                                        {"無効"},
                                        {"停用"},
                                        {"停用"}};

                                    int selected_combo_index = 0;

                                    ImGui::Combo(label, &selected_combo_index, items[ui_controller->m_language_index], IM_ARRAYSIZE(items[ui_controller->m_language_index]));

                                    assert(0 == selected_combo_index);

                                    ui_controller->m_new_instance_model_selected_video_detector_face_index = BRX_MOTION_UINT32_INDEX_INVALID;
                                }
                            }
                            else
                            {
                                char const *const items[LANGUAGE_COUNT][1] = {
                                    {"Disable"},
                                    {"無効"},
                                    {"停用"},
                                    {"停用"}};

                                int selected_combo_index = 0;

                                ImGui::Combo(label, &selected_combo_index, items[ui_controller->m_language_index], IM_ARRAYSIZE(items[ui_controller->m_language_index]));

                                assert(0 == selected_combo_index);

                                ui_controller->m_new_instance_model_selected_video_detector_face_index = BRX_MOTION_UINT32_INDEX_INVALID;
                            }
                        }

                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        ImGui::AlignTextToFramePadding();
                        {
                            constexpr char const *const text[LANGUAGE_COUNT] = {
                                "Pose Index",
                                "姿勢索引",
                                "姿勢索引",
                                "姿勢索引"};

                            ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                        }
                        ImGui::TableNextColumn();
                        {
                            constexpr char const *const label = "##Instance-Model-Manager-Selected-Video-Detector-Pose-Index";

                            if (ui_model->m_video_detectors.end() != found_video_detector)
                            {
                                assert(NULL != found_video_detector->second.m_video_detector);

                                uint32_t const pose_count = found_video_detector->second.m_video_detector->get_pose_count();

                                if (pose_count > 0U)
                                {
                                    mcrt_vector<uint32_t> item_values(static_cast<size_t>(pose_count));
                                    mcrt_vector<mcrt_string> item_strings(static_cast<size_t>(pose_count));
                                    mcrt_vector<char const *> items(static_cast<size_t>(pose_count));
                                    int selected_combo_index = 0;
                                    {
                                        uint32_t combo_index = 0U;

                                        for (uint32_t pose_index = 0U; pose_index < pose_count; ++pose_index)
                                        {
                                            item_values[combo_index] = pose_index;

                                            char item_value_text[] = {"18446744073709551615"};
                                            std::snprintf(item_value_text, sizeof(item_value_text) / sizeof(item_value_text[0]), "%llu", static_cast<long long unsigned>(item_values[combo_index]));
                                            item_value_text[(sizeof(item_value_text) / sizeof(item_value_text[0])) - 1] = '\0';

                                            item_strings[combo_index] = item_value_text;
                                            items[combo_index] = item_strings[combo_index].c_str();

                                            if (ui_controller->m_new_instance_model_selected_video_detector_pose_index == item_values[combo_index])
                                            {
                                                assert(0 == selected_combo_index);
                                                assert(static_cast<size_t>(1U + combo_index) < static_cast<size_t>(INT_MAX));
                                                selected_combo_index = static_cast<int>(combo_index);
                                            }

                                            ++combo_index;
                                        }
                                    }

                                    ImGui::Combo(label, &selected_combo_index, items.data(), items.size());

                                    if (selected_combo_index >= 0)
                                    {
                                        ui_controller->m_new_instance_model_selected_video_detector_pose_index = item_values[selected_combo_index];
                                    }
                                    else
                                    {
                                        assert(false);
                                    }
                                }
                                else
                                {

                                    char const *const items[LANGUAGE_COUNT][1] = {
                                        {"Disable"},
                                        {"無効"},
                                        {"停用"},
                                        {"停用"}};

                                    int selected_combo_index = 0;

                                    ImGui::Combo(label, &selected_combo_index, items[ui_controller->m_language_index], IM_ARRAYSIZE(items[ui_controller->m_language_index]));

                                    assert(0 == selected_combo_index);

                                    ui_controller->m_new_instance_model_selected_video_detector_pose_index = BRX_MOTION_UINT32_INDEX_INVALID;
                                }
                            }
                            else
                            {

                                char const *const items[LANGUAGE_COUNT][1] = {
                                    {"Disable"},
                                    {"無効"},
                                    {"停用"},
                                    {"停用"}};

                                int selected_combo_index = 0;

                                ImGui::Combo(label, &selected_combo_index, items[ui_controller->m_language_index], IM_ARRAYSIZE(items[ui_controller->m_language_index]));

                                assert(0 == selected_combo_index);

                                ui_controller->m_new_instance_model_selected_video_detector_pose_index = BRX_MOTION_UINT32_INDEX_INVALID;
                            }
                        }
                    }
                }
                else if (ANIMATION_INPUT_TYPE_INSTANCE_MOTION == ui_controller->m_new_instance_model_animation_input_type)
                {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::AlignTextToFramePadding();
                    {
                        constexpr char const *const text[LANGUAGE_COUNT] = {
                            "Instance Motion",
                            "実例行動",
                            "實例動作",
                            "实例动作"};

                        ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                    }
                    ImGui::TableNextColumn();
                    {
                        mcrt_vector<uint64_t> item_values(static_cast<size_t>(ui_model->m_instance_motions.size() + 1U));
                        mcrt_vector<mcrt_string> item_strings(static_cast<size_t>(ui_model->m_instance_motions.size() + 1U));
                        mcrt_vector<char const *> items(static_cast<size_t>(ui_model->m_instance_motions.size() + 1U));
                        int selected_combo_index = 0;
                        {
                            size_t combo_index = 0U;

                            {
                                item_values[combo_index] = INVALID_TIMESTAMP;

                                char const *const text[LANGUAGE_COUNT] = {
                                    "Disable",
                                    "無効",
                                    "停用",
                                    "停用"};

                                item_strings[combo_index] = text[ui_controller->m_language_index];

                                items[combo_index] = item_strings[combo_index].c_str();

                                if (ui_controller->m_new_instance_model_selected_instance_motion == item_values[combo_index])
                                {
                                    assert(0 == selected_combo_index);
                                    assert(0 == combo_index);
                                }

                                ++combo_index;
                            }

                            for (auto const &instance_motion : ui_model->m_instance_motions)
                            {
                                item_values[combo_index] = instance_motion.first;

                                item_strings[combo_index].clear();
                                {
                                    char item_value_text[] = {"18446744073709551615"};
                                    std::snprintf(item_value_text, sizeof(item_value_text) / sizeof(item_value_text[0]), "%020llu", static_cast<long long unsigned>(item_values[combo_index]));
                                    item_value_text[(sizeof(item_value_text) / sizeof(item_value_text[0])) - 1] = '\0';

                                    item_strings[combo_index] += item_value_text;
                                    item_strings[combo_index] += ' ';
                                    item_strings[combo_index] += instance_motion.second.m_name;
                                }

                                items[combo_index] = item_strings[combo_index].c_str();

                                if (ui_controller->m_new_instance_model_selected_instance_motion == item_values[combo_index])
                                {
                                    assert(0 == selected_combo_index);
                                    assert((1U + combo_index) < static_cast<size_t>(INT_MAX));
                                    selected_combo_index = static_cast<int>(combo_index);
                                    assert(INVALID_TIMESTAMP == ui_controller->m_new_instance_model_selected_video_detector);
                                }

                                ++combo_index;
                            }
                        }

                        ImGui::Combo("##Instance-Model-Manager-Select-Instance-Motion", &selected_combo_index, items.data(), items.size());

                        if (selected_combo_index >= 0)
                        {
                            ui_controller->m_new_instance_model_selected_instance_motion = item_values[selected_combo_index];
                            ui_controller->m_new_instance_model_selected_video_detector = INVALID_TIMESTAMP;
                        }
                        else
                        {
                            assert(false);
                        }
                    }
                }
                else
                {
                    assert(ANIMATION_INPUT_TYPE_INVALID == ui_controller->m_new_instance_model_animation_input_type);
                    ui_controller->m_new_instance_model_selected_video_detector = INVALID_TIMESTAMP;
                    ui_controller->m_new_instance_model_selected_instance_motion = INVALID_TIMESTAMP;
                }

                {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::AlignTextToFramePadding();
                    {
                        constexpr char const *const text[LANGUAGE_COUNT] = {
                            "Rotation Euler Roll",
                            "回転 Euler Roll",
                            "旋轉 Euler Roll",
                            "旋转 Euler Roll"};
                        ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                    }
                    ImGui::TableNextColumn();
                    {

                        float rotation = std::min(std::max(-180.0F, DirectX::XMConvertToDegrees(ui_controller->m_new_instance_model_transform_rotation_roll)), 180.0F);
#define _INTERNAL_BRX_STRINGIZING(string) #string
#define _INTERNAL_BRX_X_STRINGIZING(string) _INTERNAL_BRX_STRINGIZING(string)
                        ImGui::SliderFloat("##Instance-Model-Manager-Model-Transform-Rotation-Euler-Roll-Slider", &rotation, -180.0F, 180.0F, "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f");

                        // ImGui::SameLine();

                        // ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * table_column_same_line_ratio);

                        char rotation_text[256];
                        std::snprintf(&rotation_text[0], sizeof(rotation_text) / sizeof(rotation_text[0]), "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f", rotation);
#undef _INTERNAL_BRX_STRINGIZING
#undef _INTERNAL_BRX_X_STRINGIZING
                        rotation_text[(sizeof(rotation_text) / sizeof(rotation_text[0]) - 1)] = '\0';

                        ImGui::InputText("##Instance-Model-Manager-Model-Transform-Rotation-Euler-Roll-Text", &rotation_text[0], sizeof(rotation_text) / sizeof(rotation_text[0]), ImGuiInputTextFlags_CharsDecimal);

                        ui_controller->m_new_instance_model_transform_rotation_roll = DirectX::XMConvertToRadians(std::min(std::max(-180.0F, std::strtof(rotation_text, NULL)), 180.0F));
                    }

                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::AlignTextToFramePadding();
                    {
                        constexpr char const *const text[LANGUAGE_COUNT] = {
                            "Rotation Euler Pitch",
                            "回転 Euler Pitch",
                            "旋轉 Euler Pitch",
                            "旋转 Euler Pitch"};
                        ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                    }
                    ImGui::TableNextColumn();
                    {
                        float rotation = std::min(std::max(-180.0F, DirectX::XMConvertToDegrees(ui_controller->m_new_instance_model_transform_rotation_pitch)), 180.0F);
#define _INTERNAL_BRX_STRINGIZING(string) #string
#define _INTERNAL_BRX_X_STRINGIZING(string) _INTERNAL_BRX_STRINGIZING(string)
                        ImGui::SliderFloat("##Instance-Model-Manager-Model-Transform-Rotation-Euler-Pitch-Slider", &rotation, -180.0F, 180.0F, "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f");

                        // ImGui::SameLine();

                        // ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * table_column_same_line_ratio);

                        char rotation_text[256];
                        std::snprintf(&rotation_text[0], sizeof(rotation_text) / sizeof(rotation_text[0]), "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f", rotation);
#undef _INTERNAL_BRX_STRINGIZING
#undef _INTERNAL_BRX_X_STRINGIZING
                        rotation_text[(sizeof(rotation_text) / sizeof(rotation_text[0]) - 1)] = '\0';

                        ImGui::InputText("##Instance-Model-Manager-Model-Transform-Rotation-Euler-Pitch-Text", &rotation_text[0], sizeof(rotation_text) / sizeof(rotation_text[0]), ImGuiInputTextFlags_CharsDecimal);

                        ui_controller->m_new_instance_model_transform_rotation_pitch = DirectX::XMConvertToRadians(std::min(std::max(-180.0F, std::strtof(rotation_text, NULL)), 180.0F));
                    }

                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::AlignTextToFramePadding();
                    {
                        constexpr char const *const text[LANGUAGE_COUNT] = {
                            "Rotation Euler Yaw",
                            "回転 Euler Yaw",
                            "旋轉 Euler Yaw",
                            "旋转 Euler Yaw"};
                        ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                    }
                    ImGui::TableNextColumn();
                    {
                        float rotation = std::min(std::max(-180.0F, DirectX::XMConvertToDegrees(ui_controller->m_new_instance_model_transform_rotation_yaw)), 180.0F);
#define _INTERNAL_BRX_STRINGIZING(string) #string
#define _INTERNAL_BRX_X_STRINGIZING(string) _INTERNAL_BRX_STRINGIZING(string)
                        ImGui::SliderFloat("##Instance-Model-Manager-Model-Transform-Rotation-Euler-Yaw-Slider", &rotation, -180.0F, 180.0F, "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f");

                        // ImGui::SameLine();

                        // ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * table_column_same_line_ratio);

                        char rotation_text[256];
                        std::snprintf(&rotation_text[0], sizeof(rotation_text) / sizeof(rotation_text[0]), "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f", rotation);
#undef _INTERNAL_BRX_STRINGIZING
#undef _INTERNAL_BRX_X_STRINGIZING
                        rotation_text[(sizeof(rotation_text) / sizeof(rotation_text[0]) - 1)] = '\0';

                        ImGui::InputText("##Instance-Model-Manager-Model-Transform-Rotation-Euler-Yaw-Text", &rotation_text[0], sizeof(rotation_text) / sizeof(rotation_text[0]), ImGuiInputTextFlags_CharsDecimal);

                        ui_controller->m_new_instance_model_transform_rotation_yaw = DirectX::XMConvertToRadians(std::min(std::max(-180.0F, std::strtof(rotation_text, NULL)), 180.0F));
                    }
                }

                {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::AlignTextToFramePadding();
                    {
                        constexpr char const *const text[LANGUAGE_COUNT] = {
                            "Translation X",
                            "平行移動 X",
                            "平移 X",
                            "平移 X"};
                        ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                    }
                    ImGui::TableNextColumn();
                    {
                        char translation_text[256];
#define _INTERNAL_BRX_STRINGIZING(string) #string
#define _INTERNAL_BRX_X_STRINGIZING(string) _INTERNAL_BRX_STRINGIZING(string)
                        std::snprintf(&translation_text[0], sizeof(translation_text) / sizeof(translation_text[0]), "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f", ui_controller->m_new_instance_model_transform_translation_x);
#undef _INTERNAL_BRX_STRINGIZING
#undef _INTERNAL_BRX_X_STRINGIZING
                        translation_text[(sizeof(translation_text) / sizeof(translation_text[0]) - 1)] = '\0';

                        ImGui::InputText("##Instance-Model-Manager-Model-Transform-Translation-X", &translation_text[0], sizeof(translation_text) / sizeof(translation_text[0]), ImGuiInputTextFlags_CharsDecimal);

                        ui_controller->m_new_instance_model_transform_translation_x = std::strtof(translation_text, NULL);
                    }

                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::AlignTextToFramePadding();
                    {
                        constexpr char const *const text[LANGUAGE_COUNT] = {
                            "Translation Y",
                            "平行移動 Y",
                            "平移 Y",
                            "平移 Y"};
                        ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                    }
                    ImGui::TableNextColumn();
                    {
                        char translation_text[256];
#define _INTERNAL_BRX_STRINGIZING(string) #string
#define _INTERNAL_BRX_X_STRINGIZING(string) _INTERNAL_BRX_STRINGIZING(string)
                        std::snprintf(&translation_text[0], sizeof(translation_text) / sizeof(translation_text[0]), "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f", ui_controller->m_new_instance_model_transform_translation_y);
#undef _INTERNAL_BRX_STRINGIZING
#undef _INTERNAL_BRX_X_STRINGIZING
                        translation_text[(sizeof(translation_text) / sizeof(translation_text[0]) - 1)] = '\0';

                        ImGui::InputText("##Instance-Model-Manager-Model-Transform-Translation-Y", &translation_text[0], sizeof(translation_text) / sizeof(translation_text[0]), ImGuiInputTextFlags_CharsDecimal);

                        ui_controller->m_new_instance_model_transform_translation_y = std::strtof(translation_text, NULL);
                    }

                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::AlignTextToFramePadding();
                    {
                        constexpr char const *const text[LANGUAGE_COUNT] = {
                            "Translation Z",
                            "平行移動 Z",
                            "平移 Z",
                            "平移 Z"};
                        ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                    }
                    ImGui::TableNextColumn();
                    {
                        char translation_text[256];
#define _INTERNAL_BRX_STRINGIZING(string) #string
#define _INTERNAL_BRX_X_STRINGIZING(string) _INTERNAL_BRX_STRINGIZING(string)
                        std::snprintf(&translation_text[0], sizeof(translation_text) / sizeof(translation_text[0]), "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f", ui_controller->m_new_instance_model_transform_translation_z);
#undef _INTERNAL_BRX_STRINGIZING
#undef _INTERNAL_BRX_X_STRINGIZING
                        translation_text[(sizeof(translation_text) / sizeof(translation_text[0]) - 1)] = '\0';

                        ImGui::InputText("##Instance-Model-Manager-Model-Transform-Translation-Z", &translation_text[0], sizeof(translation_text) / sizeof(translation_text[0]), ImGuiInputTextFlags_CharsDecimal);

                        ui_controller->m_new_instance_model_transform_translation_z = std::strtof(translation_text, NULL);
                    }
                }

                ImGui::EndTable();
            }

            ImGui::TreePop();
        }

        {
            {
                constexpr char const *const text[LANGUAGE_COUNT] = {
                    "New",
                    "新規作成",
                    "新建",
                    "新建"};
                ImGui::TextUnformatted(text[ui_controller->m_language_index]);
            }

            ImGui::SameLine();

            if (ImGui::Button("N##Instance-Model-Manager-New-Button"))
            {
                mcrt_string const asset_model = ui_controller->m_new_instance_model_selected_asset_model;

                uint32_t const surface_group_index = ui_controller->m_new_instance_model_selected_surface_group_index;

                auto const found_asset_model = ui_model->m_asset_models.find(asset_model);

                if ((ui_model->m_asset_models.end() != found_asset_model) && (surface_group_index < found_asset_model->second.m_surface_groups.size()) && (surface_group_index < found_asset_model->second.m_skeletons.size()))
                {
                    assert(found_asset_model->second.m_surface_groups.size() == found_asset_model->second.m_skeletons.size());

                    brx_anari_surface_group *const surface_group = found_asset_model->second.m_surface_groups[surface_group_index];

                    brx_motion_skeleton *const skeleton = found_asset_model->second.m_skeletons[surface_group_index];

                    if (NULL != surface_group)
                    {
                        brx_anari_surface_group_instance *const surface_group_instance = device->world_new_surface_group_instance(surface_group);

                        brx_motion_skeleton_instance *const skeleton_instance = ((NULL != skeleton) ? brx_motion_create_skeleton_instance(skeleton) : NULL);

                        if ((NULL != surface_group_instance) && ((!skeleton) || (NULL != skeleton_instance)))
                        {
                            ANIMATION_INPUT_TYPE animation_input_type = ANIMATION_INPUT_TYPE_INVALID;
                            uint64_t video_detector = INVALID_TIMESTAMP;
                            uint64_t instance_motion = INVALID_TIMESTAMP;

                            if ((NULL != skeleton) && (NULL != skeleton_instance))
                            {
                                animation_input_type = ui_controller->m_new_instance_model_animation_input_type;

                                video_detector = ui_controller->m_new_instance_model_selected_video_detector;

                                uint32_t const video_detector_face_index = ui_controller->m_new_instance_model_selected_video_detector_face_index;

                                uint32_t const video_detector_pose_index = ui_controller->m_new_instance_model_selected_video_detector_pose_index;

                                instance_motion = ui_controller->m_new_instance_model_selected_instance_motion;

                                auto const found_video_detector = ui_model->m_video_detectors.find(video_detector);

                                auto const found_instance_motion = ui_model->m_instance_motions.find(instance_motion);

                                assert((ui_model->m_video_detectors.end() == found_video_detector) || (ui_model->m_instance_motions.end() == found_instance_motion));

                                if (ui_model->m_video_detectors.end() != found_video_detector)
                                {
                                    assert(NULL != found_video_detector->second.m_video_detector);

                                    skeleton_instance->set_input_video_detector(found_video_detector->second.m_video_detector);
                                    skeleton_instance->set_input_video_detector_face_index(video_detector_face_index);
                                    skeleton_instance->set_input_video_detector_pose_index(video_detector_pose_index);

                                    assert(INVALID_TIMESTAMP == instance_motion);

                                    assert(ANIMATION_INPUT_TYPE_VIDEO_DETECTOR == animation_input_type);
                                }
                                else if (ui_model->m_instance_motions.end() != found_instance_motion)
                                {
                                    assert(NULL != found_instance_motion->second.m_animation_instance);

                                    skeleton_instance->set_input_animation_instance(found_instance_motion->second.m_animation_instance);

                                    assert(INVALID_TIMESTAMP == video_detector);

                                    assert(ANIMATION_INPUT_TYPE_INSTANCE_MOTION == animation_input_type);
                                }
                                else
                                {
                                    for (uint32_t morph_target_name_index = 0U; morph_target_name_index < BRX_ANARI_MORPH_TARGET_NAME_MMD_COUNT; ++morph_target_name_index)
                                    {
                                        BRX_ANARI_MORPH_TARGET_NAME const morph_target_name = static_cast<BRX_ANARI_MORPH_TARGET_NAME>(morph_target_name_index);

                                        surface_group_instance->set_morph_target_weight(morph_target_name, 0.0F);
                                    }

                                    {
                                        mcrt_vector<brx_anari_rigid_transform> const skin_transforms(static_cast<size_t>(skeleton_instance->get_skin_transform_count()), brx_anari_rigid_transform{{0.0F, 0.0F, 0.0F, 1.0F}, {0.0F, 0.0F, 0.0F}});

                                        surface_group_instance->set_skin_transforms(skeleton_instance->get_skin_transform_count(), skin_transforms.data());
                                    }

                                    assert(ANIMATION_INPUT_TYPE_INVALID == animation_input_type);
                                }
                            }
                            else
                            {
                                assert(NULL == skeleton);
                                assert(NULL == skeleton_instance);

                                assert(ANIMATION_INPUT_TYPE_INVALID == animation_input_type);
                                assert(INVALID_TIMESTAMP == video_detector);
                                assert(INVALID_TIMESTAMP == instance_motion);
                            }

                            float const model_transform_rotation_roll = ui_controller->m_new_instance_model_transform_rotation_roll;
                            float const model_transform_rotation_pitch = ui_controller->m_new_instance_model_transform_rotation_pitch;
                            float const model_transform_rotation_yaw = ui_controller->m_new_instance_model_transform_rotation_yaw;

                            float const model_transform_translation_x = ui_controller->m_new_instance_model_transform_translation_x;
                            float const model_transform_translation_y = ui_controller->m_new_instance_model_transform_translation_y;
                            float const model_transform_translation_z = ui_controller->m_new_instance_model_transform_translation_z;

                            {
                                DirectX::XMFLOAT4 quaternion;
                                DirectX::XMStoreFloat4(&quaternion, DirectX::XMQuaternionRotationRollPitchYaw(model_transform_rotation_pitch, model_transform_rotation_yaw, model_transform_rotation_roll));

                                brx_anari_rigid_transform const model_transform{{quaternion.x, quaternion.y, quaternion.z, quaternion.w}, {model_transform_translation_x, model_transform_translation_y, model_transform_translation_z}};
                                surface_group_instance->set_model_transform(model_transform);
                            }

                            uint64_t const timestamp = mcrt_tick_count_now();

                            // should always NOT already exist in practice
                            assert(ui_model->m_instance_models.end() == ui_model->m_instance_models.find(timestamp));

                            ui_model->m_instance_models.insert(ui_model->m_instance_models.end(), std::pair<uint64_t, ui_instance_model_model_t>{timestamp, ui_instance_model_model_t{ui_controller->m_new_instance_model_name.data(), asset_model, surface_group_index, surface_group_instance, skeleton_instance, video_detector, instance_motion, model_transform_rotation_roll, model_transform_rotation_pitch, model_transform_rotation_yaw, model_transform_translation_x, model_transform_translation_y, model_transform_translation_z}});

                            // should always NOT already exist in practice
                            assert(ui_controller->m_instance_controllers.end() == ui_controller->m_instance_controllers.find(timestamp));
                            ui_controller->m_instance_controllers.insert(ui_controller->m_instance_controllers.end(), std::pair<uint64_t, ui_instance_model_controller_t>{timestamp, ui_instance_model_controller_t{animation_input_type}});
                        }
                        else
                        {
                            assert(false);

                            if (NULL != surface_group_instance)
                            {
                                device->world_release_surface_group_instance(surface_group_instance);
                            }

                            if (NULL != skeleton_instance)
                            {
                                brx_motion_destroy_skeleton_instance(skeleton_instance);
                            }
                        }
                    }
                    else
                    {
                        assert(false);
                    }
                }
            }
        }

        ImGui::Separator();

        {

            {
                constexpr char const *const text[LANGUAGE_COUNT] = {
                    "Delete",
                    "削除",
                    "刪除",
                    "删除"};
                ImGui::TextUnformatted(text[ui_controller->m_language_index]);
            }

            ImGui::SameLine();

            if (ImGui::Button("X##Instance-Model-Manager-Delete-Button"))
            {
                auto const found_instance_model = ui_model->m_instance_models.find(ui_controller->m_tree_view_selected_instance_model);

                auto const found_instance_controller = ui_controller->m_instance_controllers.find(ui_controller->m_tree_view_selected_instance_model);

                assert((ui_model->m_instance_models.end() != found_instance_model) == (ui_controller->m_instance_controllers.end() != found_instance_controller));

                if ((ui_model->m_instance_models.end() != found_instance_model) || (ui_controller->m_instance_controllers.end() != found_instance_controller))
                {
                    if (ui_model->m_instance_models.end() != found_instance_model)
                    {
                        if (NULL != found_instance_model->second.m_surface_group_instance)
                        {
                            device->world_release_surface_group_instance(found_instance_model->second.m_surface_group_instance);
                            found_instance_model->second.m_surface_group_instance = NULL;
                        }
                        else
                        {
                            assert(false);
                        }

                        if (NULL != found_instance_model->second.m_skeleton_instance)
                        {
                            brx_motion_destroy_skeleton_instance(found_instance_model->second.m_skeleton_instance);
                            found_instance_model->second.m_skeleton_instance = NULL;
                        }
                        else
                        {
                            assert(INVALID_TIMESTAMP == found_instance_model->second.m_video_detector);
                            assert(INVALID_TIMESTAMP == found_instance_model->second.m_instance_motion);
                        }

                        ui_model->m_instance_models.erase(found_instance_model);
                    }

                    if (ui_controller->m_instance_controllers.end() != found_instance_controller)
                    {
                        ui_controller->m_instance_controllers.erase(found_instance_controller);
                    }

                    ui_controller->m_tree_view_selected_instance_model = INVALID_TIMESTAMP;
                }
                else
                {
                    assert(INVALID_TIMESTAMP == ui_controller->m_tree_view_selected_instance_model);
                }
            }
        }

        ImGui::Separator();

        if (ImGui::BeginChild("##Instance-Model-Manager-Left-Child", ImVec2(ui_width * left_child_width_ratio, 0.0F), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX))
        {
            ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::PushItemFlag(ImGuiItemFlags_NoNavDefaultFocus, true);
            ImGuiTextFilter text_filter;
            {
                constexpr char const *const hint[LANGUAGE_COUNT] = {
                    "Search",
                    "検索",
                    "檢索",
                    "检索"};
                if (ImGui::InputTextWithHint("##Instance-Model-Manager-Left-Child-Text-Filter", hint[ui_controller->m_language_index], text_filter.InputBuf, IM_ARRAYSIZE(text_filter.InputBuf), ImGuiInputTextFlags_EscapeClearsAll))
                {
                    text_filter.Build();
                }
            }

            ImGui::PopItemFlag();

            if (ImGui::BeginTable("##Instance-Model-Manager-Left-Child-Table", 1, ImGuiTableFlags_RowBg))
            {
                for (auto const &instance_model : ui_model->m_instance_models)
                {
                    mcrt_string instance_model_identity;
                    {
                        char instance_model_timestamp_text[] = {"18446744073709551615"};
                        std::snprintf(instance_model_timestamp_text, sizeof(instance_model_timestamp_text) / sizeof(instance_model_timestamp_text[0]), "%020llu", static_cast<long long unsigned>(instance_model.first));
                        instance_model_timestamp_text[(sizeof(instance_model_timestamp_text) / sizeof(instance_model_timestamp_text[0])) - 1] = '\0';

                        instance_model_identity += instance_model_timestamp_text;
                        instance_model_identity += ' ';
                        instance_model_identity += instance_model.second.m_name;
                    }

                    if (text_filter.PassFilter(instance_model_identity.c_str()))
                    {
                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();

                        ImGuiTreeNodeFlags const flags = ((ui_controller->m_tree_view_selected_instance_model != instance_model.first)) ? ImGuiTreeNodeFlags_Leaf : (ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Selected);

                        bool const node_open = ImGui::TreeNodeEx(instance_model_identity.c_str(), flags);

                        if (ImGui::IsItemFocused())
                        {
                            ui_controller->m_tree_view_selected_instance_model = instance_model.first;
                        }

                        if (node_open)
                        {
                            ImGui::TreePop();
                        }
                    }
                }

                ImGui::EndTable();
            }
        }
        ImGui::EndChild();

        auto const found_instance_model = ui_model->m_instance_models.find(ui_controller->m_tree_view_selected_instance_model);

        if (ui_model->m_instance_models.end() != found_instance_model)
        {
            ImGui::SameLine();

            ImGui::BeginGroup();

            {
                ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                ImGui::TextUnformatted(found_instance_model->second.m_name.c_str());
                ImGui::PopStyleColor();
            }

            ImGui::Separator();
            {
                mcrt_string timestamp_text = "Timestamp: ";
                mcrt_string directory_name;
                mcrt_string file_name;
                {
                    size_t const timestamp_text_end_pos = found_instance_model->second.m_asset_model.find(' ');
                    size_t const directory_name_end_pos = found_instance_model->second.m_asset_model.find_last_of("/\\");
                    if ((mcrt_string::npos != timestamp_text_end_pos) && (mcrt_string::npos != directory_name_end_pos) && (timestamp_text_end_pos < directory_name_end_pos))
                    {
                        timestamp_text += found_instance_model->second.m_asset_model.substr(0U, timestamp_text_end_pos);
                        directory_name = found_instance_model->second.m_asset_model.substr(timestamp_text_end_pos + 1U, ((directory_name_end_pos - timestamp_text_end_pos) - 1U));
                        file_name = found_instance_model->second.m_asset_model.substr(directory_name_end_pos + 1U);
                    }
                    else
                    {
                        assert(false);
                        timestamp_text = "N/A";
                        directory_name = "N/A";
                        file_name = "N/A";
                    }
                }

                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                    ImGui::TextUnformatted(file_name.c_str());
                    ImGui::PopStyleColor();
                }

                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                    ImGui::TextUnformatted(directory_name.c_str());
                    ImGui::PopStyleColor();
                }

                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                    ImGui::TextUnformatted(timestamp_text.c_str());
                    ImGui::PopStyleColor();
                }
            }

            {
                mcrt_string surface_group_index_text;
                {

                    constexpr char const *const text1[LANGUAGE_COUNT] = {
                        "Surface Group Index: ",
                        "表面 Group 索引: ",
                        "表面組索引: ",
                        "表面组索引: "};

                    surface_group_index_text += text1[ui_controller->m_language_index];

                    char text2[] = {"18446744073709551615"};
                    std::snprintf(text2, sizeof(text2) / sizeof(text2[0]), "%llu", static_cast<long long unsigned>(found_instance_model->second.m_surface_group_index));
                    text2[(sizeof(text2) / sizeof(text2[0])) - 1] = '\0';

                    surface_group_index_text += text2;
                }

                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                    ImGui::TextUnformatted(surface_group_index_text.c_str());
                    ImGui::PopStyleColor();
                }
            }

            ImGui::Separator();

            if (ImGui::BeginTable("##Instance-Model-Manager-Right-Group-Table", 2, ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersInnerV))
            {
                ImGui::TableSetupColumn("##Instance-Model-Manager-Right-Group-Table-Property", ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableSetupColumn("##Instance-Model-Manager-Right-Group-Table-Value", ImGuiTableColumnFlags_WidthStretch);

                if (NULL != found_instance_model->second.m_skeleton_instance)
                {
#ifndef NDEBUG
                    {
                        brx_motion_video_detector *const video_detector = found_instance_model->second.m_skeleton_instance->get_input_video_detector();

                        brx_motion_animation_instance *const animation_instance = found_instance_model->second.m_skeleton_instance->get_input_animation_instance();

                        assert((NULL == video_detector) || (NULL == animation_instance));

                        if (NULL != video_detector)
                        {
                            auto const found_video_detector = ui_model->m_video_detectors.find(found_instance_model->second.m_video_detector);

                            assert((ui_model->m_video_detectors.end() == found_video_detector) || (video_detector == found_video_detector->second.m_video_detector));

                            assert(INVALID_TIMESTAMP == found_instance_model->second.m_instance_motion);
                        }
                        else if (NULL != animation_instance)
                        {
                            auto const found_instance_motion = ui_model->m_instance_motions.find(found_instance_model->second.m_instance_motion);

                            assert((ui_model->m_instance_motions.end() == found_instance_motion) || (animation_instance == found_instance_motion->second.m_animation_instance));

                            assert(INVALID_TIMESTAMP == found_instance_model->second.m_video_detector);
                        }
                        else
                        {
                            assert(INVALID_TIMESTAMP == found_instance_model->second.m_video_detector);
                            assert(INVALID_TIMESTAMP == found_instance_model->second.m_instance_motion);
                        }
                    }
#endif

                    assert(ui_controller->m_instance_controllers.end() != ui_controller->m_instance_controllers.find(ui_controller->m_tree_view_selected_instance_model));
                    auto &found_instance_controller = ui_controller->m_instance_controllers[ui_controller->m_tree_view_selected_instance_model];

                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::AlignTextToFramePadding();
                    {
                        constexpr char const *const text[LANGUAGE_COUNT] = {
                            "Animation Input Type",
                            "Animation 入力種類",
                            "動畫輸入類型",
                            "动画输入类型"};
                        ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                    }
                    ImGui::TableNextColumn();
                    {
                        constexpr char const *const items[LANGUAGE_COUNT][ANIMATION_INPUT_TYPE_COUNT] = {
                            {"Disable", "Video Detector", "Instance Motion"},
                            {"無効", "動画検出", "実例行動"},
                            {"停用", "視訊檢測", "實例動作"},
                            {"停用", "视频检测", "实例动作"}};

                        int select_combo_index = std::min(std::max(ANIMATION_INPUT_TYPE_MIN, found_instance_controller.m_animation_input_type), ANIMATION_INPUT_TYPE_MAX);

                        ImGui::Combo("##Instance-Model-Manager-Animation-Input-Type", &select_combo_index, items[ui_controller->m_language_index], IM_ARRAYSIZE(items[0]));

                        found_instance_controller.m_animation_input_type = std::min(std::max(ANIMATION_INPUT_TYPE_MIN, static_cast<ANIMATION_INPUT_TYPE>(select_combo_index)), ANIMATION_INPUT_TYPE_MAX);
                    }

                    if (ANIMATION_INPUT_TYPE_VIDEO_DETECTOR == found_instance_controller.m_animation_input_type)
                    {
                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        ImGui::AlignTextToFramePadding();
                        {
                            constexpr char const *const text[LANGUAGE_COUNT] = {
                                "Video Detector",
                                "動画検出",
                                "視訊檢測",
                                "视频检测"};
                            ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                        }
                        ImGui::TableNextColumn();
                        {
                            mcrt_vector<uint64_t> item_values(static_cast<size_t>(ui_model->m_video_detectors.size() + 1U));
                            mcrt_vector<mcrt_string> item_strings(static_cast<size_t>(ui_model->m_video_detectors.size() + 1U));
                            mcrt_vector<char const *> items(static_cast<size_t>(ui_model->m_video_detectors.size() + 1U));
                            int selected_combo_index = 0;
                            {
                                size_t combo_index = 0U;

                                {
                                    item_values[combo_index] = INVALID_TIMESTAMP;

                                    char const *const text[LANGUAGE_COUNT] = {
                                        "Disable",
                                        "無効",
                                        "停用",
                                        "停用"};

                                    item_strings[combo_index] = text[ui_controller->m_language_index];

                                    items[combo_index] = item_strings[combo_index].c_str();

                                    if (found_instance_model->second.m_video_detector == item_values[combo_index])
                                    {
                                        assert(0 == selected_combo_index);
                                        assert(0 == combo_index);
                                    }

                                    ++combo_index;
                                }

                                for (auto const &video_detector : ui_model->m_video_detectors)
                                {
                                    item_values[combo_index] = video_detector.first;

                                    item_strings[combo_index].clear();
                                    {
                                        char video_detector_timestamp_text[] = {"18446744073709551615"};
                                        std::snprintf(video_detector_timestamp_text, sizeof(video_detector_timestamp_text) / sizeof(video_detector_timestamp_text[0]), "%020llu", static_cast<long long unsigned>(item_values[combo_index]));
                                        video_detector_timestamp_text[(sizeof(video_detector_timestamp_text) / sizeof(video_detector_timestamp_text[0])) - 1] = '\0';

                                        item_strings[combo_index] += video_detector_timestamp_text;
                                        item_strings[combo_index] += ' ';
                                        item_strings[combo_index] += video_detector.second.m_name;
                                    }

                                    items[combo_index] = item_strings[combo_index].c_str();

                                    if (found_instance_model->second.m_video_detector == item_values[combo_index])
                                    {
                                        assert(0 == selected_combo_index);
                                        assert((1U + combo_index) < static_cast<size_t>(INT_MAX));
                                        selected_combo_index = static_cast<int>(combo_index);
                                    }

                                    ++combo_index;
                                }
                            }

                            ImGui::Combo("##Instance-Model-Manager-Right-Group-Table-Value-Video-Detector", &selected_combo_index, items.data(), items.size());

                            if (selected_combo_index >= 0)
                            {
                                uint64_t const selected_video_detector = item_values[selected_combo_index];

                                if (found_instance_model->second.m_video_detector != selected_video_detector)
                                {
                                    auto const found_video_detector = ui_model->m_video_detectors.find(selected_video_detector);

                                    if (ui_model->m_video_detectors.end() != found_video_detector)
                                    {
                                        found_instance_model->second.m_skeleton_instance->set_input_video_detector(found_video_detector->second.m_video_detector);

                                        found_instance_model->second.m_video_detector = selected_video_detector;

                                        found_instance_model->second.m_instance_motion = INVALID_TIMESTAMP;
                                    }
                                    else
                                    {
                                        assert(0 == selected_combo_index);
                                        assert(INVALID_TIMESTAMP == selected_video_detector);

                                        found_instance_model->second.m_skeleton_instance->set_input_video_detector(NULL);

                                        found_instance_model->second.m_skeleton_instance->set_input_animation_instance(NULL);

                                        for (uint32_t morph_target_name_index = 0U; morph_target_name_index < BRX_ANARI_MORPH_TARGET_NAME_MMD_COUNT; ++morph_target_name_index)
                                        {
                                            BRX_ANARI_MORPH_TARGET_NAME const morph_target_name = static_cast<BRX_ANARI_MORPH_TARGET_NAME>(morph_target_name_index);

                                            found_instance_model->second.m_surface_group_instance->set_morph_target_weight(morph_target_name, 0.0F);
                                        }

                                        {
                                            mcrt_vector<brx_anari_rigid_transform> const skin_transforms(static_cast<size_t>(found_instance_model->second.m_skeleton_instance->get_skin_transform_count()), brx_anari_rigid_transform{{0.0F, 0.0F, 0.0F, 1.0F}, {0.0F, 0.0F, 0.0F}});

                                            found_instance_model->second.m_surface_group_instance->set_skin_transforms(found_instance_model->second.m_skeleton_instance->get_skin_transform_count(), skin_transforms.data());
                                        }

                                        found_instance_model->second.m_video_detector = INVALID_TIMESTAMP;

                                        found_instance_model->second.m_instance_motion = INVALID_TIMESTAMP;
                                    }
                                }
                            }
                            else
                            {
                                assert(false);
                            }
                        }

                        assert(NULL != found_instance_model->second.m_skeleton_instance);
                        brx_motion_video_detector *const video_detector = found_instance_model->second.m_skeleton_instance->get_input_video_detector();

                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        ImGui::AlignTextToFramePadding();
                        {
                            constexpr char const *const text[LANGUAGE_COUNT] = {
                                "Hand Index",
                                "人手索引",
                                "人手索引",
                                "人手索引"};
                            ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                        }
                        ImGui::TableNextColumn();
                        {
                            constexpr char const *const label = "##Instance-Model-Manager-Right-Group-Table-Value-Video-Detector-Hand-Index";

                            if (NULL != video_detector)
                            {
                                uint32_t const hand_count = static_cast<uint32_t>(video_detector->get_hand_count());

                                if (hand_count > 0U)
                                {
                                    mcrt_vector<uint32_t> item_values(static_cast<size_t>(hand_count));
                                    mcrt_vector<mcrt_string> item_strings(static_cast<size_t>(hand_count));
                                    mcrt_vector<char const *> items(static_cast<size_t>(hand_count));
                                    int selected_combo_index = 0;
                                    {
                                        uint32_t combo_index = 0U;

                                        for (uint32_t hand_index = 0U; hand_index < hand_count; ++hand_index)
                                        {
                                            item_values[combo_index] = hand_index;

                                            char hand_index_text[] = {"18446744073709551615"};
                                            std::snprintf(hand_index_text, sizeof(hand_index_text) / sizeof(hand_index_text[0]), "%llu", static_cast<long long unsigned>(item_values[combo_index]));
                                            hand_index_text[(sizeof(hand_index_text) / sizeof(hand_index_text[0])) - 1] = '\0';

                                            item_strings[combo_index] = hand_index_text;
                                            items[combo_index] = item_strings[combo_index].c_str();

                                            if (found_instance_model->second.m_skeleton_instance->get_input_video_detector_hand_index() == item_values[combo_index])
                                            {
                                                assert(0 == selected_combo_index);
                                                assert(static_cast<size_t>(1U + combo_index) < static_cast<size_t>(INT_MAX));
                                                selected_combo_index = static_cast<int>(combo_index);
                                            }

                                            ++combo_index;
                                        }
                                    }

                                    ImGui::Combo(label, &selected_combo_index, items.data(), items.size());

                                    if (selected_combo_index >= 0)
                                    {
                                        if (found_instance_model->second.m_skeleton_instance->get_input_video_detector_hand_index() != item_values[selected_combo_index])
                                        {
                                            found_instance_model->second.m_skeleton_instance->set_input_video_detector_hand_index(item_values[selected_combo_index]);
                                        }
                                    }
                                    else
                                    {
                                        assert(false);
                                    }
                                }
                                else
                                {

                                    char const *const items[LANGUAGE_COUNT][1] = {
                                        {"Disable"},
                                        {"無効"},
                                        {"停用"},
                                        {"停用"}};

                                    int selected_combo_index = 0;

                                    ImGui::Combo(label, &selected_combo_index, items[ui_controller->m_language_index], IM_ARRAYSIZE(items[ui_controller->m_language_index]));

                                    assert(0 == selected_combo_index);

                                    found_instance_model->second.m_skeleton_instance->set_input_video_detector_hand_index(BRX_MOTION_UINT32_INDEX_INVALID);
                                }
                            }
                            else
                            {

                                char const *const items[LANGUAGE_COUNT][1] = {
                                    {"Disable"},
                                    {"無効"},
                                    {"停用"},
                                    {"停用"}};

                                int selected_combo_index = 0;

                                ImGui::Combo(label, &selected_combo_index, items[ui_controller->m_language_index], IM_ARRAYSIZE(items[ui_controller->m_language_index]));

                                assert(0 == selected_combo_index);

                                found_instance_model->second.m_skeleton_instance->set_input_video_detector_hand_index(BRX_MOTION_UINT32_INDEX_INVALID);
                            }
                        }

                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        ImGui::AlignTextToFramePadding();
                        {
                            constexpr char const *const text[LANGUAGE_COUNT] = {
                                "Face Index",
                                "人顔索引",
                                "人臉索引",
                                "人脸索引"};
                            ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                        }
                        ImGui::TableNextColumn();
                        {
                            constexpr char const *const label = "##Instance-Model-Manager-Right-Group-Table-Value-Video-Detector-Face-Index";

                            if (NULL != video_detector)
                            {
                                uint32_t const face_count = video_detector->get_face_count();

                                if (face_count > 0U)
                                {
                                    mcrt_vector<uint32_t> item_values(static_cast<size_t>(face_count));
                                    mcrt_vector<mcrt_string> item_strings(static_cast<size_t>(face_count));
                                    mcrt_vector<char const *> items(static_cast<size_t>(face_count));
                                    int selected_combo_index = 0;
                                    {
                                        uint32_t combo_index = 0U;

                                        for (uint32_t face_index = 0U; face_index < face_count; ++face_index)
                                        {
                                            item_values[combo_index] = face_index;

                                            char face_index_text[] = {"18446744073709551615"};
                                            std::snprintf(face_index_text, sizeof(face_index_text) / sizeof(face_index_text[0]), "%llu", static_cast<long long unsigned>(item_values[combo_index]));
                                            face_index_text[(sizeof(face_index_text) / sizeof(face_index_text[0])) - 1] = '\0';

                                            item_strings[combo_index] = face_index_text;
                                            items[combo_index] = item_strings[combo_index].c_str();

                                            if (found_instance_model->second.m_skeleton_instance->get_input_video_detector_face_index() == item_values[combo_index])
                                            {
                                                assert(0 == selected_combo_index);
                                                assert(static_cast<size_t>(1U + combo_index) < static_cast<size_t>(INT_MAX));
                                                selected_combo_index = static_cast<int>(combo_index);
                                            }

                                            ++combo_index;
                                        }
                                    }

                                    ImGui::Combo(label, &selected_combo_index, items.data(), items.size());

                                    if (selected_combo_index >= 0)
                                    {
                                        if (found_instance_model->second.m_skeleton_instance->get_input_video_detector_face_index() != item_values[selected_combo_index])
                                        {
                                            found_instance_model->second.m_skeleton_instance->set_input_video_detector_face_index(item_values[selected_combo_index]);
                                        }
                                    }
                                    else
                                    {
                                        assert(false);
                                    }
                                }
                                else
                                {
                                    char const *const items[LANGUAGE_COUNT][1] = {
                                        {"Disable"},
                                        {"無効"},
                                        {"停用"},
                                        {"停用"}};

                                    int selected_combo_index = 0;

                                    ImGui::Combo(label, &selected_combo_index, items[ui_controller->m_language_index], IM_ARRAYSIZE(items[ui_controller->m_language_index]));

                                    assert(0 == selected_combo_index);

                                    found_instance_model->second.m_skeleton_instance->set_input_video_detector_face_index(BRX_MOTION_UINT32_INDEX_INVALID);
                                }
                            }
                            else
                            {
                                char const *const items[LANGUAGE_COUNT][1] = {
                                    {"Disable"},
                                    {"無効"},
                                    {"停用"},
                                    {"停用"}};

                                int selected_combo_index = 0;

                                ImGui::Combo(label, &selected_combo_index, items[ui_controller->m_language_index], IM_ARRAYSIZE(items[ui_controller->m_language_index]));

                                assert(0 == selected_combo_index);

                                found_instance_model->second.m_skeleton_instance->set_input_video_detector_face_index(BRX_MOTION_UINT32_INDEX_INVALID);
                            }
                        }

                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        ImGui::AlignTextToFramePadding();
                        {
                            constexpr char const *const text[LANGUAGE_COUNT] = {
                                "Pose Index",
                                "姿勢索引",
                                "姿勢索引",
                                "姿勢索引"};
                            ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                        }
                        ImGui::TableNextColumn();
                        {
                            constexpr char const *const label = "##Instance-Model-Manager-Right-Group-Table-Value-Video-Detector-Pose-Index";

                            if (NULL != video_detector)
                            {
                                uint32_t const pose_count = static_cast<uint32_t>(video_detector->get_pose_count());

                                if (pose_count > 0U)
                                {
                                    mcrt_vector<uint32_t> item_values(static_cast<size_t>(pose_count));
                                    mcrt_vector<mcrt_string> item_strings(static_cast<size_t>(pose_count));
                                    mcrt_vector<char const *> items(static_cast<size_t>(pose_count));
                                    int selected_combo_index = 0;
                                    {
                                        uint32_t combo_index = 0U;

                                        for (uint32_t pose_index = 0U; pose_index < pose_count; ++pose_index)
                                        {
                                            item_values[combo_index] = pose_index;

                                            char pose_index_text[] = {"18446744073709551615"};
                                            std::snprintf(pose_index_text, sizeof(pose_index_text) / sizeof(pose_index_text[0]), "%llu", static_cast<long long unsigned>(item_values[combo_index]));
                                            pose_index_text[(sizeof(pose_index_text) / sizeof(pose_index_text[0])) - 1] = '\0';

                                            item_strings[combo_index] = pose_index_text;
                                            items[combo_index] = item_strings[combo_index].c_str();

                                            if (found_instance_model->second.m_skeleton_instance->get_input_video_detector_pose_index() == item_values[combo_index])
                                            {
                                                assert(0 == selected_combo_index);
                                                assert(static_cast<size_t>(1U + combo_index) < static_cast<size_t>(INT_MAX));
                                                selected_combo_index = static_cast<int>(combo_index);
                                            }

                                            ++combo_index;
                                        }
                                    }

                                    ImGui::Combo(label, &selected_combo_index, items.data(), items.size());

                                    if (selected_combo_index >= 0)
                                    {
                                        if (found_instance_model->second.m_skeleton_instance->get_input_video_detector_pose_index() != item_values[selected_combo_index])
                                        {
                                            found_instance_model->second.m_skeleton_instance->set_input_video_detector_pose_index(item_values[selected_combo_index]);
                                        }
                                    }
                                    else
                                    {
                                        assert(false);
                                    }
                                }
                                else
                                {

                                    char const *const items[LANGUAGE_COUNT][1] = {
                                        {"Disable"},
                                        {"無効"},
                                        {"停用"},
                                        {"停用"}};

                                    int selected_combo_index = 0;

                                    ImGui::Combo(label, &selected_combo_index, items[ui_controller->m_language_index], IM_ARRAYSIZE(items[ui_controller->m_language_index]));

                                    assert(0 == selected_combo_index);

                                    found_instance_model->second.m_skeleton_instance->set_input_video_detector_pose_index(BRX_MOTION_UINT32_INDEX_INVALID);
                                }
                            }
                            else
                            {

                                char const *const items[LANGUAGE_COUNT][1] = {
                                    {"Disable"},
                                    {"無効"},
                                    {"停用"},
                                    {"停用"}};

                                int selected_combo_index = 0;

                                ImGui::Combo(label, &selected_combo_index, items[ui_controller->m_language_index], IM_ARRAYSIZE(items[ui_controller->m_language_index]));

                                assert(0 == selected_combo_index);

                                found_instance_model->second.m_skeleton_instance->set_input_video_detector_pose_index(BRX_MOTION_UINT32_INDEX_INVALID);
                            }
                        }
                    }
                    else if (ANIMATION_INPUT_TYPE_INSTANCE_MOTION == found_instance_controller.m_animation_input_type)
                    {
                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();
                        ImGui::AlignTextToFramePadding();
                        {
                            constexpr char const *const text[LANGUAGE_COUNT] = {
                                "Instance Motion",
                                "実例行動",
                                "實例動作",
                                "实例动作"};
                            ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                        }
                        ImGui::TableNextColumn();
                        {

                            mcrt_vector<uint64_t> item_values(static_cast<size_t>(ui_model->m_instance_motions.size() + 1U));
                            mcrt_vector<mcrt_string> item_strings(static_cast<size_t>(ui_model->m_instance_motions.size() + 1U));
                            mcrt_vector<char const *> items(static_cast<size_t>(ui_model->m_instance_motions.size() + 1U));
                            int selected_combo_index = 0;
                            {
                                size_t combo_index = 0U;

                                {
                                    item_values[combo_index] = INVALID_TIMESTAMP;

                                    char const *const text[LANGUAGE_COUNT] = {
                                        "Disable",
                                        "無効",
                                        "停用",
                                        "停用"};

                                    item_strings[combo_index] = text[ui_controller->m_language_index];

                                    items[combo_index] = item_strings[combo_index].c_str();

                                    if (found_instance_model->second.m_instance_motion == item_values[combo_index])
                                    {
                                        assert(0 == selected_combo_index);
                                        assert(0 == combo_index);
                                    }

                                    ++combo_index;
                                }

                                for (auto const &instance_motion : ui_model->m_instance_motions)
                                {
                                    item_values[combo_index] = instance_motion.first;

                                    item_strings[combo_index].clear();
                                    {
                                        char instance_motion_timestamp_text[] = {"18446744073709551615"};
                                        std::snprintf(instance_motion_timestamp_text, sizeof(instance_motion_timestamp_text) / sizeof(instance_motion_timestamp_text[0]), "%020llu", static_cast<long long unsigned>(item_values[combo_index]));
                                        instance_motion_timestamp_text[(sizeof(instance_motion_timestamp_text) / sizeof(instance_motion_timestamp_text[0])) - 1] = '\0';

                                        item_strings[combo_index] += instance_motion_timestamp_text;
                                        item_strings[combo_index] += ' ';
                                        item_strings[combo_index] += instance_motion.second.m_name;
                                    }

                                    items[combo_index] = item_strings[combo_index].c_str();

                                    if (found_instance_model->second.m_instance_motion == item_values[combo_index])
                                    {
                                        assert(0 == selected_combo_index);
                                        assert((1U + combo_index) < static_cast<size_t>(INT_MAX));
                                        selected_combo_index = static_cast<int>(combo_index);
                                    }

                                    ++combo_index;
                                }
                            }

                            ImGui::Combo("##Instance-Model-Manager-Right-Group-Table-Value-Instance-Motion", &selected_combo_index, items.data(), items.size());

                            if (selected_combo_index >= 0)
                            {
                                uint64_t const selected_instance_motion = item_values[selected_combo_index];

                                if (found_instance_model->second.m_instance_motion != selected_instance_motion)
                                {
                                    auto const found_instance_motion = ui_model->m_instance_motions.find(selected_instance_motion);

                                    if (ui_model->m_instance_motions.end() != found_instance_motion)
                                    {
                                        found_instance_model->second.m_skeleton_instance->set_input_animation_instance(found_instance_motion->second.m_animation_instance);

                                        found_instance_model->second.m_video_detector = INVALID_TIMESTAMP;

                                        found_instance_model->second.m_instance_motion = selected_instance_motion;
                                    }
                                    else
                                    {
                                        assert(0 == selected_combo_index);
                                        assert(INVALID_TIMESTAMP == selected_instance_motion);

                                        found_instance_model->second.m_skeleton_instance->set_input_video_detector(NULL);

                                        found_instance_model->second.m_skeleton_instance->set_input_animation_instance(NULL);

                                        for (uint32_t morph_target_name_index = 0U; morph_target_name_index < BRX_ANARI_MORPH_TARGET_NAME_MMD_COUNT; ++morph_target_name_index)
                                        {
                                            BRX_ANARI_MORPH_TARGET_NAME const morph_target_name = static_cast<BRX_ANARI_MORPH_TARGET_NAME>(morph_target_name_index);

                                            found_instance_model->second.m_surface_group_instance->set_morph_target_weight(morph_target_name, 0.0F);
                                        }

                                        {
                                            mcrt_vector<brx_anari_rigid_transform> const skin_transforms(static_cast<size_t>(found_instance_model->second.m_skeleton_instance->get_skin_transform_count()), brx_anari_rigid_transform{{0.0F, 0.0F, 0.0F, 1.0F}, {0.0F, 0.0F, 0.0F}});

                                            found_instance_model->second.m_surface_group_instance->set_skin_transforms(found_instance_model->second.m_skeleton_instance->get_skin_transform_count(), skin_transforms.data());
                                        }

                                        found_instance_model->second.m_video_detector = INVALID_TIMESTAMP;

                                        found_instance_model->second.m_instance_motion = INVALID_TIMESTAMP;
                                    }
                                }
                            }
                            else
                            {
                                assert(false);
                            }
                        }
                    }
                    else
                    {
                        assert(ANIMATION_INPUT_TYPE_INVALID == found_instance_controller.m_animation_input_type);

                        found_instance_model->second.m_skeleton_instance->set_input_video_detector(NULL);

                        found_instance_model->second.m_skeleton_instance->set_input_animation_instance(NULL);

                        for (uint32_t morph_target_name_index = 0U; morph_target_name_index < BRX_ANARI_MORPH_TARGET_NAME_MMD_COUNT; ++morph_target_name_index)
                        {
                            BRX_ANARI_MORPH_TARGET_NAME const morph_target_name = static_cast<BRX_ANARI_MORPH_TARGET_NAME>(morph_target_name_index);

                            found_instance_model->second.m_surface_group_instance->set_morph_target_weight(morph_target_name, 0.0F);
                        }

                        {
                            mcrt_vector<brx_anari_rigid_transform> const skin_transforms(static_cast<size_t>(found_instance_model->second.m_skeleton_instance->get_skin_transform_count()), brx_anari_rigid_transform{{0.0F, 0.0F, 0.0F, 1.0F}, {0.0F, 0.0F, 0.0F}});

                            found_instance_model->second.m_surface_group_instance->set_skin_transforms(found_instance_model->second.m_skeleton_instance->get_skin_transform_count(), skin_transforms.data());
                        }

                        found_instance_model->second.m_video_detector = INVALID_TIMESTAMP;

                        found_instance_model->second.m_instance_motion = INVALID_TIMESTAMP;
                    }
                }
                else
                {
                    assert(INVALID_TIMESTAMP == found_instance_model->second.m_video_detector);
                    assert(INVALID_TIMESTAMP == found_instance_model->second.m_instance_motion);
                }

                {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::AlignTextToFramePadding();

                    {
                        constexpr char const *const text[LANGUAGE_COUNT] = {
                            "Rotation Euler Roll",
                            "回転 Euler Roll",
                            "旋轉 Euler Roll",
                            "旋转 Euler Roll"};
                        ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                    }

                    ImGui::TableNextColumn();

                    {
                        float rotation = std::min(std::max(-180.0F, DirectX::XMConvertToDegrees(found_instance_model->second.m_model_transform_rotation_roll)), 180.0F);
#define _INTERNAL_BRX_STRINGIZING(string) #string
#define _INTERNAL_BRX_X_STRINGIZING(string) _INTERNAL_BRX_STRINGIZING(string)
                        ImGui::SliderFloat("##Instance-Model-Manager-Right-Group-Table-Value-Model-Transform-Rotation-Euler-Roll-Slider", &rotation, -180.0F, 180.0F, "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f");

                        // ImGui::SameLine();

                        // ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * table_column_same_line_ratio);

                        char rotation_text[256];
                        std::snprintf(&rotation_text[0], sizeof(rotation_text) / sizeof(rotation_text[0]), "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f", rotation);
#undef _INTERNAL_BRX_STRINGIZING
#undef _INTERNAL_BRX_X_STRINGIZING
                        rotation_text[(sizeof(rotation_text) / sizeof(rotation_text[0]) - 1)] = '\0';

                        ImGui::InputText("##Instance-Model-Manager-Right-Group-Table-Value-Model-Transform-Rotation-Euler-Roll-Text", &rotation_text[0], sizeof(rotation_text) / sizeof(rotation_text[0]), ImGuiInputTextFlags_CharsDecimal);

                        found_instance_model->second.m_model_transform_rotation_roll = DirectX::XMConvertToRadians(std::min(std::max(-180.0F, std::strtof(rotation_text, NULL)), 180.0F));
                    }

                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::AlignTextToFramePadding();

                    {
                        constexpr char const *const text[LANGUAGE_COUNT] = {
                            "Rotation Euler Pitch",
                            "回転 Euler Pitch",
                            "旋轉 Euler Pitch",
                            "旋转 Euler Pitch"};
                        ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                    }

                    ImGui::TableNextColumn();

                    {
                        float rotation = std::min(std::max(-180.0F, DirectX::XMConvertToDegrees(found_instance_model->second.m_model_transform_rotation_pitch)), 180.0F);
#define _INTERNAL_BRX_STRINGIZING(string) #string
#define _INTERNAL_BRX_X_STRINGIZING(string) _INTERNAL_BRX_STRINGIZING(string)
                        ImGui::SliderFloat("##Instance-Model-Manager-Right-Group-Table-Value-Model-Transform-Rotation-Euler-Pitch-Slider", &rotation, -180.0F, 180.0F, "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f");

                        // ImGui::SameLine();

                        // ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * table_column_same_line_ratio);

                        char rotation_text[256];
                        std::snprintf(&rotation_text[0], sizeof(rotation_text) / sizeof(rotation_text[0]), "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f", rotation);
#undef _INTERNAL_BRX_STRINGIZING
#undef _INTERNAL_BRX_X_STRINGIZING
                        rotation_text[(sizeof(rotation_text) / sizeof(rotation_text[0]) - 1)] = '\0';

                        ImGui::InputText("##Instance-Model-Manager-Right-Group-Table-Value-Model-Transform-Rotation-Euler-Pitch-Text", &rotation_text[0], sizeof(rotation_text) / sizeof(rotation_text[0]), ImGuiInputTextFlags_CharsDecimal);

                        found_instance_model->second.m_model_transform_rotation_pitch = DirectX::XMConvertToRadians(std::min(std::max(-180.0F, std::strtof(rotation_text, NULL)), 180.0F));
                    }

                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::AlignTextToFramePadding();

                    {
                        constexpr char const *const text[LANGUAGE_COUNT] = {
                            "Rotation Euler Yaw",
                            "回転 Euler Yaw",
                            "旋轉 Euler Yaw",
                            "旋转 Euler Yaw"};
                        ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                    }

                    ImGui::TableNextColumn();

                    {
                        float rotation = std::min(std::max(-180.0F, DirectX::XMConvertToDegrees(found_instance_model->second.m_model_transform_rotation_yaw)), 180.0F);
#define _INTERNAL_BRX_STRINGIZING(string) #string
#define _INTERNAL_BRX_X_STRINGIZING(string) _INTERNAL_BRX_STRINGIZING(string)
                        ImGui::SliderFloat("##Instance-Model-Manager-Right-Group-Table-Value-Model-Transform-Rotation-Euler-Yaw-Slider", &rotation, -180.0F, 180.0F, "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f");

                        // ImGui::SameLine();

                        // ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * table_column_same_line_ratio);

                        char rotation_text[256];
                        std::snprintf(&rotation_text[0], sizeof(rotation_text) / sizeof(rotation_text[0]), "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f", rotation);
#undef _INTERNAL_BRX_STRINGIZING
#undef _INTERNAL_BRX_X_STRINGIZING
                        rotation_text[(sizeof(rotation_text) / sizeof(rotation_text[0]) - 1)] = '\0';

                        ImGui::InputText("##Instance-Model-Manager-Right-Group-Table-Value-Model-Transform-Rotation-Euler-Yaw-Text", &rotation_text[0], sizeof(rotation_text) / sizeof(rotation_text[0]), ImGuiInputTextFlags_CharsDecimal);

                        found_instance_model->second.m_model_transform_rotation_yaw = DirectX::XMConvertToRadians(std::min(std::max(-180.0F, std::strtof(rotation_text, NULL)), 180.0F));
                    }
                }

                {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::AlignTextToFramePadding();

                    {
                        constexpr char const *const text[LANGUAGE_COUNT] = {
                            "Translation X",
                            "平行移動 X",
                            "平移 X",
                            "平移 X"};
                        ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                    }

                    ImGui::TableNextColumn();

                    {
                        char translation_text[256];
#define _INTERNAL_BRX_STRINGIZING(string) #string
#define _INTERNAL_BRX_X_STRINGIZING(string) _INTERNAL_BRX_STRINGIZING(string)
                        std::snprintf(&translation_text[0], sizeof(translation_text) / sizeof(translation_text[0]), "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f", found_instance_model->second.m_model_transform_translation_x);
#undef _INTERNAL_BRX_STRINGIZING
#undef _INTERNAL_BRX_X_STRINGIZING
                        translation_text[(sizeof(translation_text) / sizeof(translation_text[0]) - 1)] = '\0';

                        ImGui::InputText("##Instance-Model-Manager-Right-Group-Table-Value-Model-Transform-Translation-X", &translation_text[0], sizeof(translation_text) / sizeof(translation_text[0]), ImGuiInputTextFlags_CharsDecimal);

                        found_instance_model->second.m_model_transform_translation_x = std::strtof(translation_text, NULL);
                    }

                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::AlignTextToFramePadding();

                    {
                        constexpr char const *const text[LANGUAGE_COUNT] = {
                            "Translation Y",
                            "平行移動 Y",
                            "平移 Y",
                            "平移 Y"};
                        ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                    }

                    ImGui::TableNextColumn();

                    {
                        char translation_text[256];
#define _INTERNAL_BRX_STRINGIZING(string) #string
#define _INTERNAL_BRX_X_STRINGIZING(string) _INTERNAL_BRX_STRINGIZING(string)
                        std::snprintf(&translation_text[0], sizeof(translation_text) / sizeof(translation_text[0]), "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f", found_instance_model->second.m_model_transform_translation_y);
#undef _INTERNAL_BRX_STRINGIZING
#undef _INTERNAL_BRX_X_STRINGIZING
                        translation_text[(sizeof(translation_text) / sizeof(translation_text[0]) - 1)] = '\0';

                        ImGui::InputText("##Instance-Model-Manager-Right-Group-Table-Value-Model-Transform-Translation-Y", &translation_text[0], sizeof(translation_text) / sizeof(translation_text[0]), ImGuiInputTextFlags_CharsDecimal);

                        found_instance_model->second.m_model_transform_translation_y = std::strtof(translation_text, NULL);
                    }

                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::AlignTextToFramePadding();

                    {
                        constexpr char const *const text[LANGUAGE_COUNT] = {
                            "Translation Z",
                            "平行移動 Z",
                            "平移 Z",
                            "平移 Z"};
                        ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                    }

                    ImGui::TableNextColumn();

                    {
                        char translation_text[256];
#define _INTERNAL_BRX_STRINGIZING(string) #string
#define _INTERNAL_BRX_X_STRINGIZING(string) _INTERNAL_BRX_STRINGIZING(string)
                        std::snprintf(&translation_text[0], sizeof(translation_text) / sizeof(translation_text[0]), "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f", found_instance_model->second.m_model_transform_translation_z);
#undef _INTERNAL_BRX_STRINGIZING
#undef _INTERNAL_BRX_X_STRINGIZING
                        translation_text[(sizeof(translation_text) / sizeof(translation_text[0]) - 1)] = '\0';

                        ImGui::InputText("##Instance-Model-Manager-Right-Group-Table-Value-Model-Transform-Translation-Z", &translation_text[0], sizeof(translation_text) / sizeof(translation_text[0]), ImGuiInputTextFlags_CharsDecimal);

                        found_instance_model->second.m_model_transform_translation_z = std::strtof(translation_text, NULL);
                    }
                }

                {
                    DirectX::XMFLOAT4 quaternion;
                    DirectX::XMStoreFloat4(&quaternion, DirectX::XMQuaternionRotationRollPitchYaw(found_instance_model->second.m_model_transform_rotation_pitch, found_instance_model->second.m_model_transform_rotation_yaw, found_instance_model->second.m_model_transform_rotation_roll));

                    brx_anari_rigid_transform const model_transform{{quaternion.x, quaternion.y, quaternion.z, quaternion.w}, {found_instance_model->second.m_model_transform_translation_x, found_instance_model->second.m_model_transform_translation_y, found_instance_model->second.m_model_transform_translation_z}};
                    found_instance_model->second.m_surface_group_instance->set_model_transform(model_transform);
                }

                ImGui::EndTable();
            }

            ImGui::EndGroup();
        }

        ImGui::End();
    }

    if (ui_controller->m_show_camera_manager)
    {
        {
            ImGuiViewport const *const main_viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + ui_x * ui_index, main_viewport->WorkPos.y + ui_y * ui_index), ImGuiCond_FirstUseEver);
            ++ui_index;

            ImGui::SetNextWindowSize(ImVec2(ui_width, ui_height), ImGuiCond_FirstUseEver);
        }

        ImGui::Begin("Camera Manager", &ui_controller->m_show_camera_manager);

        {
            constexpr char const *const text[LANGUAGE_COUNT] = {
                "Camera Manager",
                "写真機管理",
                "相機管理",
                "相机管理"};
            ImGui::TextUnformatted(text[ui_controller->m_language_index]);
        }

        ImGui::SameLine();

        {
            ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
            ImGui::TextUnformatted(help_marker_text);
            ImGui::PopStyleColor();
            if (ImGui::BeginItemTooltip())
            {
                ImGui::PushTextWrapPos(help_marker_position);
                constexpr char const *const text[LANGUAGE_COUNT] = {
                    "Q -> Up | E -> Down | W -> Forward | S -> Backward | A -> Left | D -> Right | Mouse Right Button -> Rotation",
                    "Q -> 上 | E -> 下 | W -> 前 | S -> 後 | A -> 左 | D -> 右 | Mouse Right Button -> 回転",
                    "Q -> 上 | E -> 下 | W -> 前 | S -> 後 | A -> 左 | D -> 右 | Mouse Right Button -> 旋轉",
                    "Q -> 上 | E -> 下 | W -> 前 | S -> 后 | A -> 左 | D -> 右 | Mouse Right Button -> 旋转"};
                ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                ImGui::PopTextWrapPos();
                ImGui::EndTooltip();
            }
        }

        ImGui::Separator();

        if (ImGui::BeginTable("##Camera-Manager-Table", 2, ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersInnerV))
        {
            ImGui::TableSetupColumn("##Camera-Manager-Table-Property", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("##Camera-Manager-Table-Value", ImGuiTableColumnFlags_WidthStretch);

            {
                brx_anari_vec3 camera_position = device->camera_get_position();

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Position X",
                        "位置 X",
                        "位置 X",
                        "位置 X"};
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {
                    char position_text[256];
#define _INTERNAL_BRX_STRINGIZING(string) #string
#define _INTERNAL_BRX_X_STRINGIZING(string) _INTERNAL_BRX_STRINGIZING(string)
                    std::snprintf(&position_text[0], sizeof(position_text) / sizeof(position_text[0]), "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f", camera_position.m_x);
#undef _INTERNAL_BRX_STRINGIZING
#undef _INTERNAL_BRX_X_STRINGIZING
                    position_text[(sizeof(position_text) / sizeof(position_text[0]) - 1)] = '\0';

                    ImGui::InputText("##Camera-Manager-Table-Value-Position-X", &position_text[0], sizeof(position_text) / sizeof(position_text[0]), ImGuiInputTextFlags_CharsDecimal);

                    camera_position.m_x = std::strtof(position_text, NULL);
                }

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Position Y",
                        "位置 Y",
                        "位置 Y",
                        "位置 Y"};
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {
                    char position_text[256];
#define _INTERNAL_BRX_STRINGIZING(string) #string
#define _INTERNAL_BRX_X_STRINGIZING(string) _INTERNAL_BRX_STRINGIZING(string)
                    std::snprintf(&position_text[0], sizeof(position_text) / sizeof(position_text[0]), "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f", camera_position.m_y);
#undef _INTERNAL_BRX_STRINGIZING
#undef _INTERNAL_BRX_X_STRINGIZING
                    position_text[(sizeof(position_text) / sizeof(position_text[0]) - 1)] = '\0';

                    ImGui::InputText("##Camera-Manager-Table-Value-Position-Y", &position_text[0], sizeof(position_text) / sizeof(position_text[0]), ImGuiInputTextFlags_CharsDecimal);

                    camera_position.m_y = std::strtof(position_text, NULL);
                }

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Position Z",
                        "位置 Z",
                        "位置 Z",
                        "位置 Z"};
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {
                    char position_text[256];
#define _INTERNAL_BRX_STRINGIZING(string) #string
#define _INTERNAL_BRX_X_STRINGIZING(string) _INTERNAL_BRX_STRINGIZING(string)
                    std::snprintf(&position_text[0], sizeof(position_text) / sizeof(position_text[0]), "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f", camera_position.m_z);
#undef _INTERNAL_BRX_STRINGIZING
#undef _INTERNAL_BRX_X_STRINGIZING
                    position_text[(sizeof(position_text) / sizeof(position_text[0]) - 1)] = '\0';

                    ImGui::InputText("##Camera-Manager-Table-Value-Position-Z", &position_text[0], sizeof(position_text) / sizeof(position_text[0]), ImGuiInputTextFlags_CharsDecimal);

                    camera_position.m_z = std::strtof(position_text, NULL);
                }

                device->camera_set_position(camera_position);
            }

            {
                brx_anari_vec3 camera_direction = device->camera_get_direction();

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Direction X",
                        "方向 X",
                        "方向 X",
                        "方向 X"};
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {
                    char direction_text[256];
#define _INTERNAL_BRX_STRINGIZING(string) #string
#define _INTERNAL_BRX_X_STRINGIZING(string) _INTERNAL_BRX_STRINGIZING(string)
                    std::snprintf(&direction_text[0], sizeof(direction_text) / sizeof(direction_text[0]), "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f", camera_direction.m_x);
#undef _INTERNAL_BRX_STRINGIZING
#undef _INTERNAL_BRX_X_STRINGIZING
                    direction_text[(sizeof(direction_text) / sizeof(direction_text[0]) - 1)] = '\0';

                    ImGui::InputText("##Camera-Manager-Table-Value-Direction-X", &direction_text[0], sizeof(direction_text) / sizeof(direction_text[0]), ImGuiInputTextFlags_CharsDecimal);

                    camera_direction.m_x = std::strtof(direction_text, NULL);
                }

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Direction Y",
                        "方向 Y",
                        "方向 Y",
                        "方向 Y"};
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {
                    char direction_text[256];
#define _INTERNAL_BRX_STRINGIZING(string) #string
#define _INTERNAL_BRX_X_STRINGIZING(string) _INTERNAL_BRX_STRINGIZING(string)
                    std::snprintf(&direction_text[0], sizeof(direction_text) / sizeof(direction_text[0]), "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f", camera_direction.m_y);
#undef _INTERNAL_BRX_STRINGIZING
#undef _INTERNAL_BRX_X_STRINGIZING
                    direction_text[(sizeof(direction_text) / sizeof(direction_text[0]) - 1)] = '\0';

                    ImGui::InputText("##Camera-Manager-Table-Value-Direction-Y", &direction_text[0], sizeof(direction_text) / sizeof(direction_text[0]), ImGuiInputTextFlags_CharsDecimal);

                    camera_direction.m_y = std::strtof(direction_text, NULL);
                }

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Direction Z",
                        "方向 Z",
                        "方向 Z",
                        "方向 Z"};
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {
                    char direction_text[256];
#define _INTERNAL_BRX_STRINGIZING(string) #string
#define _INTERNAL_BRX_X_STRINGIZING(string) _INTERNAL_BRX_STRINGIZING(string)
                    std::snprintf(&direction_text[0], sizeof(direction_text) / sizeof(direction_text[0]), "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f", camera_direction.m_z);
#undef _INTERNAL_BRX_STRINGIZING
#undef _INTERNAL_BRX_X_STRINGIZING
                    direction_text[(sizeof(direction_text) / sizeof(direction_text[0]) - 1)] = '\0';

                    ImGui::InputText("##Camera-Manager-Table-Value-Direction-Z", &direction_text[0], sizeof(direction_text) / sizeof(direction_text[0]), ImGuiInputTextFlags_CharsDecimal);

                    camera_direction.m_z = std::strtof(direction_text, NULL);
                }

                {
                    DirectX::XMFLOAT3 const directx_camera_direction(camera_direction.m_x, camera_direction.m_y, camera_direction.m_z);

                    DirectX::XMFLOAT3 directx_normalized_camera_direction;
                    DirectX::XMStoreFloat3(&directx_normalized_camera_direction, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&directx_camera_direction)));
                    camera_direction.m_x = directx_normalized_camera_direction.x;
                    camera_direction.m_y = directx_normalized_camera_direction.y;
                    camera_direction.m_z = directx_normalized_camera_direction.z;
                }

                device->camera_set_direction(camera_direction);
            }

            {
                brx_anari_vec3 camera_up = device->camera_get_up();

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Up X",
                        "上 X",
                        "上 X",
                        "上 X"};
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {
                    char up_text[256];
#define _INTERNAL_BRX_STRINGIZING(string) #string
#define _INTERNAL_BRX_X_STRINGIZING(string) _INTERNAL_BRX_STRINGIZING(string)
                    std::snprintf(&up_text[0], sizeof(up_text) / sizeof(up_text[0]), "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f", camera_up.m_x);
#undef _INTERNAL_BRX_STRINGIZING
#undef _INTERNAL_BRX_X_STRINGIZING
                    up_text[(sizeof(up_text) / sizeof(up_text[0]) - 1)] = '\0';

                    ImGui::InputText("##Camera-Manager-Table-Value-Up-X", &up_text[0], sizeof(up_text) / sizeof(up_text[0]), ImGuiInputTextFlags_CharsDecimal);

                    camera_up.m_x = std::strtof(up_text, NULL);
                }

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Up Y",
                        "上 Y",
                        "上 Y",
                        "上 Y"};
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {
                    char up_text[256];
#define _INTERNAL_BRX_STRINGIZING(string) #string
#define _INTERNAL_BRX_X_STRINGIZING(string) _INTERNAL_BRX_STRINGIZING(string)
                    std::snprintf(&up_text[0], sizeof(up_text) / sizeof(up_text[0]), "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f", camera_up.m_y);
#undef _INTERNAL_BRX_STRINGIZING
#undef _INTERNAL_BRX_X_STRINGIZING
                    up_text[(sizeof(up_text) / sizeof(up_text[0]) - 1)] = '\0';

                    ImGui::InputText("##Camera-Manager-Table-Value-Up-Y", &up_text[0], sizeof(up_text) / sizeof(up_text[0]), ImGuiInputTextFlags_CharsDecimal);

                    camera_up.m_y = std::strtof(up_text, NULL);
                }

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Up Z",
                        "上 Z",
                        "上 Z",
                        "上 Z"};
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {
                    char up_text[256];
#define _INTERNAL_BRX_STRINGIZING(string) #string
#define _INTERNAL_BRX_X_STRINGIZING(string) _INTERNAL_BRX_STRINGIZING(string)
                    std::snprintf(&up_text[0], sizeof(up_text) / sizeof(up_text[0]), "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f", camera_up.m_z);
#undef _INTERNAL_BRX_STRINGIZING
#undef _INTERNAL_BRX_X_STRINGIZING
                    up_text[(sizeof(up_text) / sizeof(up_text[0]) - 1)] = '\0';

                    ImGui::InputText("##Camera-Manager-Table-Value-Up-Z", &up_text[0], sizeof(up_text) / sizeof(up_text[0]), ImGuiInputTextFlags_CharsDecimal);

                    camera_up.m_z = std::strtof(up_text, NULL);
                }

                {
                    DirectX::XMFLOAT3 const directx_camera_up(camera_up.m_x, camera_up.m_y, camera_up.m_z);

                    DirectX::XMFLOAT3 directx_normalized_camera_up;
                    DirectX::XMStoreFloat3(&directx_normalized_camera_up, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&directx_camera_up)));
                    camera_up.m_x = directx_normalized_camera_up.x;
                    camera_up.m_y = directx_normalized_camera_up.y;
                    camera_up.m_z = directx_normalized_camera_up.z;
                }

                device->camera_set_up(camera_up);
            }

            {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "FOV",
                        "FOV",
                        "FOV",
                        "FOV"};
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {
                    float fov = std::min(std::max(0.0F, DirectX::XMConvertToDegrees(device->camera_get_fovy())), 180.0F);
#define _INTERNAL_BRX_STRINGIZING(string) #string
#define _INTERNAL_BRX_X_STRINGIZING(string) _INTERNAL_BRX_STRINGIZING(string)
                    ImGui::SliderFloat("##Camera-Manager-Table-Value-FOV-Slider", &fov, 1.0F, 180.0F, "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f");

                    // ImGui::SameLine();

                    // ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * table_column_same_line_ratio);

                    char fov_text[256];
                    std::snprintf(&fov_text[0], sizeof(fov_text) / sizeof(fov_text[0]), "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f", fov);
#undef _INTERNAL_BRX_STRINGIZING
#undef _INTERNAL_BRX_X_STRINGIZING
                    fov_text[(sizeof(fov_text) / sizeof(fov_text[0]) - 1)] = '\0';

                    ImGui::InputText("##Camera-Manager-Table-Value-FOV-Text", &fov_text[0], sizeof(fov_text) / sizeof(fov_text[0]), ImGuiInputTextFlags_CharsDecimal);

                    device->camera_set_fovy(DirectX::XMConvertToRadians(std::min(std::max(1.0F, std::strtof(fov_text, NULL)), 180.0F)));
                }
            }

            {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Near Z",
                        "近 Z",
                        "近 Z",
                        "近 Z"};
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {
                    char near_text[256];
#define _INTERNAL_BRX_STRINGIZING(string) #string
#define _INTERNAL_BRX_X_STRINGIZING(string) _INTERNAL_BRX_STRINGIZING(string)
                    std::snprintf(&near_text[0], sizeof(near_text) / sizeof(near_text[0]), "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f", std::max(0.001F, device->camera_get_near()));
#undef _INTERNAL_BRX_STRINGIZING
#undef _INTERNAL_BRX_X_STRINGIZING
                    near_text[(sizeof(near_text) / sizeof(near_text[0]) - 1)] = '\0';

                    ImGui::InputText("##Camera-Manager-Table-Value-Near", &near_text[0], sizeof(near_text) / sizeof(near_text[0]), ImGuiInputTextFlags_CharsDecimal);

                    device->camera_set_near(std::max(0.001F, std::strtof(near_text, NULL)));
                }
            }

            {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Far Z",
                        "遠 Z",
                        "遠 Z",
                        "远 Z"};
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {
                    char far_text[256];
#define _INTERNAL_BRX_STRINGIZING(string) #string
#define _INTERNAL_BRX_X_STRINGIZING(string) _INTERNAL_BRX_STRINGIZING(string)
                    std::snprintf(&far_text[0], sizeof(far_text) / sizeof(far_text[0]), "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f", std::max((device->camera_get_near() + 0.001F), device->camera_get_far()));
#undef _INTERNAL_BRX_STRINGIZING
#undef _INTERNAL_BRX_X_STRINGIZING
                    far_text[(sizeof(far_text) / sizeof(far_text[0]) - 1)] = '\0';

                    ImGui::InputText("##Camera-Manager-Table-Value-Far", &far_text[0], sizeof(far_text) / sizeof(far_text[0]), ImGuiInputTextFlags_CharsDecimal);

                    device->camera_set_far(std::max((device->camera_get_near() + 0.001F), std::strtof(far_text, NULL)));
                }
            }

            {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Rotation Speed",
                        "回転速率",
                        "旋轉速率",
                        "旋转速率"};
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {
                    char rotation_speed_text[256];
#define _INTERNAL_BRX_STRINGIZING(string) #string
#define _INTERNAL_BRX_X_STRINGIZING(string) _INTERNAL_BRX_STRINGIZING(string)
                    std::snprintf(&rotation_speed_text[0], sizeof(rotation_speed_text) / sizeof(rotation_speed_text[0]), "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f", std::max(0.0F, DirectX::XMConvertToDegrees(ui_controller->m_first_person_camera.GetRotationScaler())));
#undef _INTERNAL_BRX_STRINGIZING
#undef _INTERNAL_BRX_X_STRINGIZING
                    rotation_speed_text[(sizeof(rotation_speed_text) / sizeof(rotation_speed_text[0]) - 1)] = '\0';

                    ImGui::InputText("##Camera-Manager-Table-Value-Rotation-Speed", &rotation_speed_text[0], sizeof(rotation_speed_text) / sizeof(rotation_speed_text[0]), ImGuiInputTextFlags_CharsDecimal);

                    ui_controller->m_first_person_camera.SetRotationScaler(DirectX::XMConvertToRadians(std::max(0.0F, std::strtof(rotation_speed_text, NULL))));
                }
            }

            {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Translation Speed",
                        "平行移動速率",
                        "平移速率",
                        "平移速率"};
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {
                    char translation_speed_text[256];
#define _INTERNAL_BRX_STRINGIZING(string) #string
#define _INTERNAL_BRX_X_STRINGIZING(string) _INTERNAL_BRX_STRINGIZING(string)
                    std::snprintf(&translation_speed_text[0], sizeof(translation_speed_text) / sizeof(translation_speed_text[0]), "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f", std::max(0.0F, ui_controller->m_first_person_camera.GetMoveScaler()));
#undef _INTERNAL_BRX_STRINGIZING
#undef _INTERNAL_BRX_X_STRINGIZING
                    translation_speed_text[(sizeof(translation_speed_text) / sizeof(translation_speed_text[0]) - 1)] = '\0';

                    ImGui::InputText("##Camera-Manager-Table-Value-Translation-Speed", &translation_speed_text[0], sizeof(translation_speed_text) / sizeof(translation_speed_text[0]), ImGuiInputTextFlags_CharsDecimal);

                    ui_controller->m_first_person_camera.SetMoveScaler(std::max(0.0F, std::strtof(translation_speed_text, NULL)));
                }
            }

            ImGui::EndTable();
        }

        ImGui::End();
    }

    if (ui_controller->m_show_physics_ragdoll_manager)
    {
        {
            ImGuiViewport const *const main_viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + ui_x * ui_index, main_viewport->WorkPos.y + ui_y * ui_index), ImGuiCond_FirstUseEver);
            ++ui_index;

            ImGui::SetNextWindowSize(ImVec2(ui_width, ui_height), ImGuiCond_FirstUseEver);
        }

        ImGui::Begin("Physics Ragdoll Manager", &ui_controller->m_show_physics_ragdoll_manager);

        {
            constexpr char const *const text[LANGUAGE_COUNT] = {
                "Physics Ragdoll Manager",
                "物理布人形管理",
                "物理布偶管理",
                "物理布娃娃管理"};
            ImGui::TextUnformatted(text[ui_controller->m_language_index]);
        }

        ImGui::Separator();

        if (ImGui::BeginTable("##Physics-Ragdoll-Manager-Table", 2, ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersInnerV))
        {
            ImGui::TableSetupColumn("##Physics-Ragdoll-Manager-Table-Property", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("##Physics-Ragdoll-Manager-Table-Value", ImGuiTableColumnFlags_WidthStretch);

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            {
                constexpr char const *const text[LANGUAGE_COUNT] = {
                    "Quality",
                    "品質",
                    "質量",
                    "质量"};

                ImGui::TextUnformatted(text[ui_controller->m_language_index]);
            }
            ImGui::TableNextColumn();
            {
                constexpr char const *const items[LANGUAGE_COUNT][BRX_MOTION_PHYSICS_RAGDOLL_QUALITY_COUNT] = {
                    {"Disable", "Low", "Medium", "High"},
                    {"無効", "低", "中", "高"},
                    {"停用", "低", "中", "高"},
                    {"停用", "低", "中", "高"}};

                int select_physics_ragdoll_quality = std::min(std::max(0, static_cast<int>(ui_controller->m_physics_ragdoll_quality)), (int(BRX_MOTION_PHYSICS_RAGDOLL_QUALITY_COUNT) - 1));

                ImGui::Combo("##Physics-Ragdoll-Manager-Table-Value-Quality", &select_physics_ragdoll_quality, items[ui_controller->m_language_index], IM_ARRAYSIZE(items[0]));

                ui_controller->m_physics_ragdoll_quality = static_cast<BRX_MOTION_PHYSICS_RAGDOLL_QUALITY>(std::min(std::max(0, select_physics_ragdoll_quality), (int(BRX_MOTION_PHYSICS_RAGDOLL_QUALITY_COUNT) - 1)));
            }

            ImGui::EndTable();
        }

        ImGui::End();
    }

    if (ui_controller->m_show_window_manager)
    {
        {
            ImGuiViewport const *const main_viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + ui_x * ui_index, main_viewport->WorkPos.y + ui_y * ui_index), ImGuiCond_FirstUseEver);
            ++ui_index;

            ImGui::SetNextWindowSize(ImVec2(ui_width, ui_height), ImGuiCond_FirstUseEver);
        }

        ImGui::Begin("Window Manager", &ui_controller->m_show_window_manager);

        {
            constexpr char const *const text[LANGUAGE_COUNT] = {
                "Window Manager",
                "窓管理",
                "視窗管理",
                "窗口管理"};
            ImGui::TextUnformatted(text[ui_controller->m_language_index]);
        }

        ImGui::Separator();

        if (ImGui::BeginTable("##Window-Manager-Table", 2, ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersInnerV))
        {
            ImGui::TableSetupColumn("##Window-Manager-Table-Property", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("##Window-Manager-Table-Value", ImGuiTableColumnFlags_WidthStretch);

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            {
                constexpr char const *const text[LANGUAGE_COUNT] = {
                    "Resolution",
                    "解像度",
                    "解析度",
                    "分辨率"};

                ImGui::TextUnformatted(text[ui_controller->m_language_index]);
            }
            ImGui::TableNextColumn();
            {
                constexpr char const *const items[] = {
                    "SD Landscape (640 x 480)",
                    "SD Portrait (480 x 640)",
                    "qHD Landscape (960 x 540)",
                    "qHD Portrait (540 x 960)",
                    "HD Landscape (1280 x 720)",
                    "HD Portrait (720 x 1280)",
                    "HD+ Landscape (1600 x 900)",
                    "HD+ Portrait (900 x 1600)",
                    "FHD Landscape (1920 x 1080)",
                    "FHD Portrait (1080 x 1920)"};

                constexpr int const resolutions[][2] = {
                    {640, 480},
                    {480, 640},
                    {960, 540},
                    {540, 960},
                    {1280, 720},
                    {720, 1280},
                    {1600, 900},
                    {900, 1600},
                    {1920, 1080},
                    {1080, 1920}};

                static_assert((sizeof(items) / sizeof(items[0])) == (sizeof(resolutions) / sizeof(resolutions[0])), "");

                int main_window_width;
                int main_window_height;
                brx_wsi_get_main_window_size(&main_window_width, &main_window_height);

                int select_main_window_resolution_index = 0;

                while (select_main_window_resolution_index < static_cast<int>(sizeof(resolutions) / sizeof(resolutions[0])))
                {
                    if ((resolutions[select_main_window_resolution_index][0] >= main_window_width) && (resolutions[select_main_window_resolution_index][1] >= main_window_height))
                    {
                        break;
                    }

                    ++select_main_window_resolution_index;
                }

                select_main_window_resolution_index = std::min(std::max(0, select_main_window_resolution_index), static_cast<int>(sizeof(items) / sizeof(items[0])) - 1);

                ImGui::Combo("##Window-Manager-Table-Value-Resolution", &select_main_window_resolution_index, items, IM_ARRAYSIZE(items));

                select_main_window_resolution_index = std::min(std::max(0, select_main_window_resolution_index), static_cast<int>(sizeof(items) / sizeof(items[0])) - 1);

                int const main_window_new_width = resolutions[select_main_window_resolution_index][0];
                int const main_window_new_height = resolutions[select_main_window_resolution_index][1];

                if ((main_window_width != main_window_new_width) || (main_window_height != main_window_new_height))
                {
                    brx_wsi_set_main_window_size(main_window_new_width, main_window_new_height);
                }
            }

            ImGui::EndTable();
        }

        ImGui::End();
    }

    if (ui_controller->m_show_area_lighting_manager)
    {
        {
            ImGuiViewport const *const main_viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + ui_x * ui_index, main_viewport->WorkPos.y + ui_y * ui_index), ImGuiCond_FirstUseEver);
            ++ui_index;

            ImGui::SetNextWindowSize(ImVec2(ui_width, ui_height), ImGuiCond_FirstUseEver);
        }

        ImGui::Begin("Area Lighting Manager", &ui_controller->m_show_area_lighting_manager);

        {
            constexpr char const *const text[LANGUAGE_COUNT] = {
                "Area Lighting Manager",
                "領域照明管理",
                "區域照明管理",
                "区域光照管理"};
            ImGui::TextUnformatted(text[ui_controller->m_language_index]);
        }

        ImGui::Separator();

        if (ImGui::BeginTable("##Area-Lighting-Manager-Configuration-Table", 2, ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersInnerV))
        {
            ImGui::TableSetupColumn("##Area-Lighting-Manager-Configuration-Table-Property", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("##Area-Lighting-Manager-Configuration-Table-Value", ImGuiTableColumnFlags_WidthStretch);

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            {
                constexpr char const *text[LANGUAGE_COUNT] = {
                    "Enable Debug Renderer",
                    "Debug Renderer 有効",
                    "啟用調試渲染",
                    "启用调试渲染"};
                ImGui::TextUnformatted(text[ui_controller->m_language_index]);
            }

            ImGui::TableNextColumn();
            {
                bool enable_debug_renderer = device->get_quad_lights_enable_debug_renderer();

                ImGui::Checkbox("##Area-Lighting-Manager-Configuration-Table-Value-Enable-Debug-Renderer", &enable_debug_renderer);

                device->set_quad_lights_enable_debug_renderer(enable_debug_renderer);
            }

            ImGui::SameLine();

            {
                ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                ImGui::TextUnformatted(help_marker_text);
                ImGui::PopStyleColor();
                if (ImGui::BeginItemTooltip())
                {
                    ImGui::PushTextWrapPos(help_marker_position);
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "You can view the emissive surface representing the light source.",
                        "光源を表現する発光面を閲覧できます。",
                        "你可以查看代表光源的發光表面。",
                        "你可以查看代表光源的发光表面。"};
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                    ImGui::PopTextWrapPos();
                    ImGui::EndTooltip();
                }
            }

            ImGui::EndTable();
        }

        ImGui::Separator();

        if (ImGui::TreeNodeEx("##Area-Lighting-Manager-New-Node", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_NoAutoOpenOnLog))
        {
            if (ImGui::BeginTable("##Area-Lighting-Manager-New-Table", 2, ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersInnerV))
            {
                ImGui::TableSetupColumn("##Area-Lighting-Manager-New-Table-Property", ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableSetupColumn("##Area-Lighting-Manager-New-Table-Value", ImGuiTableColumnFlags_WidthStretch);

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Name",
                        "名称",
                        "名稱",
                        "名称"};
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {
                    constexpr char const *const hint[LANGUAGE_COUNT] = {
                        "New Area Lighting Name",
                        "新規領域照明名前",
                        "新建區域照明名稱",
                        "新建区域光照名称"};
                    ImGui::InputTextWithHint("##Area-Lighting-Manager-New-Area-Lighting-Name", hint[ui_controller->m_language_index], ui_controller->m_new_area_lighting_name.data(), ui_controller->m_new_area_lighting_name.size());
                }

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Color",
                        "顔色",
                        "顏色",
                        "颜色"};
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {
                    float new_area_lighting_color_rgb[3] = {ui_controller->m_new_area_lighting_color_r, ui_controller->m_new_area_lighting_color_g, ui_controller->m_new_area_lighting_color_b};

                    ImGui::ColorEdit3("##Area-Lighting-Manager-New-Area-Lighting-Color", new_area_lighting_color_rgb);

                    ui_controller->m_new_area_lighting_color_r = new_area_lighting_color_rgb[0];
                    ui_controller->m_new_area_lighting_color_g = new_area_lighting_color_rgb[1];
                    ui_controller->m_new_area_lighting_color_b = new_area_lighting_color_rgb[2];
                }

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Radiance",
                        "放射輝度",
                        "輻射亮度",
                        "辐射亮度"};
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {
                    char radiance_text[256];
#define _INTERNAL_BRX_STRINGIZING(string) #string
#define _INTERNAL_BRX_X_STRINGIZING(string) _INTERNAL_BRX_STRINGIZING(string)
                    std::snprintf(&radiance_text[0], sizeof(radiance_text) / sizeof(radiance_text[0]), "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f", ui_controller->m_new_area_lighting_radiance);
#undef _INTERNAL_BRX_STRINGIZING
#undef _INTERNAL_BRX_X_STRINGIZING
                    radiance_text[(sizeof(radiance_text) / sizeof(radiance_text[0]) - 1)] = '\0';

                    ImGui::InputText("##Area-Lighting-Manager-New-Area-Lighting-Radiance", &radiance_text[0], sizeof(radiance_text) / sizeof(radiance_text[0]), ImGuiInputTextFlags_CharsDecimal);

                    ui_controller->m_new_area_lighting_radiance = std::strtof(radiance_text, NULL);
                }

                {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::AlignTextToFramePadding();
                    {
                        constexpr char const *const text[LANGUAGE_COUNT] = {
                            "Position X",
                            "位置 X",
                            "位置 X",
                            "位置 X"};
                        ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                    }
                    ImGui::TableNextColumn();
                    {
                        char position_text[256];
#define _INTERNAL_BRX_STRINGIZING(string) #string
#define _INTERNAL_BRX_X_STRINGIZING(string) _INTERNAL_BRX_STRINGIZING(string)
                        std::snprintf(&position_text[0], sizeof(position_text) / sizeof(position_text[0]), "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f", ui_controller->m_new_area_lighting_position_x);
#undef _INTERNAL_BRX_STRINGIZING
#undef _INTERNAL_BRX_X_STRINGIZING
                        position_text[(sizeof(position_text) / sizeof(position_text[0]) - 1)] = '\0';

                        ImGui::InputText("##Area-Lighting-Manager-New-Area-Lighting-Position-X", &position_text[0], sizeof(position_text) / sizeof(position_text[0]), ImGuiInputTextFlags_CharsDecimal);

                        ui_controller->m_new_area_lighting_position_x = std::strtof(position_text, NULL);
                    }

                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::AlignTextToFramePadding();
                    {
                        constexpr char const *const text[LANGUAGE_COUNT] = {
                            "Position Y",
                            "位置 Y",
                            "位置 Y",
                            "位置 Y"};
                        ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                    }
                    ImGui::TableNextColumn();
                    {
                        char position_text[256];
#define _INTERNAL_BRX_STRINGIZING(string) #string
#define _INTERNAL_BRX_X_STRINGIZING(string) _INTERNAL_BRX_STRINGIZING(string)
                        std::snprintf(&position_text[0], sizeof(position_text) / sizeof(position_text[0]), "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f", ui_controller->m_new_area_lighting_position_y);
#undef _INTERNAL_BRX_STRINGIZING
#undef _INTERNAL_BRX_X_STRINGIZING
                        position_text[(sizeof(position_text) / sizeof(position_text[0]) - 1)] = '\0';

                        ImGui::InputText("##Area-Lighting-Manager-New-Area-Lighting-Position-Y", &position_text[0], sizeof(position_text) / sizeof(position_text[0]), ImGuiInputTextFlags_CharsDecimal);

                        ui_controller->m_new_area_lighting_position_y = std::strtof(position_text, NULL);
                    }

                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::AlignTextToFramePadding();
                    {
                        constexpr char const *const text[LANGUAGE_COUNT] = {
                            "Position Z",
                            "位置 Z",
                            "位置 Z",
                            "位置 Z"};
                        ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                    }
                    ImGui::TableNextColumn();
                    {
                        char position_text[256];
#define _INTERNAL_BRX_STRINGIZING(string) #string
#define _INTERNAL_BRX_X_STRINGIZING(string) _INTERNAL_BRX_STRINGIZING(string)
                        std::snprintf(&position_text[0], sizeof(position_text) / sizeof(position_text[0]), "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f", ui_controller->m_new_area_lighting_position_z);
#undef _INTERNAL_BRX_STRINGIZING
#undef _INTERNAL_BRX_X_STRINGIZING
                        position_text[(sizeof(position_text) / sizeof(position_text[0]) - 1)] = '\0';

                        ImGui::InputText("##Area-Lighting-Manager-New-Area-Lighting-Position-Z", &position_text[0], sizeof(position_text) / sizeof(position_text[0]), ImGuiInputTextFlags_CharsDecimal);

                        ui_controller->m_new_area_lighting_position_z = std::strtof(position_text, NULL);
                    }
                }

                {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::AlignTextToFramePadding();
                    {
                        constexpr char const *const text[LANGUAGE_COUNT] = {
                            "Edge1 X",
                            "側1 X",
                            "邊1 X",
                            "边1 X"};
                        ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                    }
                    ImGui::TableNextColumn();
                    {
                        char edge1_text[256];
#define _INTERNAL_BRX_STRINGIZING(string) #string
#define _INTERNAL_BRX_X_STRINGIZING(string) _INTERNAL_BRX_STRINGIZING(string)
                        std::snprintf(&edge1_text[0], sizeof(edge1_text) / sizeof(edge1_text[0]), "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f", ui_controller->m_new_area_lighting_edge1_x);
#undef _INTERNAL_BRX_STRINGIZING
#undef _INTERNAL_BRX_X_STRINGIZING
                        edge1_text[(sizeof(edge1_text) / sizeof(edge1_text[0]) - 1)] = '\0';

                        ImGui::InputText("##Area-Lighting-Manager-New-Area-Lighting-Edge1-X", &edge1_text[0], sizeof(edge1_text) / sizeof(edge1_text[0]), ImGuiInputTextFlags_CharsDecimal);

                        ui_controller->m_new_area_lighting_edge1_x = std::strtof(edge1_text, NULL);
                    }

                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::AlignTextToFramePadding();
                    {
                        constexpr char const *const text[LANGUAGE_COUNT] = {
                            "Edge1 Y",
                            "側1 Y",
                            "邊1 Y",
                            "边1 Y"};
                        ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                    }
                    ImGui::TableNextColumn();
                    {
                        char edge1_text[256];
#define _INTERNAL_BRX_STRINGIZING(string) #string
#define _INTERNAL_BRX_X_STRINGIZING(string) _INTERNAL_BRX_STRINGIZING(string)
                        std::snprintf(&edge1_text[0], sizeof(edge1_text) / sizeof(edge1_text[0]), "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f", ui_controller->m_new_area_lighting_edge1_y);
#undef _INTERNAL_BRX_STRINGIZING
#undef _INTERNAL_BRX_X_STRINGIZING
                        edge1_text[(sizeof(edge1_text) / sizeof(edge1_text[0]) - 1)] = '\0';

                        ImGui::InputText("##Area-Lighting-Manager-New-Area-Lighting-Edge1-Y", &edge1_text[0], sizeof(edge1_text) / sizeof(edge1_text[0]), ImGuiInputTextFlags_CharsDecimal);

                        ui_controller->m_new_area_lighting_edge1_y = std::strtof(edge1_text, NULL);
                    }

                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::AlignTextToFramePadding();
                    {
                        constexpr char const *const text[LANGUAGE_COUNT] = {
                            "Edge1 Z",
                            "側1 Z",
                            "邊1 Z",
                            "边1 Z"};
                        ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                    }
                    ImGui::TableNextColumn();
                    {
                        char edge1_text[256];
#define _INTERNAL_BRX_STRINGIZING(string) #string
#define _INTERNAL_BRX_X_STRINGIZING(string) _INTERNAL_BRX_STRINGIZING(string)
                        std::snprintf(&edge1_text[0], sizeof(edge1_text) / sizeof(edge1_text[0]), "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f", ui_controller->m_new_area_lighting_edge1_z);
#undef _INTERNAL_BRX_STRINGIZING
#undef _INTERNAL_BRX_X_STRINGIZING
                        edge1_text[(sizeof(edge1_text) / sizeof(edge1_text[0]) - 1)] = '\0';

                        ImGui::InputText("##Area-Lighting-Manager-New-Area-Lighting-Edge1-Z", &edge1_text[0], sizeof(edge1_text) / sizeof(edge1_text[0]), ImGuiInputTextFlags_CharsDecimal);

                        ui_controller->m_new_area_lighting_edge1_z = std::strtof(edge1_text, NULL);
                    }
                }

                {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::AlignTextToFramePadding();
                    {
                        constexpr char const *const text[LANGUAGE_COUNT] = {
                            "Edge2 X",
                            "側2 X",
                            "邊2 X",
                            "边2 X"};
                        ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                    }
                    ImGui::TableNextColumn();
                    {
                        char edge2_text[256];
#define _INTERNAL_BRX_STRINGIZING(string) #string
#define _INTERNAL_BRX_X_STRINGIZING(string) _INTERNAL_BRX_STRINGIZING(string)
                        std::snprintf(&edge2_text[0], sizeof(edge2_text) / sizeof(edge2_text[0]), "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f", ui_controller->m_new_area_lighting_edge2_x);
#undef _INTERNAL_BRX_STRINGIZING
#undef _INTERNAL_BRX_X_STRINGIZING
                        edge2_text[(sizeof(edge2_text) / sizeof(edge2_text[0]) - 1)] = '\0';

                        ImGui::InputText("##Area-Lighting-Manager-New-Area-Lighting-Edge2-X", &edge2_text[0], sizeof(edge2_text) / sizeof(edge2_text[0]), ImGuiInputTextFlags_CharsDecimal);

                        ui_controller->m_new_area_lighting_edge2_x = std::strtof(edge2_text, NULL);
                    }

                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::AlignTextToFramePadding();
                    {
                        constexpr char const *const text[LANGUAGE_COUNT] = {
                            "Edge2 Y",
                            "側2 Y",
                            "邊2 Y",
                            "边2 Y"};
                        ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                    }
                    ImGui::TableNextColumn();
                    {
                        char edge2_text[256];
#define _INTERNAL_BRX_STRINGIZING(string) #string
#define _INTERNAL_BRX_X_STRINGIZING(string) _INTERNAL_BRX_STRINGIZING(string)
                        std::snprintf(&edge2_text[0], sizeof(edge2_text) / sizeof(edge2_text[0]), "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f", ui_controller->m_new_area_lighting_edge2_y);
#undef _INTERNAL_BRX_STRINGIZING
#undef _INTERNAL_BRX_X_STRINGIZING
                        edge2_text[(sizeof(edge2_text) / sizeof(edge2_text[0]) - 1)] = '\0';

                        ImGui::InputText("##Area-Lighting-Manager-New-Area-Lighting-Edge2-Y", &edge2_text[0], sizeof(edge2_text) / sizeof(edge2_text[0]), ImGuiInputTextFlags_CharsDecimal);

                        ui_controller->m_new_area_lighting_edge2_y = std::strtof(edge2_text, NULL);
                    }

                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::AlignTextToFramePadding();
                    {
                        constexpr char const *const text[LANGUAGE_COUNT] = {
                            "Edge2 Z",
                            "側2 Z",
                            "邊2 Z",
                            "边2 Z"};
                        ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                    }
                    ImGui::TableNextColumn();
                    {
                        char edge2_text[256];
#define _INTERNAL_BRX_STRINGIZING(string) #string
#define _INTERNAL_BRX_X_STRINGIZING(string) _INTERNAL_BRX_STRINGIZING(string)
                        std::snprintf(&edge2_text[0], sizeof(edge2_text) / sizeof(edge2_text[0]), "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f", ui_controller->m_new_area_lighting_edge2_z);
#undef _INTERNAL_BRX_STRINGIZING
#undef _INTERNAL_BRX_X_STRINGIZING
                        edge2_text[(sizeof(edge2_text) / sizeof(edge2_text[0]) - 1)] = '\0';

                        ImGui::InputText("##Area-Lighting-Manager-New-Area-Lighting-Edge2-Z", &edge2_text[0], sizeof(edge2_text) / sizeof(edge2_text[0]), ImGuiInputTextFlags_CharsDecimal);

                        ui_controller->m_new_area_lighting_edge2_z = std::strtof(edge2_text, NULL);
                    }
                }

                ImGui::EndTable();
            }

            ImGui::TreePop();
        }

        {
            {
                constexpr char const *const text[LANGUAGE_COUNT] = {
                    "New",
                    "新規作成",
                    "新建",
                    "新建"};
                ImGui::TextUnformatted(text[ui_controller->m_language_index]);
            }

            ImGui::SameLine();

            if (ImGui::Button("N##Area-Lighting-Manager-New-Button"))
            {
                mcrt_string const name = ui_controller->m_new_area_lighting_name.data();

                float const color_r = ui_controller->m_new_area_lighting_color_r;
                float const color_g = ui_controller->m_new_area_lighting_color_g;
                float const color_b = ui_controller->m_new_area_lighting_color_b;

                float const radiance = ui_controller->m_new_area_lighting_radiance;

                float const position_x = ui_controller->m_new_area_lighting_position_x;
                float const position_y = ui_controller->m_new_area_lighting_position_y;
                float const position_z = ui_controller->m_new_area_lighting_position_z;

                float const edge1_x = ui_controller->m_new_area_lighting_edge1_x;
                float const edge1_y = ui_controller->m_new_area_lighting_edge1_y;
                float const edge1_z = ui_controller->m_new_area_lighting_edge1_z;

                float const edge2_x = ui_controller->m_new_area_lighting_edge2_x;
                float const edge2_y = ui_controller->m_new_area_lighting_edge2_y;
                float const edge2_z = ui_controller->m_new_area_lighting_edge2_z;

                uint64_t const timestamp = mcrt_tick_count_now();

                mcrt_string area_lighting_identity;
                {
                    char area_lighting_timestamp_text[] = {"18446744073709551615"};
                    std::snprintf(area_lighting_timestamp_text, sizeof(area_lighting_timestamp_text) / sizeof(area_lighting_timestamp_text[0]), "%020llu", static_cast<long long unsigned>(timestamp));
                    area_lighting_timestamp_text[(sizeof(area_lighting_timestamp_text) / sizeof(area_lighting_timestamp_text[0])) - 1] = '\0';

                    area_lighting_identity += area_lighting_identity;
                    area_lighting_identity += ' ';
                    area_lighting_identity += name;
                }

                // should always NOT already exist in practice
                assert(ui_model->m_area_lightings.end() == ui_model->m_area_lightings.find(timestamp));

                ui_model->m_area_lightings.insert(ui_model->m_area_lightings.end(), std::pair<uint64_t, ui_area_lighting_model_t>{timestamp, ui_area_lighting_model_t{name, color_r, color_g, color_b, radiance, position_x, position_y, position_z, edge1_x, edge1_y, edge1_z, edge2_x, edge2_y, edge2_z}});
            }
        }

        ImGui::Separator();

        {

            {
                constexpr char const *const text[LANGUAGE_COUNT] = {
                    "Delete",
                    "削除",
                    "刪除",
                    "删除"};
                ImGui::TextUnformatted(text[ui_controller->m_language_index]);
            }

            ImGui::SameLine();

            if (ImGui::Button("X##Area-Lighting-Manager-Delete-Button"))
            {
                auto const found_area_lighting = ui_model->m_area_lightings.find(ui_controller->m_tree_view_selected_area_lighting);

                if (ui_model->m_area_lightings.end() != found_area_lighting)
                {
                    ui_model->m_area_lightings.erase(found_area_lighting);

                    ui_controller->m_tree_view_selected_area_lighting = INVALID_TIMESTAMP;
                }
                else
                {
                    assert(INVALID_TIMESTAMP == ui_controller->m_tree_view_selected_area_lighting);
                }
            }
        }

        ImGui::Separator();

        if (ImGui::BeginChild("##Area-Lighting-Manager-Left-Child", ImVec2(ui_width * left_child_width_ratio, 0.0F), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX))
        {
            ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::PushItemFlag(ImGuiItemFlags_NoNavDefaultFocus, true);
            ImGuiTextFilter text_filter;
            {
                constexpr char const *const hint[LANGUAGE_COUNT] = {
                    "Search",
                    "検索",
                    "檢索",
                    "检索"};
                if (ImGui::InputTextWithHint("##Area-Lighting-Manager-Left-Child-Text-Filter", hint[ui_controller->m_language_index], text_filter.InputBuf, IM_ARRAYSIZE(text_filter.InputBuf), ImGuiInputTextFlags_EscapeClearsAll))
                {
                    text_filter.Build();
                }
            }

            ImGui::PopItemFlag();

            if (ImGui::BeginTable("##Area-Lighting-Manager-Left-Child-Table", 1, ImGuiTableFlags_RowBg))
            {
                for (auto const &area_lighting : ui_model->m_area_lightings)
                {
                    mcrt_string area_lighting_identity;
                    {
                        char area_lighting_timestamp_text[] = {"18446744073709551615"};
                        std::snprintf(area_lighting_timestamp_text, sizeof(area_lighting_timestamp_text) / sizeof(area_lighting_timestamp_text[0]), "%020llu", static_cast<long long unsigned>(area_lighting.first));
                        area_lighting_timestamp_text[(sizeof(area_lighting_timestamp_text) / sizeof(area_lighting_timestamp_text[0])) - 1] = '\0';

                        area_lighting_identity += area_lighting_timestamp_text;
                        area_lighting_identity += ' ';
                        area_lighting_identity += area_lighting.second.m_name;
                    }

                    if (text_filter.PassFilter(area_lighting_identity.c_str()))
                    {
                        ImGui::TableNextRow();
                        ImGui::TableNextColumn();

                        ImGuiTreeNodeFlags const flags = ((ui_controller->m_tree_view_selected_area_lighting != area_lighting.first)) ? ImGuiTreeNodeFlags_Leaf : (ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Selected);

                        bool const node_open = ImGui::TreeNodeEx(area_lighting_identity.c_str(), flags);

                        if (ImGui::IsItemFocused())
                        {
                            ui_controller->m_tree_view_selected_area_lighting = area_lighting.first;
                        }

                        if (node_open)
                        {
                            ImGui::TreePop();
                        }
                    }
                }

                ImGui::EndTable();
            }
        }
        ImGui::EndChild();

        auto const found_area_lighting = ui_model->m_area_lightings.find(ui_controller->m_tree_view_selected_area_lighting);

        if (ui_model->m_area_lightings.end() != found_area_lighting)
        {
            ImGui::SameLine();

            ImGui::BeginGroup();

            {
                ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                ImGui::TextUnformatted(found_area_lighting->second.m_name.c_str());
                ImGui::PopStyleColor();
            }

            ImGui::Separator();

            if (ImGui::BeginTable("##Area-Lighting-Manager-Right-Group-Table", 2, ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersInnerV))
            {
                ImGui::TableSetupColumn("##Area-Lighting-Manager-Right-Group-Table-Property", ImGuiTableColumnFlags_WidthFixed);
                ImGui::TableSetupColumn("##Area-Lighting-Manager-Right-Group-Table-Value", ImGuiTableColumnFlags_WidthStretch);

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Color",
                        "顔色",
                        "顏色",
                        "颜色"};
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {
                    float area_lighting_color_rgb[3] = {found_area_lighting->second.m_color_r, found_area_lighting->second.m_color_g, found_area_lighting->second.m_color_b};

                    ImGui::ColorEdit3("##Area-Lighting-Manager-Right-Group-Table-Value-Color", area_lighting_color_rgb);

                    found_area_lighting->second.m_color_r = area_lighting_color_rgb[0];
                    found_area_lighting->second.m_color_g = area_lighting_color_rgb[1];
                    found_area_lighting->second.m_color_b = area_lighting_color_rgb[2];
                }

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Radiance",
                        "放射輝度",
                        "輻射亮度",
                        "辐射亮度"};
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {
                    char radiance_text[256];
#define _INTERNAL_BRX_STRINGIZING(string) #string
#define _INTERNAL_BRX_X_STRINGIZING(string) _INTERNAL_BRX_STRINGIZING(string)
                    std::snprintf(&radiance_text[0], sizeof(radiance_text) / sizeof(radiance_text[0]), "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f", found_area_lighting->second.m_radiance);
#undef _INTERNAL_BRX_STRINGIZING
#undef _INTERNAL_BRX_X_STRINGIZING
                    radiance_text[(sizeof(radiance_text) / sizeof(radiance_text[0]) - 1)] = '\0';

                    ImGui::InputText("##Area-Lighting-Manager-Right-Group-Table-Value-Radiance", &radiance_text[0], sizeof(radiance_text) / sizeof(radiance_text[0]), ImGuiInputTextFlags_CharsDecimal);

                    found_area_lighting->second.m_radiance = std::strtof(radiance_text, NULL);
                }

                {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::AlignTextToFramePadding();

                    {
                        constexpr char const *const text[LANGUAGE_COUNT] = {
                            "Position X",
                            "位置 X",
                            "位置 X",
                            "位置 X"};
                        ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                    }

                    ImGui::TableNextColumn();

                    {
                        char position_text[256];
#define _INTERNAL_BRX_STRINGIZING(string) #string
#define _INTERNAL_BRX_X_STRINGIZING(string) _INTERNAL_BRX_STRINGIZING(string)
                        std::snprintf(&position_text[0], sizeof(position_text) / sizeof(position_text[0]), "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f", found_area_lighting->second.m_position_x);
#undef _INTERNAL_BRX_STRINGIZING
#undef _INTERNAL_BRX_X_STRINGIZING
                        position_text[(sizeof(position_text) / sizeof(position_text[0]) - 1)] = '\0';

                        ImGui::InputText("##Area-Lighting-Manager-Right-Group-Table-Value-Position-X", &position_text[0], sizeof(position_text) / sizeof(position_text[0]), ImGuiInputTextFlags_CharsDecimal);

                        found_area_lighting->second.m_position_x = std::strtof(position_text, NULL);
                    }

                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::AlignTextToFramePadding();

                    {
                        constexpr char const *const text[LANGUAGE_COUNT] = {
                            "Position Y",
                            "位置 Y",
                            "位置 Y",
                            "位置 Y"};
                        ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                    }

                    ImGui::TableNextColumn();

                    {
                        char position_text[256];
#define _INTERNAL_BRX_STRINGIZING(string) #string
#define _INTERNAL_BRX_X_STRINGIZING(string) _INTERNAL_BRX_STRINGIZING(string)
                        std::snprintf(&position_text[0], sizeof(position_text) / sizeof(position_text[0]), "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f", found_area_lighting->second.m_position_y);
#undef _INTERNAL_BRX_STRINGIZING
#undef _INTERNAL_BRX_X_STRINGIZING
                        position_text[(sizeof(position_text) / sizeof(position_text[0]) - 1)] = '\0';

                        ImGui::InputText("##Area-Lighting-Manager-Right-Group-Table-Value-Position-Y", &position_text[0], sizeof(position_text) / sizeof(position_text[0]), ImGuiInputTextFlags_CharsDecimal);

                        found_area_lighting->second.m_position_y = std::strtof(position_text, NULL);
                    }

                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::AlignTextToFramePadding();

                    {
                        constexpr char const *const text[LANGUAGE_COUNT] = {
                            "Position Z",
                            "位置 Z",
                            "位置 Z",
                            "位置 Z"};
                        ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                    }

                    ImGui::TableNextColumn();

                    {
                        char position_text[256];
#define _INTERNAL_BRX_STRINGIZING(string) #string
#define _INTERNAL_BRX_X_STRINGIZING(string) _INTERNAL_BRX_STRINGIZING(string)
                        std::snprintf(&position_text[0], sizeof(position_text) / sizeof(position_text[0]), "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f", found_area_lighting->second.m_position_z);
#undef _INTERNAL_BRX_STRINGIZING
#undef _INTERNAL_BRX_X_STRINGIZING
                        position_text[(sizeof(position_text) / sizeof(position_text[0]) - 1)] = '\0';

                        ImGui::InputText("##Area-Lighting-Manager-Right-Group-Table-Value-Position-Z", &position_text[0], sizeof(position_text) / sizeof(position_text[0]), ImGuiInputTextFlags_CharsDecimal);

                        found_area_lighting->second.m_position_z = std::strtof(position_text, NULL);
                    }
                }

                {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::AlignTextToFramePadding();
                    {
                        constexpr char const *const text[LANGUAGE_COUNT] = {
                            "Edge1 X",
                            "側1 X",
                            "邊1 X",
                            "边1 X"};
                        ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                    }
                    ImGui::TableNextColumn();
                    {
                        char edge1_text[256];
#define _INTERNAL_BRX_STRINGIZING(string) #string
#define _INTERNAL_BRX_X_STRINGIZING(string) _INTERNAL_BRX_STRINGIZING(string)
                        std::snprintf(&edge1_text[0], sizeof(edge1_text) / sizeof(edge1_text[0]), "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f", found_area_lighting->second.m_edge1_x);
#undef _INTERNAL_BRX_STRINGIZING
#undef _INTERNAL_BRX_X_STRINGIZING
                        edge1_text[(sizeof(edge1_text) / sizeof(edge1_text[0]) - 1)] = '\0';

                        ImGui::InputText("##Area-Lighting-Manager-Right-Group-Table-Value-Edge1-X", &edge1_text[0], sizeof(edge1_text) / sizeof(edge1_text[0]), ImGuiInputTextFlags_CharsDecimal);

                        found_area_lighting->second.m_edge1_x = std::strtof(edge1_text, NULL);
                    }

                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::AlignTextToFramePadding();
                    {
                        constexpr char const *const text[LANGUAGE_COUNT] = {
                            "Edge1 Y",
                            "側1 Y",
                            "邊1 Y",
                            "边1 Y"};
                        ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                    }
                    ImGui::TableNextColumn();
                    {
                        char edge1_text[256];
#define _INTERNAL_BRX_STRINGIZING(string) #string
#define _INTERNAL_BRX_X_STRINGIZING(string) _INTERNAL_BRX_STRINGIZING(string)
                        std::snprintf(&edge1_text[0], sizeof(edge1_text) / sizeof(edge1_text[0]), "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f", found_area_lighting->second.m_edge1_y);
#undef _INTERNAL_BRX_STRINGIZING
#undef _INTERNAL_BRX_X_STRINGIZING
                        edge1_text[(sizeof(edge1_text) / sizeof(edge1_text[0]) - 1)] = '\0';

                        ImGui::InputText("##Area-Lighting-Manager-Right-Group-Table-Value-Edge1-Y", &edge1_text[0], sizeof(edge1_text) / sizeof(edge1_text[0]), ImGuiInputTextFlags_CharsDecimal);

                        found_area_lighting->second.m_edge1_y = std::strtof(edge1_text, NULL);
                    }

                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::AlignTextToFramePadding();
                    {
                        constexpr char const *const text[LANGUAGE_COUNT] = {
                            "Edge1 Z",
                            "側1 Z",
                            "邊1 Z",
                            "边1 Z"};
                        ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                    }
                    ImGui::TableNextColumn();
                    {
                        char edge1_text[256];
#define _INTERNAL_BRX_STRINGIZING(string) #string
#define _INTERNAL_BRX_X_STRINGIZING(string) _INTERNAL_BRX_STRINGIZING(string)
                        std::snprintf(&edge1_text[0], sizeof(edge1_text) / sizeof(edge1_text[0]), "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f", found_area_lighting->second.m_edge1_z);
#undef _INTERNAL_BRX_STRINGIZING
#undef _INTERNAL_BRX_X_STRINGIZING
                        edge1_text[(sizeof(edge1_text) / sizeof(edge1_text[0]) - 1)] = '\0';

                        ImGui::InputText("##Area-Lighting-Manager-Right-Group-Table-Value-Edge1-Z", &edge1_text[0], sizeof(edge1_text) / sizeof(edge1_text[0]), ImGuiInputTextFlags_CharsDecimal);

                        found_area_lighting->second.m_edge1_z = std::strtof(edge1_text, NULL);
                    }
                }

                {
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::AlignTextToFramePadding();
                    {
                        constexpr char const *const text[LANGUAGE_COUNT] = {
                            "Edge2 X",
                            "側2 X",
                            "邊2 X",
                            "边2 X"};
                        ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                    }
                    ImGui::TableNextColumn();
                    {
                        char edge2_text[256];
#define _INTERNAL_BRX_STRINGIZING(string) #string
#define _INTERNAL_BRX_X_STRINGIZING(string) _INTERNAL_BRX_STRINGIZING(string)
                        std::snprintf(&edge2_text[0], sizeof(edge2_text) / sizeof(edge2_text[0]), "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f", found_area_lighting->second.m_edge2_x);
#undef _INTERNAL_BRX_STRINGIZING
#undef _INTERNAL_BRX_X_STRINGIZING
                        edge2_text[(sizeof(edge2_text) / sizeof(edge2_text[0]) - 1)] = '\0';

                        ImGui::InputText("##Area-Lighting-Manager-Right-Group-Table-Value-Edge2-X", &edge2_text[0], sizeof(edge2_text) / sizeof(edge2_text[0]), ImGuiInputTextFlags_CharsDecimal);

                        found_area_lighting->second.m_edge2_x = std::strtof(edge2_text, NULL);
                    }

                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::AlignTextToFramePadding();
                    {
                        constexpr char const *const text[LANGUAGE_COUNT] = {
                            "Edge2 Y",
                            "側2 Y",
                            "邊2 Y",
                            "边2 Y"};
                        ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                    }
                    ImGui::TableNextColumn();
                    {
                        char edge2_text[256];
#define _INTERNAL_BRX_STRINGIZING(string) #string
#define _INTERNAL_BRX_X_STRINGIZING(string) _INTERNAL_BRX_STRINGIZING(string)
                        std::snprintf(&edge2_text[0], sizeof(edge2_text) / sizeof(edge2_text[0]), "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f", found_area_lighting->second.m_edge2_y);
#undef _INTERNAL_BRX_STRINGIZING
#undef _INTERNAL_BRX_X_STRINGIZING
                        edge2_text[(sizeof(edge2_text) / sizeof(edge2_text[0]) - 1)] = '\0';

                        ImGui::InputText("##Area-Lighting-Manager-Right-Group-Table-Value-Edge2-Y", &edge2_text[0], sizeof(edge2_text) / sizeof(edge2_text[0]), ImGuiInputTextFlags_CharsDecimal);

                        found_area_lighting->second.m_edge2_y = std::strtof(edge2_text, NULL);
                    }

                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::AlignTextToFramePadding();
                    {
                        constexpr char const *const text[LANGUAGE_COUNT] = {
                            "Edge2 Z",
                            "側2 Z",
                            "邊2 Z",
                            "边2 Z"};
                        ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                    }
                    ImGui::TableNextColumn();
                    {
                        char edge2_text[256];
#define _INTERNAL_BRX_STRINGIZING(string) #string
#define _INTERNAL_BRX_X_STRINGIZING(string) _INTERNAL_BRX_STRINGIZING(string)
                        std::snprintf(&edge2_text[0], sizeof(edge2_text) / sizeof(edge2_text[0]), "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f", found_area_lighting->second.m_edge2_z);
#undef _INTERNAL_BRX_STRINGIZING
#undef _INTERNAL_BRX_X_STRINGIZING
                        edge2_text[(sizeof(edge2_text) / sizeof(edge2_text[0]) - 1)] = '\0';

                        ImGui::InputText("##Area-Lighting-Manager-Right-Group-Table-Value-Edge2-Z", &edge2_text[0], sizeof(edge2_text) / sizeof(edge2_text[0]), ImGuiInputTextFlags_CharsDecimal);

                        found_area_lighting->second.m_edge2_z = std::strtof(edge2_text, NULL);
                    }
                }

                ImGui::EndTable();
            }

            ImGui::EndGroup();
        }

        ImGui::End();
    }

    if (ui_controller->m_show_environment_lighting_manager)
    {
        {
            ImGuiViewport const *const main_viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + ui_x * ui_index, main_viewport->WorkPos.y + ui_y * ui_index), ImGuiCond_FirstUseEver);
            ++ui_index;

            ImGui::SetNextWindowSize(ImVec2(ui_width, ui_height), ImGuiCond_FirstUseEver);
        }

        ImGui::Begin("Environment Lighting Manager", &ui_controller->m_show_environment_lighting_manager);

        {
            constexpr char const *const text[LANGUAGE_COUNT] = {
                "Environment Lighting Manager",
                "環境照明管理",
                "環境照明管理",
                "环境光照管理"};
            ImGui::TextUnformatted(text[ui_controller->m_language_index]);
        }

        ImGui::Separator();

        if (ImGui::BeginTable("##Environment-Lighting-Manager-Table", 2, ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersInnerV))
        {
            ImGui::TableSetupColumn("##Environment-Lighting-Manager-Table-Property", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("##Environment-Lighting-Manager-Table-Value", ImGuiTableColumnFlags_WidthStretch);

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            {
                constexpr char const *const text[LANGUAGE_COUNT] = {
                    "Asset Image",
                    "資源画像",
                    "資源图像",
                    "资源圖像"};
                ImGui::TextUnformatted(text[ui_controller->m_language_index]);
            }
            ImGui::TableNextColumn();
            {
                constexpr char const *const label = "##Environment-Lighting-Manager-Table-Select-Asset-Image";

                if (!ui_model->m_asset_images.empty())
                {
                    mcrt_vector<mcrt_string> item_strings(static_cast<size_t>(ui_model->m_asset_images.size() + 1U));
                    mcrt_vector<char const *> items(static_cast<size_t>(ui_model->m_asset_images.size() + 1U));
                    int selected_combo_index = 0;
                    {
                        size_t combo_index = 0U;

                        {
                            char const *const text[LANGUAGE_COUNT] = {
                                "Disable",
                                "無効",
                                "停用",
                                "停用"};

                            item_strings[combo_index] = text[ui_controller->m_language_index];

                            items[combo_index] = item_strings[combo_index].c_str();

                            if (ui_controller->m_hdri_selected_asset_image.empty())
                            {
                                assert(0 == selected_combo_index);
                                assert(0 == combo_index);

                                assert(device->hdri_light_get_radiance() == NULL);
                            }

                            ++combo_index;
                        }

                        for (auto const &asset_image : ui_model->m_asset_images)
                        {
                            item_strings[combo_index] = asset_image.first;
                            items[combo_index] = item_strings[combo_index].c_str();

                            if (ui_controller->m_hdri_selected_asset_image == item_strings[combo_index])
                            {
                                assert(0 == selected_combo_index);
                                assert((1U + combo_index) < static_cast<size_t>(INT_MAX));
                                selected_combo_index = static_cast<int>(combo_index);

                                assert(device->hdri_light_get_radiance() == asset_image.second.m_image);
                            }

                            ++combo_index;
                        }
                    }

                    ImGui::Combo(label, &selected_combo_index, items.data(), items.size());

                    if (selected_combo_index >= 0)
                    {
                        mcrt_string selected_asset_image = std::move(item_strings[selected_combo_index]);

                        if (ui_controller->m_hdri_selected_asset_image != selected_asset_image)
                        {
                            auto const found_asset_image = ui_model->m_asset_images.find(selected_asset_image);
                            if (ui_model->m_asset_images.end() != found_asset_image)
                            {
                                assert(NULL != found_asset_image->second.m_image);

                                device->hdri_light_set_radiance(found_asset_image->second.m_image);

                                ui_controller->m_hdri_selected_asset_image = std::move(selected_asset_image);
                            }
                            else
                            {
                                device->hdri_light_set_radiance(NULL);

                                ui_controller->m_hdri_selected_asset_image.clear();
                            }
                        }
                    }
                    else
                    {
                        assert(false);
                    }
                }
                else
                {
                    char const *const items[LANGUAGE_COUNT][1] = {
                        {"Disable"},
                        {"無効"},
                        {"停用"},
                        {"停用"}};

                    int selected_combo_index = 0;

                    ImGui::Combo(label, &selected_combo_index, items[ui_controller->m_language_index], IM_ARRAYSIZE(items[ui_controller->m_language_index]));

                    assert(0 == selected_combo_index);

                    device->hdri_light_set_radiance(NULL);

                    ui_controller->m_hdri_selected_asset_image.clear();
                }
            }

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            {
                constexpr char const *const text[LANGUAGE_COUNT] = {
                    "Layout",
                    "配置",
                    "佈局",
                    "布局"};

                ImGui::TextUnformatted(text[ui_controller->m_language_index]);

                ImGui::SameLine();

                {
                    ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
                    ImGui::TextUnformatted(help_marker_text);
                    ImGui::PopStyleColor();
                    if (ImGui::BeginItemTooltip())
                    {
                        ImGui::PushTextWrapPos(help_marker_position);
                        constexpr char const *const text[LANGUAGE_COUNT] = {
                            "You may refer to PBRT-V4 for more information about equirectangular mapping and octahedral mapping.",
                            "正距円筒図法及八面体図法詳細は PBRT-V4 参照。",
                            "你可以參考 PBRT-V4 以了解有關等矩形映射和八面體映射的更多資訊。",
                            "你可以参考 PBRT-V4 以了解有关等矩形映射和八面体映射的更多信息。"};
                        ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                        ImGui::PopTextWrapPos();
                        ImGui::EndTooltip();
                    }
                }
            }
            ImGui::TableNextColumn();
            {
                constexpr char const *const items[LANGUAGE_COUNT][BRX_ANARI_HDRI_LIGHT_LAYOUT_COUNT] = {
                    {"Undefined", "Equirectangular Mapping", "Octahedral Mapping"},
                    {"未定義", "正距円筒図法", "八面体図法"},
                    {"未定義", "等距矩形映射", "八面體映射"},
                    {"未定义", "等距矩形映射", "八面体映射"}};

                int select_layout = std::min(std::max(0, static_cast<int>(device->hdri_light_get_layout())), (int(BRX_ANARI_HDRI_LIGHT_LAYOUT_COUNT) - 1));

                ImGui::Combo("##Environment-Lighting-Manager-Table-Layout", &select_layout, items[ui_controller->m_language_index], IM_ARRAYSIZE(items[0]));

                device->hdri_light_set_layout(static_cast<BRX_ANARI_HDRI_LIGHT_LAYOUT>(std::min(std::max(0, select_layout), (int(BRX_ANARI_HDRI_LIGHT_LAYOUT_COUNT) - 1))));
            }

            {
                brx_anari_vec3 hdri_light_direction = device->hdri_light_get_direction();

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Direction X",
                        "方向 X",
                        "方向 X",
                        "方向 X"};
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {
                    char direction_text[256];
#define _INTERNAL_BRX_STRINGIZING(string) #string
#define _INTERNAL_BRX_X_STRINGIZING(string) _INTERNAL_BRX_STRINGIZING(string)
                    std::snprintf(&direction_text[0], sizeof(direction_text) / sizeof(direction_text[0]), "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f", hdri_light_direction.m_x);
#undef _INTERNAL_BRX_STRINGIZING
#undef _INTERNAL_BRX_X_STRINGIZING
                    direction_text[(sizeof(direction_text) / sizeof(direction_text[0]) - 1)] = '\0';

                    ImGui::InputText("##Environment-Lighting-Manager-Table-Value-Direction-X", &direction_text[0], sizeof(direction_text) / sizeof(direction_text[0]), ImGuiInputTextFlags_CharsDecimal);

                    hdri_light_direction.m_x = std::strtof(direction_text, NULL);
                }

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Direction Y",
                        "方向 Y",
                        "方向 Y",
                        "方向 Y"};
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {
                    char direction_text[256];
#define _INTERNAL_BRX_STRINGIZING(string) #string
#define _INTERNAL_BRX_X_STRINGIZING(string) _INTERNAL_BRX_STRINGIZING(string)
                    std::snprintf(&direction_text[0], sizeof(direction_text) / sizeof(direction_text[0]), "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f", hdri_light_direction.m_y);
#undef _INTERNAL_BRX_STRINGIZING
#undef _INTERNAL_BRX_X_STRINGIZING
                    direction_text[(sizeof(direction_text) / sizeof(direction_text[0]) - 1)] = '\0';

                    ImGui::InputText("##Environment-Lighting-Manager-Table-Value-Direction-Y", &direction_text[0], sizeof(direction_text) / sizeof(direction_text[0]), ImGuiInputTextFlags_CharsDecimal);

                    hdri_light_direction.m_y = std::strtof(direction_text, NULL);
                }

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Direction Z",
                        "方向 Z",
                        "方向 Z",
                        "方向 Z"};
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {
                    char direction_text[256];
#define _INTERNAL_BRX_STRINGIZING(string) #string
#define _INTERNAL_BRX_X_STRINGIZING(string) _INTERNAL_BRX_STRINGIZING(string)
                    std::snprintf(&direction_text[0], sizeof(direction_text) / sizeof(direction_text[0]), "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f", hdri_light_direction.m_z);
#undef _INTERNAL_BRX_STRINGIZING
#undef _INTERNAL_BRX_X_STRINGIZING
                    direction_text[(sizeof(direction_text) / sizeof(direction_text[0]) - 1)] = '\0';

                    ImGui::InputText("##Environment-Lighting-Manager-Table-Value-Direction-Z", &direction_text[0], sizeof(direction_text) / sizeof(direction_text[0]), ImGuiInputTextFlags_CharsDecimal);

                    hdri_light_direction.m_z = std::strtof(direction_text, NULL);
                }

                {
                    DirectX::XMFLOAT3 const directx_hdri_light_direction(hdri_light_direction.m_x, hdri_light_direction.m_y, hdri_light_direction.m_z);

                    DirectX::XMFLOAT3 directx_normalized_hdri_light_direction;
                    DirectX::XMStoreFloat3(&directx_normalized_hdri_light_direction, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&directx_hdri_light_direction)));
                    hdri_light_direction.m_x = directx_normalized_hdri_light_direction.x;
                    hdri_light_direction.m_y = directx_normalized_hdri_light_direction.y;
                    hdri_light_direction.m_z = directx_normalized_hdri_light_direction.z;
                }

                device->hdri_light_set_direction(hdri_light_direction);
            }

            {
                brx_anari_vec3 hdri_light_up = device->hdri_light_get_up();

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Up X",
                        "上 X",
                        "上 X",
                        "上 X"};
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {
                    char up_text[256];
#define _INTERNAL_BRX_STRINGIZING(string) #string
#define _INTERNAL_BRX_X_STRINGIZING(string) _INTERNAL_BRX_STRINGIZING(string)
                    std::snprintf(&up_text[0], sizeof(up_text) / sizeof(up_text[0]), "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f", hdri_light_up.m_x);
#undef _INTERNAL_BRX_STRINGIZING
#undef _INTERNAL_BRX_X_STRINGIZING
                    up_text[(sizeof(up_text) / sizeof(up_text[0]) - 1)] = '\0';

                    ImGui::InputText("##Environment-Lighting-Manager-Table-Value-Up-X", &up_text[0], sizeof(up_text) / sizeof(up_text[0]), ImGuiInputTextFlags_CharsDecimal);

                    hdri_light_up.m_x = std::strtof(up_text, NULL);
                }

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Up Y",
                        "上 Y",
                        "上 Y",
                        "上 Y"};
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {
                    char up_text[256];
#define _INTERNAL_BRX_STRINGIZING(string) #string
#define _INTERNAL_BRX_X_STRINGIZING(string) _INTERNAL_BRX_STRINGIZING(string)
                    std::snprintf(&up_text[0], sizeof(up_text) / sizeof(up_text[0]), "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f", hdri_light_up.m_y);
#undef _INTERNAL_BRX_STRINGIZING
#undef _INTERNAL_BRX_X_STRINGIZING
                    up_text[(sizeof(up_text) / sizeof(up_text[0]) - 1)] = '\0';

                    ImGui::InputText("##Environment-Lighting-Manager-Table-Value-Up-Y", &up_text[0], sizeof(up_text) / sizeof(up_text[0]), ImGuiInputTextFlags_CharsDecimal);

                    hdri_light_up.m_y = std::strtof(up_text, NULL);
                }

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::AlignTextToFramePadding();
                {
                    constexpr char const *const text[LANGUAGE_COUNT] = {
                        "Up Z",
                        "上 Z",
                        "上 Z",
                        "上 Z"};
                    ImGui::TextUnformatted(text[ui_controller->m_language_index]);
                }
                ImGui::TableNextColumn();
                {
                    char up_text[256];
#define _INTERNAL_BRX_STRINGIZING(string) #string
#define _INTERNAL_BRX_X_STRINGIZING(string) _INTERNAL_BRX_STRINGIZING(string)
                    std::snprintf(&up_text[0], sizeof(up_text) / sizeof(up_text[0]), "%." _INTERNAL_BRX_X_STRINGIZING(DBL_DIG) "f", hdri_light_up.m_z);
#undef _INTERNAL_BRX_STRINGIZING
#undef _INTERNAL_BRX_X_STRINGIZING
                    up_text[(sizeof(up_text) / sizeof(up_text[0]) - 1)] = '\0';

                    ImGui::InputText("##Environment-Lighting-Manager-Table-Value-Up-Z", &up_text[0], sizeof(up_text) / sizeof(up_text[0]), ImGuiInputTextFlags_CharsDecimal);

                    hdri_light_up.m_z = std::strtof(up_text, NULL);
                }

                {
                    DirectX::XMFLOAT3 const directx_hdri_light_up(hdri_light_up.m_x, hdri_light_up.m_y, hdri_light_up.m_z);

                    DirectX::XMFLOAT3 directx_normalized_hdri_light_up;
                    DirectX::XMStoreFloat3(&directx_normalized_hdri_light_up, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&directx_hdri_light_up)));
                    hdri_light_up.m_x = directx_normalized_hdri_light_up.x;
                    hdri_light_up.m_y = directx_normalized_hdri_light_up.y;
                    hdri_light_up.m_z = directx_normalized_hdri_light_up.z;
                }

                device->hdri_light_set_up(hdri_light_up);
            }

            ImGui::EndTable();
        }

        ImGui::End();
    }

    if (ui_controller->m_show_global_illumination_manager)
    {
        {
            ImGuiViewport const *const main_viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + ui_x * ui_index, main_viewport->WorkPos.y + ui_y * ui_index), ImGuiCond_FirstUseEver);
            ++ui_index;

            ImGui::SetNextWindowSize(ImVec2(ui_width, ui_height), ImGuiCond_FirstUseEver);
        }

        ImGui::Begin("Global Illumination Manager", &ui_controller->m_show_global_illumination_manager);

        {
            constexpr char const *const text[LANGUAGE_COUNT] = {
                "Global Illumination Manager",
                "大域照明管理",
                "全域照明管理",
                "全局光照管理"};
            ImGui::TextUnformatted(text[ui_controller->m_language_index]);
        }

        ImGui::Separator();

        if (ImGui::BeginTable("##Global-Illumination-Manager-Table", 2, ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersInnerV))
        {
            ImGui::TableSetupColumn("##Global-Illumination-Manager-Table-Property", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("##Global-Illumination-Manager-Table-Value", ImGuiTableColumnFlags_WidthStretch);

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            {
                constexpr char const *const text[LANGUAGE_COUNT] = {
                    "Quality",
                    "品質",
                    "質量",
                    "质量"};

                ImGui::TextUnformatted(text[ui_controller->m_language_index]);
            }
            ImGui::TableNextColumn();
            {
                constexpr char const *const items[LANGUAGE_COUNT][BRX_ANARI_RENDERER_GI_QUALITY_COUNT] = {
                    {"Disable", "Low", "Medium", "High"},
                    {"無効", "低", "中", "高"},
                    {"停用", "低", "中", "高"},
                    {"停用", "低", "中", "高"}};

                int select_gi_quality = std::min(std::max(0, static_cast<int>(device->renderer_get_gi_quality())), (int(BRX_ANARI_RENDERER_GI_QUALITY_COUNT) - 1));

                ImGui::Combo("##Global-Illumination-Manager-Table-Layout", &select_gi_quality, items[ui_controller->m_language_index], IM_ARRAYSIZE(items[0]));

                device->renderer_set_gi_quality(static_cast<BRX_ANARI_RENDERER_GI_QUALITY>(std::min(std::max(0, select_gi_quality), (int(BRX_ANARI_RENDERER_GI_QUALITY_COUNT) - 1))));
            }

            ImGui::EndTable();
        }

        ImGui::End();
    }

    if (ui_controller->m_show_acknowledgement)
    {
        {
            ImGuiViewport const *const main_viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + ui_x * ui_index, main_viewport->WorkPos.y + ui_y * ui_index), ImGuiCond_FirstUseEver);
            ++ui_index;

            ImGui::SetNextWindowSize(ImVec2(ui_width, ui_height), ImGuiCond_FirstUseEver);
        }

        ImGui::Begin("Acknowledgement", &ui_controller->m_show_acknowledgement);

        {
            constexpr char const *const text[LANGUAGE_COUNT] = {
                "Acknowledgement",
                "謝辞",
                "致謝",
                "致谢"};
            ImGui::TextUnformatted(text[ui_controller->m_language_index]);
        }

        ImGui::Separator();

        if (ImGui::BeginTable("##Acknowledgement-Table", 3, ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersInnerV))
        {
            ImGui::TableSetupColumn("##Acknowledgement-Table-Name", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("##Acknowledgement-Table-Author", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("##Acknowledgement-Table-License", ImGuiTableColumnFlags_WidthStretch);

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted("SHA256");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Bitcoin Dvelopers");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("MIT License");

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted("cgltf");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Johannes Kuhlmann");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("MIT License");

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted("DirectXMesh");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Microsoft");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("MIT License");

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted("libdeflate");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Eric Biggers");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("MIT License");

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted("libjpeg-turbo");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Independent JPEG Group");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Independent JPEG Group License");

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted("libpng");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("PNG Reference Library Authors");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("PNG Reference Library License");

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted("libwebp");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Google");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("BSD License");

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted("OpenEXR");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Industrial Light & Magic");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("BSD License");

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted("zlib");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Jean-loup Gailly & Mark Adler");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("zlib License");

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted("Dear ImGui");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Omar Cornut");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("MIT License");

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted("FreeType");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("FreeType Authors");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("FreeType License");

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted("MediaPipe");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Google");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Apache License");

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted("OpenCL Headers");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Khronos Group");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Apache License");

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted("Bullet Physics");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Erwin Coumans");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("zlib License");

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted("D3D12 Memory Allocator");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("AMD");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("MIT License");

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted("MoltenVK");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Khronos Group");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Apache License");

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted("Vulkan Headers");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Khronos Group");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Apache OR MIT License");

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted("Vulkan Loader");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Khronos Group");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Apache License");

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted("Vulkan Memory Allocator");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("AMD");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("MIT License");

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted("Qt");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Qt Group");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("LGPL License");

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted("CoreRT");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Microsoft");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("MIT License");

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted("DirectXMath");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Microsoft");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("MIT License");

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted("DirectX Shader Compiler");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Microsoft");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("LLVM Release License");

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted("DXUT");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Microsoft");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("MIT License");

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted("libiconv");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("GNU");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("LGPL License");

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted("TBB");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Intel");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Apache License");

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted("Android NDK");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Google");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Apache License");

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted("OpenCV");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Intel");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Apache License");

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted("FFmpeg");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("FFmpeg Team");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("LGPL License");

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted("Shaderc");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Google");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Apache License");

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted("Vulkan Validation Layers");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Khronos Group");

            ImGui::TableNextColumn();
            ImGui::TextUnformatted("Apache License");

            ImGui::EndTable();
        }

        ImGui::End();
    }
}

static inline brx_anari_image *internal_load_asset_image(uint8_t const *asset_image_url_data, uint32_t const asset_image_url_size, char const *asset_model_directory_name, bool force_srgb, brx_anari_device *device, ui_model_t *ui_model)
{
    brx_anari_image *load_anari_image;
    if (NULL != asset_image_url_data)
    {
        if ((asset_image_url_size >= 7U) && ('f' == asset_image_url_data[0]) && ('i' == asset_image_url_data[1]) && ('l' == asset_image_url_data[2]) && ('e' == asset_image_url_data[3]) && (':' == asset_image_url_data[4]) && ('/' == asset_image_url_data[5]) && ('/' == asset_image_url_data[6]))
        {
            mcrt_string asset_image_file_name;
            {
                asset_image_file_name += asset_model_directory_name;
                asset_image_file_name += '/';
                asset_image_file_name += mcrt_string(reinterpret_cast<char const *>(asset_image_url_data + 7U), asset_image_url_size - 7U);
            }

            bool asset_image_file;
            uint64_t asset_image_file_timestamp;
            mcrt_vector<uint8_t> asset_image_file_data;
            {
                asset_image_file = internal_platform_get_file_timestamp_and_data(asset_image_file_name.c_str(), &asset_image_file_timestamp, &asset_image_file_data);
            }

            if (asset_image_file)
            {
                assert(0U != asset_image_file_timestamp);
                assert(!asset_image_file_data.empty());
                load_anari_image = internal_load_asset_image_file(asset_image_file_name.c_str(), asset_image_file_timestamp, asset_image_file_data.data(), asset_image_file_data.size(), force_srgb, device, ui_model);
            }
            else
            {
                // normal or metallic roughness
                if (force_srgb)
                {
                    brx_wsi_tcc();
                }
                load_anari_image = NULL;
            }
        }
        else if ((asset_image_url_size >= 7U) && ('d' == asset_image_url_data[0]) && ('a' == asset_image_url_data[1]) && ('t' == asset_image_url_data[2]) && ('a' == asset_image_url_data[3]) && (':' == asset_image_url_data[4]) && ('/' == asset_image_url_data[5]) && ('/' == asset_image_url_data[6]))
        {
            load_anari_image = internal_load_asset_image_data(asset_image_url_data + 7U, asset_image_url_size - 7U, force_srgb, device, ui_model);
        }
        else
        {
            assert(false);
            load_anari_image = NULL;
        }
    }
    else
    {
        load_anari_image = NULL;
    }

    return load_anari_image;
}

static inline brx_anari_image *internal_load_asset_image_file(char const *asset_image_file_name, uint64_t asset_image_file_timestamp, void const *asset_image_file_data_base, size_t asset_image_file_data_size, bool force_srgb, brx_anari_device *device, ui_model_t *ui_model)
{
    brx_anari_image *load_anari_image;
    {
        mcrt_string asset_image_file_identity;
        {
            asset_image_file_identity += "file ";

            {
                char asset_image_file_timestamp_text[] = {"18446744073709551615"};
                std::snprintf(asset_image_file_timestamp_text, sizeof(asset_image_file_timestamp_text) / sizeof(asset_image_file_timestamp_text[0]), "%020llu", static_cast<long long unsigned>(asset_image_file_timestamp));
                asset_image_file_timestamp_text[(sizeof(asset_image_file_timestamp_text) / sizeof(asset_image_file_timestamp_text[0])) - 1] = '\0';

                asset_image_file_identity += asset_image_file_timestamp_text;
            }

            asset_image_file_identity += ' ';
            asset_image_file_identity += asset_image_file_name;
        }

        auto const found_asset_image = ui_model->m_asset_images.find(asset_image_file_identity);

        if (ui_model->m_asset_images.end() == found_asset_image)
        {
            brx_asset_import_image *asset_import_image = brx_asset_import_create_image_from_memory(asset_image_file_data_base, asset_image_file_data_size);

            if (NULL != asset_import_image)
            {
                if (BRX_ASSET_IMPORT_IMAGE_FORMAT_R8G8B8A8_UNORM == asset_import_image->get_format())
                {
                    brx_anari_image *anari_image = device->new_image(force_srgb ? BRX_ANARI_IMAGE_FORMAT_R8G8B8A8_SRGB : BRX_ANARI_IMAGE_FORMAT_R8G8B8A8_UNORM, asset_import_image->get_pixel_data(), asset_import_image->get_width(), asset_import_image->get_height());

                    if (NULL != anari_image)
                    {
                        ui_model->m_asset_images.insert(found_asset_image, std::pair<mcrt_string, ui_asset_image_model_t>{std::move(asset_image_file_identity), ui_asset_image_model_t{force_srgb, anari_image}});

                        load_anari_image = anari_image;
                    }
                    else
                    {
                        assert(false);
                        load_anari_image = NULL;
                    }
                }
                else if (BRX_ASSET_IMPORT_IMAGE_FORMAT_R16G16B16A16_SFLOAT == asset_import_image->get_format())
                {
                    brx_anari_image *anari_image = device->new_image(BRX_ANARI_IMAGE_FORMAT_R16G16B16A16_SFLOAT, asset_import_image->get_pixel_data(), asset_import_image->get_width(), asset_import_image->get_height());

                    if (NULL != anari_image)
                    {
                        ui_model->m_asset_images.insert(found_asset_image, std::pair<mcrt_string, ui_asset_image_model_t>{std::move(asset_image_file_identity), ui_asset_image_model_t{false, anari_image}});

                        load_anari_image = anari_image;
                    }
                    else
                    {
                        assert(false);
                        load_anari_image = NULL;
                    }
                }
                else
                {
                    assert(false);
                    load_anari_image = NULL;
                }

                brx_asset_import_destroy_image(asset_import_image);
            }
            else
            {
                assert(false);
                load_anari_image = NULL;
            }
        }
        else
        {
            assert(NULL != found_asset_image->second.m_image);
            load_anari_image = found_asset_image->second.m_image;
        }
    }

    return load_anari_image;
}

static inline brx_anari_image *internal_load_asset_image_data(void const *asset_image_file_data_base, size_t asset_image_file_data_size, bool force_srgb, brx_anari_device *device, ui_model_t *ui_model)
{
    brx_anari_image *load_anari_image;
    {
        mcrt_string asset_image_data_identity;
        {
            asset_image_data_identity += "data ";

            {
                char asset_image_file_data_size_text[] = {"18446744073709551615"};
                std::snprintf(asset_image_file_data_size_text, sizeof(asset_image_file_data_size_text) / sizeof(asset_image_file_data_size_text[0]), "%020llu", static_cast<long long unsigned>(asset_image_file_data_size));
                asset_image_file_data_size_text[(sizeof(asset_image_file_data_size_text) / sizeof(asset_image_file_data_size_text[0])) - 1] = '\0';

                asset_image_data_identity += asset_image_file_data_size_text;
                asset_image_data_identity += ' ';
            }

            {
                CSHA256 sha256;
                sha256.Write(static_cast<unsigned char const *>(asset_image_file_data_base), asset_image_file_data_size);

                unsigned char hash[CSHA256::OUTPUT_SIZE];
                sha256.Finalize(hash);

                char hash_text[2U * CSHA256::OUTPUT_SIZE + 1U] = {};
                std::snprintf(hash_text, sizeof(hash_text) / sizeof(hash_text[0]), "%02" PRIX8 "%02" PRIX8 "%02" PRIX8 "%02" PRIX8 "%02" PRIX8 "%02" PRIX8 "%02" PRIX8 "%02" PRIX8 "%02" PRIX8 "%02" PRIX8 "%02" PRIX8 "%02" PRIX8 "%02" PRIX8 "%02" PRIX8 "%02" PRIX8 "%02" PRIX8 "%02" PRIX8 "%02" PRIX8 "%02" PRIX8 "%02" PRIX8 "%02" PRIX8 "%02" PRIX8 "%02" PRIX8 "%02" PRIX8 "%02" PRIX8 "%02" PRIX8 "%02" PRIX8 "%02" PRIX8 "%02" PRIX8 "%02" PRIX8 "%02" PRIX8 "%02" PRIX8, static_cast<uint8_t>(hash[0]), static_cast<uint8_t>(hash[1]), static_cast<uint8_t>(hash[2]), static_cast<uint8_t>(hash[3]), static_cast<uint8_t>(hash[4]), static_cast<uint8_t>(hash[5]), static_cast<uint8_t>(hash[6]), static_cast<uint8_t>(hash[7]), static_cast<uint8_t>(hash[8]), static_cast<uint8_t>(hash[9]), static_cast<uint8_t>(hash[10]), static_cast<uint8_t>(hash[11]), static_cast<uint8_t>(hash[12]), static_cast<uint8_t>(hash[13]), static_cast<uint8_t>(hash[14]), static_cast<uint8_t>(hash[15]), static_cast<uint8_t>(hash[16]), static_cast<uint8_t>(hash[17]), static_cast<uint8_t>(hash[18]), static_cast<uint8_t>(hash[19]), static_cast<uint8_t>(hash[20]), static_cast<uint8_t>(hash[21]), static_cast<uint8_t>(hash[22]), static_cast<uint8_t>(hash[23]), static_cast<uint8_t>(hash[24]), static_cast<uint8_t>(hash[25]), static_cast<uint8_t>(hash[26]), static_cast<uint8_t>(hash[27]), static_cast<uint8_t>(hash[28]), static_cast<uint8_t>(hash[29]), static_cast<uint8_t>(hash[30]), static_cast<uint8_t>(hash[31]));
                hash_text[CSHA256::OUTPUT_SIZE * 2U] = '\0';

                asset_image_data_identity += hash_text;
            }
        }

        auto const found_asset_image = ui_model->m_asset_images.find(asset_image_data_identity);

        if (ui_model->m_asset_images.end() == found_asset_image)
        {
            brx_asset_import_image *asset_import_image = brx_asset_import_create_image_from_memory(asset_image_file_data_base, asset_image_file_data_size);

            if (NULL != asset_import_image)
            {
                if (BRX_ASSET_IMPORT_IMAGE_FORMAT_R8G8B8A8_UNORM == asset_import_image->get_format())
                {
                    brx_anari_image *anari_image = device->new_image(force_srgb ? BRX_ANARI_IMAGE_FORMAT_R8G8B8A8_SRGB : BRX_ANARI_IMAGE_FORMAT_R8G8B8A8_UNORM, asset_import_image->get_pixel_data(), asset_import_image->get_width(), asset_import_image->get_height());

                    if (NULL != anari_image)
                    {
                        ui_model->m_asset_images.insert(found_asset_image, std::pair<mcrt_string, ui_asset_image_model_t>{std::move(asset_image_data_identity), ui_asset_image_model_t{force_srgb, anari_image}});

                        load_anari_image = anari_image;
                    }
                    else
                    {
                        assert(false);
                        load_anari_image = NULL;
                    }
                }
                else if (BRX_ASSET_IMPORT_IMAGE_FORMAT_R16G16B16A16_SFLOAT == asset_import_image->get_format())
                {
                    brx_anari_image *anari_image = device->new_image(BRX_ANARI_IMAGE_FORMAT_R16G16B16A16_SFLOAT, asset_import_image->get_pixel_data(), asset_import_image->get_width(), asset_import_image->get_height());

                    if (NULL != anari_image)
                    {
                        ui_model->m_asset_images.insert(found_asset_image, std::pair<mcrt_string, ui_asset_image_model_t>{std::move(asset_image_data_identity), ui_asset_image_model_t{false, anari_image}});

                        load_anari_image = anari_image;
                    }
                    else
                    {
                        assert(false);
                        load_anari_image = NULL;
                    }
                }
                else
                {
                    assert(false);
                    load_anari_image = NULL;
                }

                brx_asset_import_destroy_image(asset_import_image);
            }
            else
            {
                assert(false);
                load_anari_image = NULL;
            }
        }
        else
        {
            assert(NULL != found_asset_image->second.m_image);
            load_anari_image = found_asset_image->second.m_image;
        }
    }

    return load_anari_image;
}
