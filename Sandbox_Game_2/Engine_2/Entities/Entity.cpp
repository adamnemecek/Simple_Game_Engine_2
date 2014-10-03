#include "Entity.h"

// for checking that memory pools don't overflow
#include <cassert>

// so we can assign each component's "containing entity" pointer
#include <Entities\Game_Component.h>

#include <string>
using std::string;

#include <Misc\Typedefs.h>


namespace Entities
{
   Entity::Entity()
      : m_num_current_components(0)
   {
      m_components.reserve(m_MAX_COMPONENTS);
   }

   bool Entity::initialize()
   {
      for (uint index = 0; index < m_num_current_components; index++)
      {
         if (!m_components[index].initialize())
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
         if (!m_components[index].shutdown())
         {
            return false;
         }
      }

      return true;
   }

   void add_component(const std::string &component_type)
   {
      if ("physics" == component_type)
      {

      }
   }

   // goes through all components and runs their update() function
   void update();

}
