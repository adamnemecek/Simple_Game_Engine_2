#ifndef ENGINE_DUAL_QUAT_H
#define ENGINE_DUAL_QUAT_H

#include <Math\F_Quat.h>
#include <glm\vec3.hpp>

namespace Math
{
   class F_Dual_Quat
   {
   public:
      F_Quat m_real;
      F_Quat m_dual;

      // default constructor
      // Note: Constructs a dual quat composed of two default quats, resulting
      // in a dual quat of the following form:
      // real = [0, <0,0,0>], dual = [0, <0,0,0>]
      F_Dual_Quat();

      // explicit constructor
      // Note: Constructs a dual quat of the following form:
      // real = argument real, dual = argument dual
      F_Dual_Quat(const F_Quat &real, const F_Quat &dual);

      // convenience constructor
      // Note: Constructs a dual quat to perform the requested rotation followed by the requested translation.
      F_Dual_Quat(const glm::vec3 &rotation_axis, const float rotation_angle_rad, const glm::vec3 &translate);

      // generates a dual quat that performs a translation only
      // Note: Results in a dual quat of the following form
      // real = [1, <0,0,0>], dual = [0, (1/2)translate_vector]
      static F_Dual_Quat generate_translate_only(glm::vec3 &translate);

      // generates a dual quat that performs a rotation only 
      // Note: Results in a dual quat of the following form:
      // real = [1, orientation vectors], dual = [0, <0,0,0>]
      static F_Dual_Quat generate_orientation_only(glm::vec3 &rotation_axis, float rotation_angle);

      // explicit self-assignment operator
      void operator=(const F_Dual_Quat &right);

      // self-multiplication
      // Note: See description of operator* for details.
      void operator*=(const F_Dual_Quat &right);

      // self-magnitude
      // Note: Results in a float by the following calculation: 
      // dual quat DQ = this * this->conjugate
      // sqrt(DQ.rea) == sqrt(
      float magnitude(const F_Dual_Quat &dq);

      // self-normalize
      F_Dual_Quat normalize();

      // self-conjugate
      F_Dual_Quat conjugate();      
   };

   // multiplication
   // Note: Given dual quats dq1 = (A + eB) and dq2 = (C + eD), then
   // dq1 * dq2 = AC + e(AD + BC).  There is no BD because e^2 is defined to
   // be 0.
   F_Dual_Quat operator*(const F_Dual_Quat &left, const F_Dual_Quat &right);
}

#endif
