#ifndef ENGINE_QUATERNION_HELPER_H
#define ENGINE_QUATERNION_HELPER_H

#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>
#include <Utilities\Include_Helpers\GLM_Quaternion.h>
#include <Utilities\Include_Helpers\GLM_Dual_Quaternion.h>

namespace Utilities
{
   namespace Quaternion_Helper
   {
      // takes a quaternion and rotates it (with right-multiplying) by the requested number of radians around the requested axis
      // Note: This function is an adaption of OrientationOffset(...) from ArcSynthesis Tutorial 0.3.8, tutorial 8, QuaternionYPR.cpp.
      void orientation_offset(const glm::vec3 &rotation_vector, const float rotation_angle_rad, glm::fquat &offset_this);

      // this will be useful to force the camera to point at something rather than having the user flail about in relativistic angles
      // Note: IN PROGRESS  (??how to do this??)
      void point_at(const glm::vec3 &look_here, glm::fquat &offset_this);

      // creates a purely translational dual quat out of a vector
      glm::fdualquat dual_quat_translation_only(const glm::vec3 &translation);

      // creates a dual quat from a point
      // Note: This function is computationally identical to making a translation-only dual 
      // quat, but I am making this function with a different name to clarify intent.
      glm::fdualquat dual_quat_from_point(const glm::vec3 &position);

      // transforms the given point with the provided dual quat transform, and generates its own conjugate for ease of use
      glm::vec3 dual_quat_translate_point(const glm::vec3 &point, const glm::fdualquat &transform);

      // transforms the given point with the provided dual quat transform, and uses the provided transform conjugate for efficiency
      glm::vec3 dual_quat_translate_point(const glm::vec3 &point, const glm::fdualquat &transform, const glm::fdualquat transform_conjugate);

      // creates a purely rotational dual quat out an orientation quat
      glm::fdualquat dual_quat_rotation_only(const glm::fquat &orientation);

      // creates a dual quat out of an orientation quat and a position in 3D space
      glm::fdualquat dual_quat(const glm::fquat &orientation, const glm::vec3 &position);

      // calculates the conjugate (that is, the inverse) of a dual quaternion because
      // glm doesn't have an overload of glm::conjugate(...) for a dual quat
      glm::fdualquat dual_quat_conjugate(const glm::fdualquat &dq);

      // creates a 4x4 matrix with rotation and translation out of a dual quat
      // Note: ??reference??
      glm::mat4 dual_quat_to_mat4(const glm::fdualquat &dq);
   }
}

#endif