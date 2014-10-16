#include "Renderable_Updater_Component.h"

#include <glm\mat4x4.hpp>
#include <Utilities\Include_GLM_Mat_Transform.h>
#include <Rendering\Renderable.h>
#include <Entities\Basic_Entity.h>

namespace Entities
{
   void Renderable_Updater_Component::update()
   {
      glm::vec3 &orientation_ref = m_parent_entity_ptr->m_base_orientation;

      // take the arcsine of the opposite over adjacent sides of a triangle made up by the orientation vector
      float rot_angle_rad = asinf(orientation_ref.y / glm::length(orientation_ref));

      // just hardcode the world-up vector as the Y axis and be done with it
      glm::vec3 rotation_axis = glm::cross(orientation_ref, glm::vec3(0.0f, +1.0f, 0.0f));

      m_renderable_ptr->m_model_to_world_mat =
         glm::translate(glm::mat4(), m_parent_entity_ptr->m_position) *
         glm::rotate(glm::mat4(), rot_angle_rad, rotation_axis);
   }

   void Renderable_Updater_Component::set_renderable(Rendering::Renderable *renderable_ptr)
   {
      m_renderable_ptr = renderable_ptr;
   }

}