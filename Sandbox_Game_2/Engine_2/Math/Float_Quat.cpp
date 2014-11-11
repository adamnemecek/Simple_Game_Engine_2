#include "Float_Quat.h"

// for glm::normalize(...), glm::dot(...), and glm::cross(...) overloaded for vec3
#include <glm\detail\func_geometric.hpp>
#include <math.h>

namespace Math
{
   // method functions

   Float_Quat::Float_Quat()
      : m_scalar(0),
      m_vector(glm::vec3(0))
   {
   }

   Float_Quat::Float_Quat(const float scalar, const glm::vec3 &vector)
      : m_scalar(scalar),
      m_vector(vector)
   {
   }

   Float_Quat Float_Quat::generate_real_quat(const float new_scalar)
   {
      return Float_Quat(new_scalar, glm::vec3(0));
   }

   Float_Quat Float_Quat::generate_pure_quat(const glm::vec3 &new_vector)
   {
      return Float_Quat(0, new_vector);
   }

   Float_Quat Float_Quat::generate_rotator(const glm::vec3 &rotation_axis, const float rotation_angle_rad)
   {
      glm::vec3 normalized_rotation_axis = glm::normalize(rotation_axis);
      glm::vec3 rotator_vector = normalized_rotation_axis * sinf(rotation_angle_rad / 2.0f);
      //glm::vec3 rotator_vector = normalized_rotation_axis * sinf(rotation_angle_rad);

      float scalar = cosf(rotation_angle_rad / 2.0f);
      //float scalar = cosf(rotation_angle_rad);

      return Float_Quat(scalar, rotator_vector);
   }

   Float_Quat Float_Quat::operator *= (const float right)
   {
      // multiply the scalar through
      return Float_Quat(this->m_scalar * right, this->m_vector * right);
   }

   Float_Quat Float_Quat::operator *= (const Float_Quat &right)
   {
      float new_scalar = this->m_scalar * right.m_scalar - glm::dot(this->m_vector, right.m_vector);
      glm::vec3 new_vector = this->m_scalar * right.m_vector + right.m_scalar * this->m_vector + glm::cross(this->m_vector, right.m_vector);

      return Float_Quat(new_scalar, new_vector);
   }

   Float_Quat Float_Quat::conjugate() const
   {
      return Float_Quat(this->m_scalar, (-1.0f) * this->m_vector);
   }

   float Float_Quat::magnitude() const
   {
      float mag_squared = this->magnitude_squared();

      return sqrtf(mag_squared);
   }

   float Float_Quat::magnitude_squared() const
   {
      Float_Quat mag_square = (*this) * this->conjugate();
      
      return (mag_square).m_scalar;
   }

   void Float_Quat::normalize()
   {
      float inverse_mag = 1.0f / (this->magnitude());
      (*this) *= inverse_mag;
   }

   Float_Quat Float_Quat::inverse() const
   {
      Float_Quat conj = this->conjugate();
      float inverse_mag_square = 1.0f / (this->magnitude_squared());

      return conj * inverse_mag_square;
   }


   // non-method functions

   Float_Quat operator*(const float left, const Float_Quat &right)
   {
      return Float_Quat(left * right.m_scalar, left * right.m_vector);
   }

   Float_Quat operator*(const Float_Quat &left, const float right)
   {
      return Float_Quat(left.m_scalar * right, left.m_vector * right);
   }

   Float_Quat operator*(const Float_Quat &left, const Float_Quat &right)
   {
      float new_scalar = left.m_scalar * right.m_scalar - glm::dot(left.m_vector, right.m_vector);
      glm::vec3 new_vector = left.m_scalar * right.m_vector + right.m_scalar * left.m_vector + glm::cross(left.m_vector, right.m_vector);

      return Float_Quat(new_scalar, new_vector);
   }

   Float_Quat normalize(const Float_Quat &fq)
   {
      float f = fq.magnitude_squared();
      float inverse_mag = 1.0f / (fq.magnitude());

      return (fq * inverse_mag);
   }
}