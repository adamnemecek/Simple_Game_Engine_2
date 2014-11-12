#include "F_Dual_Quat.h"

#include <math.h>

namespace Math
{
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
      m_dual = new_translation;
   }


}

