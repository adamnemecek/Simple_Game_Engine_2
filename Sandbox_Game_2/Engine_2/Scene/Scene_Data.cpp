#include "Scene_Data.h"


namespace Scene
{
   bool Scene_Data::initialize()
   {
      m_entities.clear();
      m_geometries.clear();

      return true;
   }

   Entities::Entity *Scene_Data::new_entity(const std::string& new_entity_id)
   {
      m_entities.push_back(Entities::Entity(new_entity_id));

      // return a pointer to the newly created entity
      return &(m_entities[m_entities.size() - 1]);
   }

   Shapes::Geometry *Scene_Data::new_geometry(const Shapes::Shape_Data& new_shape_data)
   {
      m_geometries.push_back(Shapes::Geometry(new_shape_data));

      // return a pointer to the newly created geometry
      return &(m_geometries[m_geometries.size() - 1]);
   }

   Shapes::Geometry *Scene_Data::geometry_already_loaded(const std::string& shape_id_str)
   {
      for (uint geometry_index_counter = 0; geometry_index_counter < m_geometries.size(); geometry_index_counter++)
      {
         //if (parameter_list == m_geometries[geometry_index_counter].get_shape_parameter_list())
         if (shape_id_str == m_geometries[geometry_index_counter].get_shape_id_string())
         {
            return &m_geometries[geometry_index_counter];
         }
      }

      return 0;
   }

}