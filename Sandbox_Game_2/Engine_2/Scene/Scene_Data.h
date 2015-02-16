#ifndef ENGINE_SCENE_DATA_H
#define ENGINE_SCENE_DATA_H

#include <vector>
#include <string>
#include <Entities\Entity.h>
#include <Shapes\Geometry.h>
#include <Rendering\Renderer.h>
#include <Rendering\Camera.h>

// forward declarations
namespace rapidxml
{
   template<class Ch = char>
   class xml_document;
}

namespace Scene
{
   class __declspec(dllexport) Scene_Data
   {
   public:
      bool initialize();

      bool load(const std::string& file_path);
      bool save(const std::string& file_path);

      void draw_scene();

      Entities::Entity *new_entity(const std::string& new_entity_id_str);
      Shapes::Geometry *new_geometry(const Shapes::Shape_Data& new_shape_data, const std::string& new_geometry_id_str);
      void new_entity_geometry_pairing(const Entities::Entity *entity_ptr, const Shapes::Geometry *geo_ptr);

      Entities::Entity *find_entity(const std::string& entity_id_str);
      Shapes::Geometry *find_geometry(const std::string& geometry_id_str);

      // checks if any of the already-loaded geometries have a shape that matches
      // the argument id string, which is simply a mashing of all the parameters
      // into a unique string
      // Note: This method cannot be const because it is returning a non-const value,
      // and apparently that makes it ineligible for const-ness.
      //Shapes::Geometry *geometry_already_loaded(const std::string& shape_id_str);
      Shapes::Geometry *geometry_already_loaded(const std::string& geometry_id_str);

   private:
      // private initialization helper functions

      bool load_renderer(const rapidxml::xml_document<> *parsed_scene_doc);
      bool load_geometries(const rapidxml::xml_document<> *parsed_scene_doc);
      bool load_entities(const rapidxml::xml_document<> *parsed_scene_doc);


      // keep this around so that new renderables can be added as new entities and
      // geometries are introduced
      // Note: I am keeping this as a class member instead of a pointer because it
      // only needs to live as long as the scene.  If there is no scene, then there
      // is no need of a renderer.  If there is a scene, then there only needs to be 
      // a single renderer with its collection of renderables.  When the scene object
      // dies, then all renderables should also die.
      // 
      // Making the renderer a class member accomplishes this goal.
      Rendering::Renderer m_renderer;

      // a single camera
      // TODO: ??make this a collection of cameras??
      Rendering::Camera m_camera;

      // these are vectors of classes instead of pointers because I want the
      // class' destructors to be called when the scene instance dies
      std::vector<Entities::Entity> m_entities;
      std::vector<Shapes::Geometry> m_geometries;
      std::vector<std::pair<const Entities::Entity *, const Shapes::Geometry *>> m_entity_geometry_pairings;
   };
}

#endif