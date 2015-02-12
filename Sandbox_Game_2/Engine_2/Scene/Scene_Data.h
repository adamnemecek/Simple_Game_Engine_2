#ifndef ENGINE_SCENE_DATA_H
#define ENGINE_SCENE_DATA_H

#include <vector>
#include <string>
#include <Entities\Entity.h>
#include <Shapes\Geometry.h>

namespace Scene
{
   class __declspec(dllexport) Scene_Data
   {
   public:
      bool initialize();

      Entities::Entity* new_entity(const std::string& new_entity_id);

   private:
      std::vector<Entities::Entity> m_entities;
      //std::vector<Shapes::Geometry> m_geometries;
   };
}

#endif