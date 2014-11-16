#include "Renderable_Updater_Component.h"

#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>
#include <Utilities\Include_Helpers\GLM_Mat_Transform.h>
#include <Utilities\Include_Helpers\Default_Vectors.h>
#include <Rendering\Renderable.h>
#include <Entities\Entity.h>
#include <Utilities\Quaternion_Helper.h>
//#include <Utilities\Include_Helpers\GLM_Quaternion.h>
#include <Utilities\Include_Helpers\GLM_Dual_Quaternion.h>


namespace Entities
{
   void Renderable_Updater_Component::update()
   {
      //// declare this to avoid excess dereferencing later
      //glm::vec3 &orientation_ref = m_parent_entity_ptr->m_base_orientation;

      //// take the arcsine of the opposite over adjacent sides of a triangle made up by the orientation vector
      //float rot_angle_rad = asinf(orientation_ref.y / glm::length(orientation_ref));

      //glm::vec3 rotation_axis = glm::cross(Utilities::Default_Vectors::WORLD_UP_VECTOR, orientation_ref);

      //m_renderable_ptr->m_model_to_world_mat =
      //   glm::translate(glm::mat4(), m_parent_entity_ptr->m_position) *
      //   glm::rotate(glm::mat4(), rot_angle_rad, rotation_axis);


      // replaced with dual quaternion
      //glm::mat4 rotation_matrix = glm::mat4_cast(m_parent_entity_ptr->m_base_orientation_quat);
      //m_renderable_ptr->m_model_to_world_mat =
      //   glm::translate(glm::mat4(), m_parent_entity_ptr->m_position) *
      //   rotation_matrix;


      //m_renderable_ptr->m_model_to_world_mat = Utilities::Quaternion_Helper::dual_quat_to_mat4(m_parent_entity_ptr->m_where_and_which_way);
      m_renderable_ptr->m_model_to_world_mat = m_parent_entity_ptr->m_where_and_which_way.to_mat4();


      //// update the entity's orientation vector for the sake of the camera
      //glm::vec3 forward_vector = m_parent_entity_ptr->m_base_orientation;
      //m_parent_entity_ptr->m_base_orientation = glm::mat3(rotation_matrix) * forward_vector;
   }

   void Renderable_Updater_Component::set_renderable(Rendering::Renderable *renderable_ptr)
   {
      m_renderable_ptr = renderable_ptr;
   }

}