#include "Renderable_Updater_Component.h"

#include <glm\mat4x4.hpp>
#include <Utilities\Include_Helper_GLM_Mat_Transform.h>
#include <Utilities\Include_Helper_WORLD_UP_VECTOR.h>
#include <Rendering\Renderable.h>
#include <Entities\Entity.h>
#include <glm\gtc\quaternion.hpp>

namespace Entities
{
   void Renderable_Updater_Component::update()
   {
      //glm::vec3 &orientation_ref = m_parent_entity_ptr->m_base_orientation;

      // take the arcsine of the opposite over adjacent sides of a triangle made up by the orientation vector
      //float rot_angle_rad = asinf(orientation_ref.y / glm::length(orientation_ref));

      //glm::vec3 rotation_axis = glm::cross(orientation_ref, Utilities::WORLD_UP_VECTOR);

      m_renderable_ptr->m_model_to_world_mat =
         glm::translate(glm::mat4(), m_parent_entity_ptr->m_position) *
         glm::mat4_cast(m_parent_entity_ptr->m_base_orientation);
         //glm::rotate(glm::mat4(), rot_angle_rad, rotation_axis);
   }

   void Renderable_Updater_Component::set_renderable(Rendering::Renderable *renderable_ptr)
   {
      m_renderable_ptr = renderable_ptr;
   }

}