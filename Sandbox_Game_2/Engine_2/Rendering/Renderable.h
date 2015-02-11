#ifndef ENGINE_RENDERABLE_H
#define ENGINE_RENDERABLE_H

#include <glm\mat4x4.hpp>
#include <vector>
#include <Shapes\Index_Meta_Data.h>
#include <Utilities\Include_Helpers\GL_Version.h>

// forward declarations
namespace Shapes
{
   class Geometry;
}

namespace Entities
{
   class Entity;
}

namespace Rendering
{
   // a renderable is a data store that is a middle-man between the renderer and the following:
   // - the collections of indices and the draw commands for the shape being shape being rendered
   // - the VAO binding information for the geometry (TODO: combine static geomitries into a single VAO)
   // - the entity-specific transform for the shape being rendered
   class __declspec(dllexport) Renderable
   {
   public:
      Renderable() :
         m_geometry_ptr(0),
         m_entity_ptr(0)
      {
      }

      // grab the VAO binding information from the geometry
      GLuint get_VAO_ID();

      // grab the model-to-world matrix from the entity
      glm::mat4 get_model_to_world_matrix();

      // grab the index meta data, which includes collections of indices and their associated
      // draw commands, from the Shape_Data within the geometry
      const std::vector<Shapes::Index_Meta_Data>& get_geometry_index_meta_data_collection();
      
      // set an internal pointer to the geometry that this renderable will work with
      void set_geometry(const Shapes::Geometry *geometry_ptr);

      // set an internal pointer to the entity that this renderable will work with
      void set_entity(const Entities::Entity *entity_ptr);

   private:
      const Shapes::Geometry *m_geometry_ptr;
      const Entities::Entity *m_entity_ptr;
   };
}

#endif
