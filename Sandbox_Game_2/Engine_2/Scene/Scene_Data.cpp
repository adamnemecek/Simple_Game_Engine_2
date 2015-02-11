#include "Scene_Data.h"


namespace Scene
{
   void Scene_Data::initialize()
   {
      m_entities.clear();
      m_geometries.clear();
   }

   Entities::Entity* Scene_Data::new_entity()
   {
      m_entities.push_back(Entities::Entity());

      return &(m_entities[m_entities.size() - 1]);
   }
}