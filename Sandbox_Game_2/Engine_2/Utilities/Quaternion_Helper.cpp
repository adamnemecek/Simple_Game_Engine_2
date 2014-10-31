#include <Utilities\Quaternion_Helper.h>

#include <Utilities\Include_Helpers\Default_Vectors.h>

namespace Utilities
{
   namespace Quaternion_Helper
   {
      void orientation_offset(const glm::vec3 &rotation_axis, const float rotation_angle_rad, glm::fquat &offset_this)
      {
         glm::vec3 normalized_rotation_axis = glm::normalize(rotation_axis);

         normalized_rotation_axis *= sinf(rotation_angle_rad / 2.0f);
         float scalar = cosf(rotation_angle_rad / 2.0f);

         glm::fquat local_offset(
            scalar,
            normalized_rotation_axis.x,
            normalized_rotation_axis.y,
            normalized_rotation_axis.z);

         // assuming right multiplication only for now
         offset_this = glm::normalize(offset_this * local_offset);
      }

      void point_at(const glm::vec3 &look_here, glm::fquat &offset_this)
      {
         //glm::vec3 current_view_direction = glm::mat3(glm::mat4_cast(offset_this)) * Utilities::Default_Vectors::WORLD_FORWARD_VECTOR;
      }

      glm::fdualquat make_dual_quat_position_only(const glm::vec3 &position)
      {
         return glm::fdualquat(glm::fquat(), position);
      }

      glm::fdualquat make_dual_quat_rotation_only(const glm::fquat &orientation)
      {
         return glm::fdualquat(orientation, glm::fquat(0.0f, 0.0f, 0.0f, 0.0f));
      }

      glm::fdualquat make_dual_quat(const glm::fquat &orientation, const glm::vec3 &position)
      {
         return glm::fdualquat(orientation, position);
      }

      glm::fdualquat dual_quat_conjugate(glm::fdualquat dq)
      {
         return glm::fdualquat(glm::conjugate(dq.real), (-1.0f) * glm::conjugate(dq.dual));
      }

   }
}