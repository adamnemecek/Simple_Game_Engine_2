#include "Renderable_Updater_Component.h"

#include <Rendering\Renderable.h>
#include <Entities\Entity.h>


namespace Entities
{
   void Renderable_Updater_Component::update()
   {
      m_renderable_ptr->m_model_to_world_mat = m_parent_entity_ptr->m_where_and_which_way.to_mat4();
   }

   void Renderable_Updater_Component::set_renderable(Rendering::Renderable *renderable_ptr)
   {
      m_renderable_ptr = renderable_ptr;
   }

}