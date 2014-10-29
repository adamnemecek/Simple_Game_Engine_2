#ifndef ENGINE_DUAL_QUAT_H
#define ENGINE_DUAL_QUAT_H

#include <Utilities\Include_Helper_GLM_Quaternion.h>
#include <glm\vec3.hpp>

namespace Math
{
   class Dual_Quaternion
   {
   public:
      Dual_Quaternion();
      Dual_Quaternion(const glm::fquat &real, const glm::fquat &dual);

      glm::fquat m_real;
      glm::fquat m_dual;

      static Dual_Quaternion point(const glm::vec3 &coordinates);
      static float magnitude(const Dual_Quaternion &dq);
      static Dual_Quaternion conjugate(const Dual_Quaternion &dq);
      static Dual_Quaternion normalize(const Dual_Quaternion &dq);
      static Dual_Quaternion translate(glm::vec3 &translate_vector);
      static Dual_Quaternion rotate(glm::vec3 &rotation_axis, float rotation_angle);
   };

   Dual_Quaternion &operator*(const Dual_Quaternion &left, const Dual_Quaternion &right);
}

#endif
