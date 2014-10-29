#include <Utilities\Quaternion_Helper.h>

#include <Utilities\Include_Helper_Default_Vectors.h>
#include "Include_Helper_GLM_Quaternion.h"
#include <glm\mat3x3.hpp>

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
   }
}