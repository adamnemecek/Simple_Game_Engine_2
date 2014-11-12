#include "F_Dual_Quat.h"

#include <math.h>

namespace Math
{
   // member functions

   F_Dual_Quat::F_Dual_Quat()
      : m_real(),
      m_dual()
   {
   }

   F_Dual_Quat::F_Dual_Quat(const F_Quat &real, const F_Quat &dual)
      : m_real(real),   // make use of the default assignment constructors
      m_dual(dual)
   {
   }

   F_Dual_Quat::F_Dual_Quat(const glm::vec3 &rotation_axis, const float rotation_angle_rad, const glm::vec3 &translate)
   {
      F_Quat new_orientation = F_Quat::generate_rotator(rotation_axis, rotation_angle_rad);
      F_Quat new_translation = F_Quat::generate_pure_quat(translate);

      m_real = new_orientation;
      m_dual = 0.5f * new_translation * new_orientation;
   }

   F_Dual_Quat F_Dual_Quat::generate_translate_only(glm::vec3 &translate)
   {
      F_Quat new_orientation(1.0f, glm::vec3());
      F_Quat new_translation = 0.5f * F_Quat::generate_pure_quat(translate);

      return F_Dual_Quat(new_orientation, new_translation);
   }

   F_Dual_Quat F_Dual_Quat::generate_orientation_only(glm::vec3 &rotation_axis, float rotation_angle)
   {
      return F_Dual_Quat();
   }

   void F_Dual_Quat::operator = (const F_Dual_Quat &right)
   {
      this->m_real = right.m_real;
      this->m_dual = right.m_dual;
   }

   void F_Dual_Quat::operator *= (const F_Dual_Quat &right)
   {
      F_Quat new_real = this->m_real * right.m_real;

      F_Quat new_dual = this->m_real * right.m_dual;
      new_dual += this->m_dual * right.m_real;

      this->m_real = new_real;
      this->m_dual = new_dual;
   }

   float F_Dual_Quat::magnitude(const F_Dual_Quat &dq)
   {
      return 0.0f;
   }

   F_Dual_Quat F_Dual_Quat::normalize()
   {
      return F_Dual_Quat();
   }

   F_Dual_Quat F_Dual_Quat::conjugate()
   {
      return F_Dual_Quat(this->m_real.conjugate(), this->m_dual.conjugate());
   }


   // non-member functions
   F_Dual_Quat operator*(const F_Dual_Quat &left, const F_Dual_Quat &right)
   {
      F_Quat new_real = left.m_real * right.m_real;
      
      F_Quat new_dual = left.m_real * right.m_dual;
      new_dual += left.m_dual * right.m_real;
      
      return F_Dual_Quat(new_real, new_dual);
   }
}

