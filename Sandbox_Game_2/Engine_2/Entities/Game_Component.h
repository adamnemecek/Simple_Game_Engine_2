#ifndef ENGINE_GAME_COMPONENT_H
#define ENGINE_GAME_COMPONENT_H

namespace Entities
{
   // just declare it so you don't have to include anything
   class Entity;

   // ??necessary to export base class to dll when only it's derived class are used??
   class Game_Component
   {
   public:
      virtual bool initialize() = 0;
      virtual bool shutdown() = 0;
      virtual void update() = 0;

   private:
      // the Entity class will need to set the "containing entity" pointer, but 
      // nothing else should be able to access this pointer, so specify a friend class and
      // do not make a public setter
      friend class Entity;

   protected:
      // the Game Component might need to access it's sibling components, and this is done by going through it's parent entity
      Entity *m_parent_entity;
   };
}

#endif
