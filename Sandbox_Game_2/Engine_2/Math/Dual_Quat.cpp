#include "dual_quat.h"

#include <math.h>

namespace Math
{
   Dual_Quaternion::Dual_Quaternion()
      : m_real(1.0f, 0.0f, 0.0f, 0.0f),
      m_dual(0.0f, 0.0f, 0.0f, 0.0f)
   {
   }

   Dual_Quaternion::Dual_Quaternion(const glm::fquat &real, const glm::fquat &dual)
      : m_real(real),
      m_dual(dual)
   {
   }

   Dual_Quaternion Dual_Quaternion::point(const glm::vec3 &coordinates)
   {
      return Dual_Quaternion(
         glm::fquat(),
         glm::fquat(0.0f, coordinates.x, coordinates.y, coordinates.z));
   }

   float Dual_Quaternion::magnitude(const Dual_Quaternion &dq)
   {
      return 5.0f;
   }

   Dual_Quaternion Dual_Quaternion::conjugate(const Dual_Quaternion &dq)
   {
      return Dual_Quaternion(dq.m_real, glm::conjugate(dq.m_dual));
   }

   Dual_Quaternion Dual_Quaternion::normalize(const Dual_Quaternion &dq)
   {
      return Dual_Quaternion();
   }

   Dual_Quaternion Dual_Quaternion::translate(glm::vec3 &translate_vector)
   {
      return Dual_Quaternion(
         glm::fquat(),
         glm::fquat(0.0f, 0.5f * translate_vector.x, 0.5f *translate_vector.y, 0.5f * translate_vector.z));
   }

   Dual_Quaternion Dual_Quaternion::rotate(glm::vec3 &rotation_axis, float rotation_angle)
   {
      return Dual_Quaternion();
   }

   Dual_Quaternion &operator*(const Dual_Quaternion &left, const Dual_Quaternion &right)
   {
      return Dual_Quaternion(left.m_real * right.m_real, (left.m_real * right.m_dual + left.m_dual * right.m_real));
   }
}

