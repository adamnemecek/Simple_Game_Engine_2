#include "Controller_Component.h"

// for setting a particular key binding
#include <Input\Supported_Bindings.h>
#include <Input\Key_Bindings\Keyboard_Bindings.h>

// for getting at the physics component
#include <Entities\Entity.h>
#include <Entities\Components\Physics_Component.h>

// for figuring out which actions are active
#include <Input\Action_List.h>

#include <cassert>

namespace Entities
{
   bool Controller_Component::initialize()
   {
      m_easy_physics_ptr = m_parent_entity_ptr->get_component_ptr<Physics_Component>();
      return (m_easy_physics_ptr != 0);
   }

   void Controller_Component::update()
   {
      using Input::ACTION_LIST;
      assert(m_key_binder_ptr != 0);
      assert(m_easy_physics_ptr != 0);
      
      uint active_actions = m_key_binder_ptr->get_active_actions();

      if (active_actions & ACTION_LIST::ACCELERTATE)
      {

      }
      if (active_actions & ACTION_LIST::ROTATE_LEFT)
      {

      }
      if (active_actions & ACTION_LIST::ROTATE_RIGHT)
      {

      }
   }

   bool Controller_Component::set_key_binding(const Input::SUPPORTED_BINDINGS binding)
   {
      using Input::SUPPORTED_BINDINGS;

      switch (binding)
      {
      case SUPPORTED_BINDINGS::KEYBOARD:
         m_key_binder_ptr = Input::Keyboard_Bindings::get_instance_ptr();
      default:
      {
         // unknown binding
         return false;
      }
      }

      return true;
   }
}