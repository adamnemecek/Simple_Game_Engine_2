#ifndef ENGINE_I_KEY_BINDINGS_H
#define ENGINE_I_KEY_BINDINGS_H

#include <Utilities\Typedefs.h>

namespace Input
{
   // derived classes will instantiate this functionality, and they are also expected
   class __declspec(dllexport) I_Key_Bindings
   {
   public:
      virtual uint get_active_actions() const = 0;

   private:
      uint m_actions_previous;
   };
}

#endif