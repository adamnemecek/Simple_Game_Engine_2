#include "F_Dual_Number.h"

namespace Math
{
   F_Dual_Number::F_Dual_Number()
      : m_real(0.0f),
      m_dual(0.0f)
   {
   }

   F_Dual_Number::F_Dual_Number(const float new_real, const float new_dual)
      : m_real(new_real),
      m_dual(new_dual)
   {
   }
}