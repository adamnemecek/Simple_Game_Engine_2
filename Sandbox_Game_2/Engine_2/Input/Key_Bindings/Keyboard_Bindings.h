#ifndef ENGINE_KEYBOARD_BINDINGS_H
#define ENGINE_KEYBOARD_BINDINGS_H


#include <Input\I_Key_Bindings.h>
#include <Utilities\Typedefs.h>

namespace Input
{
   // this is a singleton, but it is also derived from an interface, so we must return an interface pointer from "get instance"
   class __declspec(dllexport) Keyboard_Bindings : public I_Key_Bindings
   {
   public:
      static I_Key_Bindings *get_instance_ptr();

      // override base class functionality
      uint get_active_actions() const;

   private:
      Keyboard_Bindings();
      Keyboard_Bindings(Keyboard_Bindings &);
      Keyboard_Bindings &operator=(Keyboard_Bindings &);
   };
}

#endif
