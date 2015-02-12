#include "Scene_Data.h"


namespace Scene
{
   bool Scene_Data::initialize()
   {
      m_entities.clear();
      m_geometries.clear();

      return true;
   }

   Entities::Entity* Scene_Data::new_entity(const std::string& new_entity_id)
   {
      m_entities.push_back(Entities::Entity(new_entity_id));

      // return a pointer to the newly created entity
      return &(m_entities[m_entities.size() - 1]);
   }
}