#include "Keyboard_Bindings.h"

// for VK_some_key
#include <Utilities\Include_Windows_Light.h>

#include <cassert>
#include <Input\Action_List.h>

namespace Input
{
   I_Key_Bindings *Keyboard_Bindings::get_instance_ptr()
   {
      Keyboard_Bindings instance;

      return &instance;
   }

   uint Keyboard_Bindings::get_active_actions() const
   {
      uint active_actions = 0;
      
      if (GetAsyncKeyState(VK_UP))
      {
         active_actions |= ACTION_LIST::ACCELERTATE;
      }

      if (GetAsyncKeyState(VK_LEFT))
      {
         active_actions |= ACTION_LIST::ROTATE_LEFT;
      }

      if (GetAsyncKeyState(VK_LEFT))
      {
         active_actions |= ACTION_LIST::ROTATE_RIGHT;
      }

      return active_actions;
   }
}