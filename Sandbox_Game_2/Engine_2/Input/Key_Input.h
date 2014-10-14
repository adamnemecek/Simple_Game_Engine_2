#ifndef ENGINE_KEY_INPUT_H
#define ENGINE_KEY_INPUT_H

#include <Utilities\Typedefs.h>

namespace Input
{
   class I_Key_Mapper;

   class __declspec(dllexport) Key_Input
   {
   public:
      static Key_Input &get_instance();

      bool initialize();
      bool shutdown();

      bool set_mapper(I_Key_Mapper *key_mapper_ptr, int max_action_value);
      void set_key_pressed(const char c, const int window_mouse_x, const int window_moust_y);

      void update();

   private:
      // enforce singleton-ness
      Key_Input() {}
      Key_Input(const Key_Input &);
      Key_Input &operator=(const Key_Input&);

      // use magic 128 because that is the range of the base ASCII table
      static const uint M_MAX_KEYS = 128;
      bool m_keys_pressed[M_MAX_KEYS];

      I_Key_Mapper *m_key_mapper_ptr;
      int m_max_action_value;
      int m_actions_this_frame;
   };
}

#endif