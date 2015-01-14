#include "F_Quat.h"

// for glm::normalize(...), glm::dot(...), and glm::cross(...) overloaded for vec3
#include <glm\detail\func_geometric.hpp>
#include <math.h>

namespace Math
{
   // method functions

   F_Quat::F_Quat()
      : m_scalar(0),
      m_vector(glm::vec3(0))
   {
   }

   F_Quat::F_Quat(const float scalar, const glm::vec3 &vector)
      : m_scalar(scalar),
      m_vector(vector)
   {
   }

   F_Quat F_Quat::generate_real_quat(const float new_scalar)
   {
      return F_Quat(new_scalar, glm::vec3(0));
   }

   F_Quat F_Quat::generate_pure_quat(const glm::vec3 &new_vector)
   {
      return F_Quat(0, new_vector);
   }

   F_Quat F_Quat::generate_rotator(const glm::vec3 &rotation_vector, const float rotation_angle_rad)
   {
      glm::vec3 normalized_rotation_vector = glm::normalize(rotation_vector);
      glm::vec3 rotator_vector = normalized_rotation_vector * sinf(rotation_angle_rad / 2.0f);

      float scalar = cosf(rotation_angle_rad / 2.0f);

      return F_Quat(scalar, rotator_vector);
   }

   F_Quat F_Quat::normalize(const F_Quat &q)
   {
      float inverse_mag = 1.0f / (q.magnitude());

      // the F_Quat has an operator for '* float', but not for '/ float'
      return (q * inverse_mag);
   }

   void F_Quat::operator = (const F_Quat &right)
   {
      this->m_scalar = right.m_scalar;
      this->m_vector = right.m_vector;
   }

   void F_Quat::operator +=(const F_Quat &right)
   {
      this->m_scalar = this->m_scalar + right.m_scalar;
      this->m_vector = this->m_vector + right.m_vector;
   }

   void F_Quat::operator *= (const float right)
   {
      // multiply the scalar through
      this->m_scalar *= right;
      this->m_vector *= right;
   }

   void F_Quat::operator *= (const F_Quat &right)
   {
      this->m_scalar = this->m_scalar * right.m_scalar - glm::dot(this->m_vector, right.m_vector);
      this->m_vector = this->m_scalar * right.m_vector + right.m_scalar * this->m_vector + glm::cross(this->m_vector, right.m_vector);
   }

   F_Quat F_Quat::conjugate() const
   {
      return F_Quat(this->m_scalar, (-1.0f) * this->m_vector);
   }

   void F_Quat::normalize()
   {
      // calculate the inverse magnitude and multiply it through the 4 components
      // Note: 1 division and 4 multiplications is cheaper than 4 divisions.
      float inverse_mag = 1.0f / (this->magnitude());
      (*this) *= inverse_mag;
   }

   float F_Quat::magnitude() const
   {
      //float mag_squared = this->magnitude_squared();
      
      // optimize by squaring each term manually
      // Note: this cuts the number of multiplications from 16 to 4 and the additions from 13 to 3
      float mag_squared =
         (this->m_scalar * this->m_scalar) +
         (this->m_vector.x * this->m_vector.x) +
         (this->m_vector.y + this->m_vector.y) +
         (this->m_vector.z + this->m_vector.z);

      return sqrtf(mag_squared);
   }

   float F_Quat::magnitude_squared() const
   {
      // ??do I even need this??
      F_Quat mag_square = (*this) * this->conjugate();

      return (mag_square).m_scalar;
   }

   F_Quat F_Quat::inverse() const
   {
      F_Quat conj = this->conjugate();
      float inverse_mag_square = 1.0f / (this->magnitude_squared());

      return conj * inverse_mag_square;
   }


   // non-method functions

   F_Quat operator+(const F_Quat &left, const F_Quat &right)
   {
      return F_Quat(left.m_scalar + right.m_scalar, left.m_vector + right.m_vector);
   }

   F_Quat operator-(const F_Quat &left, const F_Quat &right)
   {
      return F_Quat(left.m_scalar - right.m_scalar, left.m_vector - right.m_vector);
   }

   F_Quat operator*(const float left, const F_Quat &right)
   {
      return F_Quat(left * right.m_scalar, left * right.m_vector);
   }

   F_Quat operator*(const F_Quat &left, const float right)
   {
      return F_Quat(left.m_scalar * right, left.m_vector * right);
   }

   F_Quat operator*(const F_Quat &left, const F_Quat &right)
   {
      float new_scalar = left.m_scalar * right.m_scalar - glm::dot(left.m_vector, right.m_vector);
      glm::vec3 new_vector = left.m_scalar * right.m_vector + right.m_scalar * left.m_vector + glm::cross(left.m_vector, right.m_vector);

      return F_Quat(new_scalar, new_vector);
   }

   bool operator==(const F_Quat &left, const F_Quat &right)
   {
      return (left.m_scalar == right.m_scalar && left.m_vector == right.m_vector);
   }

   bool operator!=(const F_Quat &left, const F_Quat &right)
   {
      return (left.m_scalar != right.m_scalar || left.m_vector != right.m_vector);
   }
}