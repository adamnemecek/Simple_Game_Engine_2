#include "Float_Quat.h"

// for glm::normalize(...), glm::dot(...), and glm::cross(...) overloaded for vec3
#include <glm\detail\func_geometric.hpp>
#include <math.h>

namespace Math
{
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

      float scalar = cosf(rotation_angle_rad / 2.0f);

      return Float_Quat(scalar, rotator_vector);
   }

   Float_Quat Float_Quat::operator *= (const float right)
   {
      // multiply the scalar through
      return Float_Quat(this->m_scalar * right, this->m_vector * right);
   }

   Float_Quat Float_Quat::operator *= (const Float_Quat &right)
   {

      return Float_Quat();
   }

   Float_Quat Float_Quat::conjugate()
   {
      return Float_Quat();
   }

}