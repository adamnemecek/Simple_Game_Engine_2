#ifndef ENGINE_I_ACTION_BINDINGS_H
#define ENGINE_I_ACTION_BINDINGS_H


namespace Input
{
   // an interface, so it has purely virtual methods
   class I_Action_Bindings
   {
   public:
      virtual uint get_active_actions() const = 0;

   private:
      uint m_actions_previous;
   };
}

#endif