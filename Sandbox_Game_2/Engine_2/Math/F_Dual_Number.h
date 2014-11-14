#ifndef ENGINE_F_DUAL_NUMBER_H
#define ENGINE_F_DUAL_NUMBER_H


namespace Math
{
   // this is a helper class that will optimize the normalization of the dual quat
   // Note: See the comment on F_Dual_Quat.normalize() for details.
   // Note: A dual quat is a quat in which each term has been replaced by a dual number.
   // The new set of terms can be re-organized to be equivalent to dual number notation 
   // in which the real and dual components are quats instead of real numbers.
   // Just as a quat is made of real numbers and its magnitude is also a real number, so
   // a dual quat is made of dual numbers and its magnitude is a dual number.  
   // To normalize a dual quat, we must divide it by its magnitude, and we can use the rules
   // for dual number division to normalize it.

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
