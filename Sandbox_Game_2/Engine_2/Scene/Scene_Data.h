#ifndef ENGINE_SCENE_DATA_H
#define ENGINE_SCENE_DATA_H

#include <vector>
#include <Entities\Entity.h>
#include <Shapes\Geometry.h>

namespace Scene
{
   class __declspec(dllexport) Scene_Data
   {
   public:
      void initialize();

      Entities::Entity* new_entity();

   private:
      std::vector<Entities::Entity> m_entities;
      std::vector<Shapes::Geometry> m_geometries;
   };
}

#endif