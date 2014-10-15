#include "Keyboard_Bindings.h"

// for VK_some_key
#include <Utilities\Include_Windows_Light.h>

#include <cassert>
#include <Input\Action_List.h>

namespace Input
{
   uint Keyboard_Bindings::get_active_actions() const
   {
      uint active_actions = 0;
      
      if (GetAsyncKeyState(VK_UP))
      {
         active_actions |= Action_List::ACCELERTATE;
      }

      if (GetAsyncKeyState(VK_LEFT))
      {
         active_actions |= Action_List::ROTATE_LEFT;
      }

      if (GetAsyncKeyState(VK_LEFT))
      {
         active_actions |= Action_List::ROTATE_RIGHT;
      }

      return active_actions;
   }
}