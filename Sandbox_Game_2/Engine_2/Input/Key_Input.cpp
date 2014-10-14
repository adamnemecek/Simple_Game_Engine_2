#include "Key_Input.h"

#include <Input\I_Key_Mapper.h>
#include <memory>

namespace Input
{
   Key_Input &Key_Input::get_instance()
   {
      static Key_Input instance;
      return instance;
   }

   bool Key_Input::initialize()
   {
      return true;
   }

   bool Key_Input::shutdown()
   {
      return true;
   }

   bool Key_Input::set_mapper(I_Key_Mapper *key_mapper_ptr, int max_action_value)
   {
      if (0 == key_mapper_ptr) { return false; }

      this->m_key_mapper_ptr = key_mapper_ptr;
      this->m_max_action_value = max_action_value;

      return true;
   }

   void Key_Input::set_key_pressed(const char c, const int window_mouse_x, const int window_moust_y)
   {
      m_keys_pressed[(int)c] = true;
   }

   void Key_Input::update()
   {
      // reset the possible actions this frame
      m_actions_this_frame = 0;
      memset(m_keys_pressed, 0, sizeof(bool) * M_MAX_KEYS);

      int possible_action = 1;
      while (possible_action != m_max_action_value)
      {
         int key = m_key_mapper_ptr->get_key_for(possible_action);


      }
   }

}