#ifndef ENGINE_GAME_COMPONENT_H
#define ENGINE_GAME_COMPONENT_H

namespace Entities
{
   // just declare it so you don't have to include anything
   class Basic_Entity;
   class Controllable_Entity;

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
      // the Game Component might need to access it's sibling components, and this is done by going through it's parent entity
      Basic_Entity *m_parent_entity_ptr;

   private:
      // the Entity classe(s) will need to set the "containing entity" pointer, but 
      // nothing else should be able to access this pointer, so specify a friend class and
      // do not make a public setter
      friend class Basic_Entity;
      friend class Controllable_Entity;
   };
}

#endif
