#ifndef ENGINE_F_DUAL_NUMBER_H
#define ENGINE_F_DUAL_NUMBER_H


namespace Math
{
   // this is a helper class that will optimize the normalization of the dual quat
   // Note: See the comment on F_Dual_Quat.normalize() for details.
   struct __declspec(dllexport) F_Dual_Number
   {
      float m_real;
      float m_dual;

      // default constructor
      // Note: Results in a dual number with all 0s.
      F_Dual_Number();

      // explicit constructor
      // Note: Results in a dual number with the provided values
      F_Dual_Number(const float new_real, const float new_dual);
   };
}

#endif
