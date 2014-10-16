#ifndef ENGINE_I_KEY_BINDINGS_H
#define ENGINE_I_KEY_BINDINGS_H

#include <Utilities\Typedefs.h>

namespace Input
{
   // derived classes will instantiate this functionality, and they are also expected
   class __declspec(dllexport) I_Key_Bindings
   {
   public:
      // for primitive variable initialization
      I_Key_Bindings() 
         : m_actions_previous(0)
      {}

      virtual uint get_active_actions() const = 0;

   protected:
      // this member variable is common to all key bindings, but it cannot be private 
      // because of class inheritance rules, and I don't want it to be public, so make 
      // it protected
      uint m_actions_previous;
   };
}

#endif