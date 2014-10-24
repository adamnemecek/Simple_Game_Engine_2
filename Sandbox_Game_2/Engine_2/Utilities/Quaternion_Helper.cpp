#include <Utilities\Quaternion_Helper.h>

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
   }
}