// Filename: physxEnums.h
// Created by:  enn0x (23Sep09)
//
////////////////////////////////////////////////////////////////////
//
// PANDA 3D SOFTWARE
// Copyright (c) Carnegie Mellon University.  All rights reserved.
//
// All use of this software is subject to the terms of the revised BSD
// license.  You should have received a copy of this license along
// with this source code in a file named "LICENSE."
//
////////////////////////////////////////////////////////////////////

#ifndef PHYSXENUMS_H
#define PHYSXENUMS_H

#include "pandabase.h"

#include "NoMinMax.h"
#include "NxPhysics.h"

#ifdef CPPPARSER

// PhysxBodyFlag
#define NX_BF_DISABLE_GRAVITY 1<<0
#define NX_BF_FROZEN_POS_X 1<<1
#define NX_BF_FROZEN_POS_Y 1<<2
#define NX_BF_FROZEN_POS_Z 1<<3
#define NX_BF_FROZEN_ROT_X 1<<4
#define NX_BF_FROZEN_ROT_Y 1<<5
#define NX_BF_FROZEN_ROT_Z 1<<6
#define NX_BF_FROZEN_POS 256
#define NX_BF_FROZEN_ROT 131072
#define NX_BF_FROZEN 131328
#define NX_BF_KINEMATIC 1<<7
#define NX_BF_VISUALIZATION 1<<8
#define NX_BF_FILTER_SLEEP_VEL 1<<10
#define NX_BF_ENERGY_SLEEP_TEST 1<<11

// PhysxCombineMode
#define NX_CM_AVERAGE 0
#define NX_CM_MIN 1
#define NX_CM_MULTIPLY 2
#define NX_CM_MAX 3

// PhysxD6JointDriveType
#define NX_D6JOINT_DRIVE_POSITION 1<<0
#define NX_D6JOINT_DRIVE_VELOCITY 1<<1

// PhysxD6JointFlag
#define NX_D6JOINT_SLERP_DRIVE 1<<0
#define NX_D6JOINT_GEAR_ENABLED 1<<1

// PhysxD6JointMotion
#define NX_D6JOINT_MOTION_LOCKED 0
#define NX_D6JOINT_MOTION_LIMITED 1
#define NX_D6JOINT_MOTION_FREE 2

// PhysxDistanceJointFlag
#define NX_DJF_MAX_DISTANCE_ENABLED 1<<0
#define NX_DJF_MIN_DISTANCE_ENABLED 1<<1
#define NX_DJF_SPRING_ENABLED 1<<2

// PhysxMaterialFlag
#define NX_MF_ANISOTROPIC 1<<0
#define NX_MF_DISABLE_FRICTION 1<<4
#define NX_MF_DISABLE_STRONG_FRICTION 1<<5

// PhysxForceMode
#define NX_FORCE 0
#define NX_IMPULSE 1
#define NX_VELOCITY_CHANGE 2
#define NX_SMOOTH_IMPULSE 3
#define NX_SMOOTH_VELOCITY_CHANGE 4
#define NX_ACCELERATION 5

// PhysxJointFlag
#define NX_JF_COLLISION_ENABLED 1<<0
#define NX_JF_VISUALIZATION 1<<1

// PhysxProjectionMode
#define NX_JPM_NONE 0
#define NX_JPM_POINT_MINDIST 1
#define NX_JPM_LINEAR_MINDIST 2

// PhysxPulleyJointFlag
#define NX_PJF_IS_RIGID 1<<0
#define NX_PJF_MOTOR_ENABLED 1<<1

// PhysxRevoluteJointFlag
#define NX_RJF_LIMIT_ENABLED 1<<0
#define NX_RJF_MOTOR_ENABLED 1<<1
#define NX_RJF_SPRING_ENABLED 1<<2

// PhysxShapeFlag
#define NX_TRIGGER_ON_ENTER 1<<0
#define NX_TRIGGER_ON_LEAVE 1<<1
#define NX_TRIGGER_ON_STAY 1<<2
#define NX_TRIGGER_ENABLE 7
#define NX_SF_VISUALIZATION 1<<3
#define NX_SF_DISABLE_COLLISION 1<<4
#define NX_SF_FEATURE_INDICES 1<<5
#define NX_SF_DISABLE_RAYCASTING 1<<6
#define NX_SF_POINT_CONTACT_FORCE 1<<7
#define NX_SF_FLUID_DRAIN 1<<8
#define NX_SF_FLUID_DISABLE_COLLISION 1<<10
#define NX_SF_FLUID_TWOWAY 1<<11
#define NX_SF_DISABLE_RESPONSE 1<<12
#define NX_SF_DYNAMIC_DYNAMIC_CCD 1<<13
#define NX_SF_DISABLE_SCENE_QUERIES 1<<14
#define NX_SF_CLOTH_DRAIN 1<<15
#define NX_SF_CLOTH_DISABLE_COLLISION 1<<16
#define NX_SF_CLOTH_TWOWAY 1<<17
#define NX_SF_SOFTBODY_DRAIN 1<<18
#define NX_SF_SOFTBODY_DISABLE_COLLISION 1<<19
#define NX_SF_SOFTBODY_TWOWAY 1<<20

// PhysxSphericalJointFlag
#define NX_SJF_TWIST_LIMIT_ENABLED 1<<0
#define NX_SJF_SWING_LIMIT_ENABLED 1<<1
#define NX_SJF_TWIST_SPRING_ENABLED 1<<2
#define NX_SJF_SWING_SPRING_ENABLED 1<<3
#define NX_SJF_JOINT_SPRING_ENABLED 1<<4
#define NX_SJF_PERPENDICULAR_DIR_CONSTRAINTS 1<<5

// PhysxUpAxis
#define NX_X 1
#define NX_Y 2
#define NX_Z 3

#endif // CPPPARSER

////////////////////////////////////////////////////////////////////
//       Class : BamEnums
// Description : This class exists just to provide scoping for the
//               enums shared by PhysX classes.
////////////////////////////////////////////////////////////////////
class EXPCL_PANDAPHYSX PhysxEnums {
PUBLISHED:

  enum PhysxBodyFlag {
    BF_disable_gravity   = NX_BF_DISABLE_GRAVITY,
    Bf_frozen_pos_x      = NX_BF_FROZEN_POS_X,
    BF_frozen_pos_y      = NX_BF_FROZEN_POS_Y,
    BF_frozen_pos_z      = NX_BF_FROZEN_POS_Z,
    BF_frozen_rot_x      = NX_BF_FROZEN_ROT_X,
    BF_frozen_rot_y      = NX_BF_FROZEN_ROT_Y,
    BF_frozen_rot_z      = NX_BF_FROZEN_ROT_Z,
    BF_frozen_pos        = NX_BF_FROZEN_POS,
    BF_frozen_rot        = NX_BF_FROZEN_ROT,
    BF_frozen            = NX_BF_FROZEN,
    BF_kinematic         = NX_BF_KINEMATIC,
    BF_visualization     = NX_BF_VISUALIZATION,
    BF_filter_sleep_vel  = NX_BF_FILTER_SLEEP_VEL,
    BF_energy_sleep_test = NX_BF_ENERGY_SLEEP_TEST
  };

  enum PhysxCombineMode {
    CM_average  = NX_CM_AVERAGE,
    CM_min      = NX_CM_MIN,
    CM_multiply = NX_CM_MULTIPLY,
    CM_max      = NX_CM_MAX
  };

  enum PhysxD6JointDriveType {
    D6_joint_drive_position = NX_D6JOINT_DRIVE_POSITION,
    D6_joint_drive_velocity = NX_D6JOINT_DRIVE_VELOCITY
  };

  enum PhysxD6JointFlag {
    D6_joint_slerp_drive   = NX_D6JOINT_SLERP_DRIVE,
    D6_joint_gear_disabled = NX_D6JOINT_GEAR_ENABLED
  };

  enum PhysxD6JointMotion {
    D6_joint_motion_locked  = NX_D6JOINT_MOTION_LOCKED,
    D6_joint_motion_limited = NX_D6JOINT_MOTION_LIMITED,
    D6_joint_motion_free    = NX_D6JOINT_MOTION_FREE
  };

  enum PhysxDistanceJointFlag {
    DJF_max_distance_enabled  = NX_DJF_MAX_DISTANCE_ENABLED,
    DJF_mix_distance_enabled  = NX_DJF_MIN_DISTANCE_ENABLED,
    DJF_spring_enabled        = NX_DJF_SPRING_ENABLED
  };

  enum PhysxForceMode {
    FM_force                  = NX_FORCE,
    FM_impulse                = NX_IMPULSE,
    FM_velocity_change        = NX_VELOCITY_CHANGE,
    FM_smooth_impulse         = NX_SMOOTH_IMPULSE,
    FM_smooth_velocity_change = NX_SMOOTH_VELOCITY_CHANGE,
    FM_acceleration           = NX_ACCELERATION
  };

  enum PhysxJointFlag {
    JF_collision_enabled = NX_JF_COLLISION_ENABLED,
    JF_visualization     = NX_JF_VISUALIZATION
  };

  enum PhysxMaterialFlag {
    MF_anisotropic             = NX_MF_ANISOTROPIC,
    MF_disable_friction        = NX_MF_DISABLE_FRICTION,
    MF_disable_strong_friction = NX_MF_DISABLE_STRONG_FRICTION
  };

  enum PhysxProjectionMode {
    PM_none             = NX_JPM_NONE,
    PM_point_mindist    = NX_JPM_POINT_MINDIST,
    PM_linear_mindist   = NX_JPM_LINEAR_MINDIST
  };

  enum PhysxPulleyJointFlag {
    PJF_is_rigid       = NX_PJF_IS_RIGID,
    PJF_motor_enabled  = NX_PJF_MOTOR_ENABLED
  };

  enum PhysxRevoluteJointFlag {
    RJF_limit_enabled    = NX_RJF_LIMIT_ENABLED,
    RJF_motor_enabled    = NX_RJF_MOTOR_ENABLED,
    RJF_spring_enabled   = NX_RJF_SPRING_ENABLED
  };

  enum PhysxShapeFlag {
    SF_trigger_on_enter           = NX_TRIGGER_ON_ENTER,
    SF_trigger_on_leave           = NX_TRIGGER_ON_LEAVE,
    SF_trigger_on_stay            = NX_TRIGGER_ON_STAY,
    SF_trigger_enable             = NX_TRIGGER_ENABLE,
    SF_visualization              = NX_SF_VISUALIZATION,
    SF_disable_collision          = NX_SF_DISABLE_COLLISION,
    SF_disable_raycasting         = NX_SF_DISABLE_RAYCASTING,
    SF_disable_response           = NX_SF_DISABLE_RESPONSE,
    SF_disable_scene_queries      = NX_SF_DISABLE_SCENE_QUERIES,
    SF_point_contact_force        = NX_SF_POINT_CONTACT_FORCE,
    SF_feature_indices            = NX_SF_FEATURE_INDICES,
    SF_dynamic_dynamic_ccd        = NX_SF_DYNAMIC_DYNAMIC_CCD,
    SF_fluid_drain                = NX_SF_FLUID_DRAIN,
    SF_fluid_disable_collision    = NX_SF_FLUID_DISABLE_COLLISION,
    SF_fluid_twoway               = NX_SF_FLUID_TWOWAY,
    SF_cloth_drain                = NX_SF_CLOTH_DRAIN,
    SF_cloth_disable_collision    = NX_SF_CLOTH_DISABLE_COLLISION,
    SF_cloth_twoway               = NX_SF_CLOTH_TWOWAY,
    SF_softbody_drain             = NX_SF_SOFTBODY_DRAIN,
    SF_softbody_disable_collision = NX_SF_SOFTBODY_DISABLE_COLLISION,
    SF_softbody_twoway            = NX_SF_SOFTBODY_TWOWAY
  };

  enum PhysxSphericalJointFlag {
    SJF_twist_limit_enabled           = NX_SJF_TWIST_LIMIT_ENABLED,
    SJF_swing_limit_enabled           = NX_SJF_SWING_LIMIT_ENABLED,
    SJF_twist_spring_enabled          = NX_SJF_TWIST_SPRING_ENABLED,
    SJF_swing_spring_enabled          = NX_SJF_SWING_SPRING_ENABLED,
    SJF_joint_spring_enabled          = NX_SJF_JOINT_SPRING_ENABLED,
    SJF_perpendicular_dir_constraints = NX_SJF_PERPENDICULAR_DIR_CONSTRAINTS
  };

  enum PhysxUpAxis {
    X_up                          = NX_X,
    Y_up                          = NX_Y,
    Z_up                          = NX_Z
  };

};

EXPCL_PANDAPHYSX ostream &operator << (ostream &out, PhysxEnums::PhysxUpAxis axis);
EXPCL_PANDAPHYSX istream &operator >> (istream &in, PhysxEnums::PhysxUpAxis &axis);

#endif

