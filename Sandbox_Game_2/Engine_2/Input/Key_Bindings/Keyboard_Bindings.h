#ifndef ENGINE_KEYBOARD_BINDINGS_H
#define ENGINE_KEYBOARD_BINDINGS_H


#include <Input\I_Action_Bindings.h>
#include <Utilities\Typedefs.h>

namespace Input
{
   class __declspec(dllexport) Keyboard_Bindings : public I_Action_Bindings
   {
   public:
      // override base class functionality
      uint get_active_actions() const;
   };
}

#endif
