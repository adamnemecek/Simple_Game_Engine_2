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
      F_Quat new_rotator = F_Quat::generate_rotator_for_dual_quat(rotation_axis, rotation_angle_rad);
      F_Quat new_translator = F_Quat::generate_pure_quat(0.5f * translate);

      // these are the rules for making the dual part, and they work, so just say "yes!" and we'll move on
      m_real = new_rotator;
      m_dual = new_translator * new_rotator;
   }

   F_Dual_Quat F_Dual_Quat::generate_translate_only(glm::vec3 &translate)
   {
      // in this case, the rotator is the quat equivalent of 1, so don't bother multiplying
      // the new_translation component by it (but still halve the vector)
      F_Quat new_rotator(1.0f, glm::vec3());
      F_Quat new_translator = F_Quat::generate_pure_quat(0.5f * translate);

      return F_Dual_Quat(new_rotator, new_translator);
   }

   F_Dual_Quat F_Dual_Quat::generate_rotator_only(glm::vec3 &rotation_axis, float rotation_angle_rad)
   {
      // in this case, the dual part is all 0s, so don't bother with multiplying by 1/2 and the rotator
      F_Quat new_rotator = F_Quat::generate_rotator_for_dual_quat(rotation_axis, rotation_angle_rad);
      F_Quat new_translator;

      return F_Dual_Quat(new_rotator, new_translator);
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

   F_Dual_Number F_Dual_Quat::magnitude()
   {
      // here's how this will go down:
      // Note: Legend:
      // - q = real quaternion of the form 'scalar + xi + yj + zk)
      // - q' = real quaternion in the dual part
      // - q* = conjugate of the real quaternion
      // - q'* = conjugate of the real quaternion in the dual part

      // Note: Let there be a dual quat dq = q + (q')e, where 'e' is the dual operator. 
      // The square of its magnitude is a dual quat 'result' and is given by 
      // result = dq * dq.conjugate() 
      // (I don't know why, but these are the rules and they work, so I'll go with them).  
      // This multiplication results in a dual quat of the following form:
      // result = (dq)(dq*) = qq* + (qq'* + q'q*)e
      // 
      // Solve for the parts; the negative signs of the conjugates work out; trust me :)
      // - qq* = (q.scalar)^2 + dot(q.vector,q.vector) + 0i + 0j + 0k
      // - (qq'* + q'q*) = 2(q.scalar)(q'.scalar) + 2dot(q'.vector,q.vector) + 0i + 0j + 0k
      //
      // => result = ((q.scalar)^2 + dot(q.vector,q.vector)) + (2(q.scalar)(q'.scalar) + 2dot(q'.vector,q.vector))e
      // So this dual quat has zero-vectors in its real and dual quats and is therefore 
      // a dual number.
      F_Dual_Quat dq = *this;
      F_Dual_Quat dq_conjugate = dq.conjugate();
      F_Dual_Quat magnitude_squared = (*this) * this->conjugate();

      // I can't take the square root of a dual number directly, but I can solve for it
      // Ex: Solve for sqrt(30 + 140e)
      // Let a + (b)e = sqrt(30 + 140e), where 'a' and 'b' are real numbers and 'e' is the dual operator.
      // Then (a + (b)e)^2 = 30 + 140e
      // a^2 + (2ab)e = 30 + 140e
      // 
      // Solve for the parts:
      // - a = sqrt(30)
      // - b = 140 / (2a) = 140 / (2sqrt(30))
      //
      // General case:
      // sqrt(a + (b)e), where 'a' and 'b' are real numbers and 'e' is the dual operator, 
      //  = sqrt(a) + (b / (2a))e
      float real_of_root = sqrtf(magnitude_squared.m_real.m_scalar);
      float dual_of_root = magnitude_squared.m_dual.m_scalar / (2.0f * real_of_root);

      return F_Dual_Number(real_of_root, dual_of_root);
   }

   F_Dual_Quat F_Dual_Quat::conjugate()
   {
      return F_Dual_Quat(this->m_real.conjugate(), this->m_dual.conjugate());
   }

   void F_Dual_Quat::normalize()
   {
      // how to do this:
      // Note: A dual quat is a quat in which each term has been replaced by a dual number.
      // The new set of terms can be re-organized to be equivalent to dual number notation 
      // in which the real and dual components are quats instead of real numbers.
      // Just as a quat is made of real numbers and its magnitude is also a real number, so
      // a dual quat is made of dual numbers and its magnitude is a dual number.  
      // To normalize a dual quat, we must divide it by its magnitude, and we can do this 
      // using use the rules for dual number division.

      F_Dual_Number dn = this->magnitude();

      // divide by the dual quat by its magnitude
      // Note: Here's how this is going to go down:
      // Let there be two dual numbers dn1 = a + (b)e, dn2 = c + (d)e, where a,b,c,d and 
      // real numbers and 'e' is the dual operator.
      // Then dn1 / dn2 is defined as follows:
      // dn1 / dn2 = (a + (b)e) / (c + (d)e)
      // 
      // Unlike real number division (ex: 5 / 11), this form is not helpful because it is 
      // not defined for dual numbers, but we can turn it into a helpful form by making it 
      // so that we are dividing by a scalar only, which is the same as multiplying all of 
      // the numerator's components by the inverse of that scalar, which is another scalar,
      // and that operation IS defined.  We now multiply the top and bottom by the conjugate
      // of the denominator:
      // (a + (b)e)(c - (d)e) / (c + (d)e)(c - (d)e)
      //
      // Recall that the dual operator is defined such that e^2 = 0.  This fraction now 
      // simplifies as follows:
      // (ac + (-ad + bc)e) / c^2
      //
      // Now suppose that a,b,c,d are quaternions.  This makes the numerator (top) and 
      // denominator (bottom) dual quaternions.  But if the denominator quaternion has 
      // zero vectors on its real and dual parts, which would result when calculating
      // its magnitude, then the denominator becomes a dual number.  But what about the
      // numerator?  That's a dual quaternion, which is not a dual number, so do dual
      // number divisioin rules apply?
      // 
      // A dual quat is a quat that is composed of dual numbers in each of its four 
      // components.  Consider the following:
      // quat q = w (a shorter and common term for scalar) + xi + yj + zk, where w,x,y,z are reals
      // dual quat dq = (w + w'e) + (x + x'e)i + (y + y'e)j + (z + z'e)k, where w,x,y,z are reals, and w',x',y',z' are reals in the dual part
      // 
      // *In case you are curious*, dq is rearranged to make the following:
      // dq = (w + xi + yj + zk) + (w' + x'i + y'j + z'k)e
      // dq = q + q'e, which the form of the dual quat that we know and love
      //
      // A quaternion has real components in all four positions, but it can still be 
      // divided by a single real number 'r' as follows:
      // q / r = (w/r) + (x/r)i + (y/r)j + (z/r)k
      // 
      // Similarly, a dual quat has dual numbers in all its components, so it can be 
      // divided by a dual number dn = r + r'e as follows:
      // dq / dn = (w + w'e)/(r + r'e) + ((x + x'e)/(r + r'e))i + ((y + y'e)/(r + r'e))j + ((z + z'e)/(r + r'e))k
      //
      // To save time and space, I will tell you that, in the dual number division
      // dn1 / dn2 = (a + (b)e) / (c + (d)e), the numerator can be a dual quat and
      // the denominator can be a dual number.  The math works out.
      // So let there be a dual quat dq and it's magnitude dn.
      // Then dq_normalized = dq / dn = (a + (b)e) / (c + (d)e) = (ac + (-ad + bc)e) / c^2, where a, b are quats and 'c', 'd' are real numbers.

      F_Quat a = this->m_real;
      F_Quat b = this->m_dual;
      float c = dn.m_real;
      float inverse_c = 1 / c;
      float inverse_c_squared = inverse_c * inverse_c;
      float d = dn.m_dual;

      // ac / c^2 = a/c
      F_Quat new_real = a * inverse_c;

      // (-ad + bc)e / c^2
      F_Quat new_dual = ((-1.0f) * a * d * inverse_c_squared) + (b * inverse_c);


      // assign the values
      this->m_real = new_real;
      this->m_dual = new_dual;
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
