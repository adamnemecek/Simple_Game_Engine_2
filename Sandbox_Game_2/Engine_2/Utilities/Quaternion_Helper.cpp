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

      glm::fdualquat make_dual_quat_translation_only(const glm::vec3 &translation)
      {
         return glm::fdualquat(glm::fquat(), translation);
      }

      glm::fdualquat make_dual_quat_from_point(const glm::vec3 &position)
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

      glm::fdualquat dual_quat_conjugate(const glm::fdualquat &dq)
      {
         return glm::fdualquat(glm::conjugate(dq.real), (-1.0f) * glm::conjugate(dq.dual));
      }

      glm::mat4 dual_quat_to_mat4(const glm::fdualquat &dq)
      {
         glm::fdualquat norm_dq = glm::normalize(dq);

         // get an identity matrix
         glm::mat4 mat;

         // extract rotational information
         float scalar = norm_dq.real.w;
         float x = norm_dq.real.x;
         float y = norm_dq.real.y;
         float z = norm_dq.real.z;

         mat[0][0] = (scalar * scalar) + (x * x) - (y * y) - (z * z);
         mat[0][1] = (2 * x * y) + (2 * scalar * z);
         mat[0][2] = (2 * x * z) - (2 * scalar * y);

         mat[1][0] = (2 * x * y) - (2 * scalar * z);
         mat[1][1] = (scalar * scalar) + (y * y) - (x * x) - (z * z);
         mat[1][2] = (2 * y * z) + (2 * scalar * x);

         mat[2][0] = (2 * x * z) + (2 * scalar * y);
         mat[2][1] = (2 * y * z) - (2 * scalar * x);
         mat[2][2] = (scalar * scalar) + (z * z) - (x * x) - (y * y);

         // extract translational information
         glm::fquat trans = (norm_dq.dual * 2.0f) * glm::conjugate(norm_dq.real);
         mat[3][0] = trans.x;
         mat[3][1] = trans.y;
         mat[3][2] = trans.z;


         // the last row remains untouched; that row is the realm of clip space and perspective division 


         return mat;
      }
   }
}