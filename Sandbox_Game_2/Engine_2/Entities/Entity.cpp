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
      //m_base_orientation.z = -1.0f;
      m_base_orientation_quat = glm::fquat(1.0f, 0.0f, 0.0f, 0.0f);


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
}
