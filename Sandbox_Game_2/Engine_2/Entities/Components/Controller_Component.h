#ifndef ENGINE_CONTROLLER_COMPONENT_H
#define ENGINE_CONTROLLER_COMPONENT_H

#include <Entities\Game_Component.h>

namespace Entities
{
   class Physics_Component;

   class __declspec(dllexport) Controller_Component : public Game_Component
   {
   public:
      bool initialize();
      void update();

   private:
      Physics_Component *m_easy_physics_ptr;
   };
}

#endif
