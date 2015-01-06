#ifndef ENGINE_CONTROLLER_COMPONENT_H
#define ENGINE_CONTROLLER_COMPONENT_H

#include <Entities\Game_Component.h>
#include <Input\Supported_Bindings.h>

#include <Rendering\Camera.h>

namespace Input
{
   // forward declaration of an interface for key-action bindings
   class I_Key_Bindings;
}

namespace Entities
{
   class Physics_Component;

   class __declspec(dllexport) Controller_Component : public Game_Component
   {
   public:
      // ensure that pointers are initialized to 0
      Controller_Component()
         : m_easy_physics_ptr(0),
         m_key_binder_ptr(0),
         m_camera_ptr(0)
      {}

      void set_camera(Rendering::Camera *camera_ptr);

      // override base class functionality
      bool initialize();
      void update();

      // unique functionality to controller
      bool set_key_binding(const Input::SUPPORTED_BINDINGS binding);

   private:
      Rendering::Camera *m_camera_ptr;

      Physics_Component *m_easy_physics_ptr;
      Input::I_Key_Bindings *m_key_binder_ptr;
   };
}

#endif
