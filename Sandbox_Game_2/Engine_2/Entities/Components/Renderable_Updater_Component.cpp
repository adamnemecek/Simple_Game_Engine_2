#include "Renderable_Updater_Component.h"

#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>
#include <Utilities\Include_Helper_GLM_Mat_Transform.h>
#include <Utilities\Include_Helper_Default_Vectors.h>
#include <Rendering\Renderable.h>
#include <Entities\Entity.h>

namespace Entities
{
   void Renderable_Updater_Component::update()
   {
      glm::vec3 &orientation_ref = m_parent_entity_ptr->m_base_orientation;

      // take the arcsine of the opposite over adjacent sides of a triangle made up by the orientation vector
      float rot_angle_rad = asinf(orientation_ref.y / glm::length(orientation_ref));

      glm::vec3 rotation_axis = glm::cross(Utilities::Default_Vectors::WORLD_UP_VECTOR, orientation_ref);

      m_renderable_ptr->m_model_to_world_mat =
         glm::translate(glm::mat4(), m_parent_entity_ptr->m_position) *
         glm::rotate(glm::mat4(), rot_angle_rad, rotation_axis);
   }

   void Renderable_Updater_Component::set_renderable(Rendering::Renderable *renderable_ptr)
   {
      m_renderable_ptr = renderable_ptr;
   }

}