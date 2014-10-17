#include "Controllable_Entity.h"

#include <Entities\Game_Component.h>

namespace Entities
{
   void Controllable_Entity::add_component(Game_Component *component_ptr)
   {
      // do not reject controller or camera components

      assert(m_num_current_components != m_MAX_COMPONENTS);
      m_components[m_num_current_components++] = component_ptr;
      component_ptr->m_parent_entity_ptr = this;
   }
}