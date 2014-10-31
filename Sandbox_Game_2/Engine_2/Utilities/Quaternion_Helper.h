#ifndef ENGINE_QUATERNION_HELPER_H
#define ENGINE_QUATERNION_HELPER_H

#include <glm\vec3.hpp>
#include <Utilities\Include_Helpers\GLM_Quaternion.h>
#include <Utilities\Include_Helpers\GLM_Dual_Quaternion.h>

namespace Utilities
{
   namespace Quaternion_Helper
   {
      // takes a quaternion and rotates it (with right-multiplying) by the requested number of radians around the requested axis
      // Note: This function is an adaption of OrientationOffset(...) from ArcSynthesis Tutorial 0.3.8, tutorial 8, QuaternionYPR.cpp.
      void orientation_offset(const glm::vec3 &rotation_axis, const float rotation_angle_rad, glm::fquat &offset_this);

      // this will be useful to force the camera to point at something rather than having the user flail about in relativistic angles
      // Note: IN PROGRESS  (??how to do this??)
      void point_at(const glm::vec3 &look_here, glm::fquat &offset_this);

      // creates a purely translational dual-quaternion out of a vector
      glm::fdualquat make_dual_quat_position_only(const glm::vec3 &position);
      glm::fdualquat make_dual_quat_rotation_only(const glm::fquat &orientation);
      glm::fdualquat make_dual_quat(const glm::fquat &orientation, const glm::vec3 &position);
      glm::fdualquat dual_quat_conjugate(const glm::fdualquat &dq);
   }
}

#endif