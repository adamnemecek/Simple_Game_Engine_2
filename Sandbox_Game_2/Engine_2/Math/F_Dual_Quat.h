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

      F_Dual_Quat();
      F_Dual_Quat(const F_Quat &real, const F_Quat &dual);

      /*
      TEST(Float_Dual_Quat, Default_Constructor)
      TEST(Float_Dual_Quat, Explicit_Constructor)
      TEST(Float_Dual_Quat, Translate)
      TEST(Float_Dual_Quat, Orientation)
      TEST(Float_Dual_Quat, Magnitude)
      TEST(Float_Dual_Quat, Normalize)
      TEST(Float_Dual_Quat, Conjugate)
      TEST(Float_Dual_Quat, Multiply)
      */

      // generates a dual quat that performs a translation only
      // Note: Results in a dual quat of the following form
      // real = [1, <0,0,0>], dual = [0, (1/2)translate_vector]
      static F_Dual_Quat translate(glm::vec3 &translate_vector);

      // generates a dual quat that performs a rotation only 
      // Note: Results in a dual quat of the following form:
      // real = [1, orientation vectors], dual = [0, <0,0,0>]
      static F_Dual_Quat orientation(glm::vec3 &rotation_axis, float rotation_angle);

      // self-magnitude
      // Note: Results in a float by the following calculation: 
      // dual quat DQ = this * this->conjugate
      // sqrt(DQ.rea) == sqrt(
      float magnitude(const F_Dual_Quat &dq);

      // self-multiplication
      // Note: See description of operator* for details.
      F_Dual_Quat operator*=(const F_Dual_Quat &right);

      static F_Dual_Quat conjugate(const F_Dual_Quat &dq);
      static F_Dual_Quat normalize(const F_Dual_Quat &dq);
   };

   // multiplication
   // Note: Given dual quats dq1 = (A + eB) and dq2 = (C + eD), then
   // dq1 * dq2 = AC + e(AD + BC).  There is no BD because e^2 is defined to
   // be 0.
   F_Dual_Quat &operator*(const F_Dual_Quat &left, const F_Dual_Quat &right);
}

#endif
