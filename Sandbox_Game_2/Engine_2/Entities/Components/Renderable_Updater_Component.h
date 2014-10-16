#ifndef ENGINE_RENDERABLE_UPDATER_COMPONENT_H
#define ENGINE_RENDERABLE_UPDATER_COMPONENT_H

#include <Entities\Game_Component.h>

namespace Rendering
{
   class Renderable;
}

namespace Entities
{
   class __declspec(dllexport) Renderable_Updater_Component : public Game_Component
   {
   public:
      // override base class functionality
      // Note: This function updates the renderable's "model to world" matrix with the entity's position and base orientation.
      void update();

      // Note: The argument cannot be a const ptr because the member variable that is set to it is cannot be const.
      void set_renderable(Rendering::Renderable *renderable_ptr);
   private:
      // Note: Cannot be const because the update() function will change it.
      Rendering::Renderable *m_renderable_ptr;
   };
}

#endif
