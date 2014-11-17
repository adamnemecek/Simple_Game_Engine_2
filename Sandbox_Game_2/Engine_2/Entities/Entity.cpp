#include "Entity.h"

// for checking that memory pools don't overflow
#include <Utilities\My_Assert.h>

// so we can assign each component's "containing entity" pointer and reject the controller component
#include <Entities\Game_Component.h>
#include <Entities\Components\Controller_Component.h>

#include <memory>
using std::memset;

#include <Utilities\Typedefs.h>


namespace Entities
{
   Entity::Entity()
      : m_num_current_components(0),
      m_where_and_which_way(Math::F_Quat::generate_real_quat(1.0f), Math::F_Quat())
   {
      memset(m_components, 0, sizeof(Game_Component *) * m_MAX_COMPONENTS);
   }

   bool Entity::initialize()
   {
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
      MY_ASSERT(m_num_current_components != m_MAX_COMPONENTS);
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
