#ifndef ENGINE_CONTROLLABLE_ENTITY_H
#define ENGINE_CONTROLLABLE_ENTITY_H

#include <Entities\Basic_Entity.h>

namespace Entities
{
   class __declspec(dllexport) Controllable_Entity : public Basic_Entity
   {
   public:
      // override base class functionality
      void Controllable_Entity::add_component(Game_Component *component_ptr);
   };
}

#endif
