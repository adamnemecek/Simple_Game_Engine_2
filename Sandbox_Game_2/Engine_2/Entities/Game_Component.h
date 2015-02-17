#ifndef ENGINE_GAME_COMPONENT_H
#define ENGINE_GAME_COMPONENT_H

// forward declaration
namespace Entities
{
   class Entity;
   class Controllable_Entity;
}

namespace Entities
{
   // the compiler gives a warning for a non dll-interface class being used as a 
   // base for a dll-interface class when I didn't declare this pure virtual base 
   // class for dll export, so I should probably declare it
   class __declspec(dllexport) Game_Component
   {
   public:
      // for making sure that the parent entity pointer is initialized to 0
      Game_Component()
         : m_parent_entity_ptr(0) {}

      virtual bool initialize() { return true; }
      virtual bool shutdown() { return true; }
      virtual void update() = 0;

   protected:
      // the Game Component might need to access it's sibling components, and this is 
      // done by going through it's parent entity
      // Note: The Entity class itself will set this value when a Game_Component 
      // derivative is added to it.
      Entity *m_parent_entity_ptr;

   private:
      // the Entity classe(s) will need to set the "containing entity" pointer, but 
      // nothing else should be able to access this pointer, so specify a friend class and
      // do not make a public setter
      friend class Entity;
      friend class Controllable_Entity;
   };
}

#endif
