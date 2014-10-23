#include "Entity.h"

// for checking that memory pools don't overflow
#include <cassert>

// so we can assign each component's "containing entity" pointer and reject the controller component
#include <Entities\Game_Component.h>
#include <Entities\Components\Controller_Component.h>

#include <memory>
using std::memset;

#include <Utilities\Typedefs.h>


namespace Entities
{
   Entity::Entity()
      : m_num_current_components(0)
   {
      memset(m_components, 0, sizeof(Game_Component *) * m_MAX_COMPONENTS);
   }

   bool Entity::initialize()
   {
      // give it a non-0 orientation by defaulting to the -Z axis
      m_base_orientation.z = -1.0f;


      for (uint index = 0; index < m_num_current_components; index++)
      {
         if (!m_components[index]->initialize())
         {
            return false;
         }
      }

      return true;
   }

   bool Entity::shutdown()
   {
      for (uint index = 0; index < m_num_current_components; index++)
      {
         if (!m_components[index]->shutdown())
         {
            return false;
         }
      }

      return true;
   }

   void Entity::add_component(Game_Component *component_ptr)
   {
      // ??remove this assertion??
      assert(m_num_current_components != m_MAX_COMPONENTS);
      m_components[m_num_current_components++] = component_ptr;
      component_ptr->m_parent_entity_ptr = this;
   }

   // goes through all components and runs their update() function
   void Entity::update()
   {
      for (uint index = 0; index < m_num_current_components; index++)
      {
         m_components[index]->update();
      }
   }

   void Entity::rotate_me(glm::vec3 rotation_axis, const float rotation_angle_rad)
   {
      glm::vec3 normalized_rotation_axis = glm::normalize(rotation_axis);

      normalized_rotation_axis *= sinf(rotation_angle_rad / 2.0f);
      float scalar = cosf(rotation_angle_rad / 2.0f);

      glm::fquat local_offset(
         scalar,
         normalized_rotation_axis.x,
         normalized_rotation_axis.y,
         normalized_rotation_axis.z);

      // assuming right multiplication only for now
      m_base_orientation = glm::normalize(m_base_orientation * local_offset);
   }

   glm::mat4 Entity::get_rotation_matrix()
   {
      return glm::mat4_cast(m_base_orientation);
   }
}
