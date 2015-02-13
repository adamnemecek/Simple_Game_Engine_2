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

      Entities::Entity *new_entity(const std::string& new_entity_id);
      Shapes::Geometry *new_geometry(const Shapes::Shape_Data& new_shape_data);

      bool geometry_already_loaded(const std::vector<std::string>& parameter_list);

   private:
      // these are vectors of classes instead of pointers because I want the
      // class' destructors to be called when the scene instance dies
      std::vector<Entities::Entity> m_entities;
      std::vector<Shapes::Geometry> m_geometries;
   };
}

#endif