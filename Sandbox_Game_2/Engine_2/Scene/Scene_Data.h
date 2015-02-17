#ifndef ENGINE_SCENE_DATA_H
#define ENGINE_SCENE_DATA_H

#include <memory>
#include <vector>
#include <string>
//#include <Entities\Entity.h>
//#include <Shapes\Geometry.h>
//#include <Shapes\Shape_Data.h>
#include <Rendering\Renderer.h>
#include <Rendering\Camera.h>

// forward declarations
namespace rapidxml
{
   template<class Ch = char>
   class xml_document;
}

namespace Entities
{
   class Entity;
}

namespace Shapes
{
   class Geometry;
   struct Shape_Data;
   struct My_Vertex;
}

class my_class
{
public:

private:
};

namespace Scene
{
   class __declspec(dllexport) Scene_Data
   {
   public:
      // I explicitly hide the copy constructor and assignment operator,
      // so I also have to explicitly declare a public constructor 
      Scene_Data() {}

      bool initialize();

      bool load(const std::string& file_path);
      bool save(const std::string& file_path);

      void draw_scene();

      Entities::Entity *new_entity(const std::string& new_entity_id_str);
      Shapes::Geometry *new_geometry(const Shapes::Shape_Data *new_shape_data_ptr, const std::string& new_geometry_id_str);
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
      // hide the copy and assignment constructors 
      // Note: If I don't, then default ones will be created, and they will attempt to
      // to use the copy and assignment constructors of all class members, but the 
      // unique_ptr doesn't have these, so an error will be thrown.
      Scene_Data(const Scene_Data&);
      Scene_Data& operator=(const Scene_Data&);


      // private initialization helper functions

      bool load_renderer(const rapidxml::xml_document<> *parsed_scene_doc);
      bool load_geometries(const rapidxml::xml_document<> *parsed_scene_doc);
      bool load_entities(const rapidxml::xml_document<> *parsed_scene_doc);
      bool load_camera(const rapidxml::xml_document<> *parsed_scene_doc);


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

      // storage of things
      // Note: I am using shared pointers to manage the storage of objects because of
      // the way that vectors push things into their collection: via copy constructor.
      // Then if the local copy of the object goes out of scope, it's descructor is called,
      // killing any data that it may have pointed to.  I thought about pointers, but
      // a vector won't delete pointers.  Enter the unique pointer (I could use a shared
      // pointer, but a unique one communicates sole ownership), which is a class that 
      // manages a pointer.  When this class' destructor is called, the object dies,
      // just as I need it to.  Additionally, no copy constructor is called when I 
      // push it into a vector, and no destructor is called either.  Sweet!
      //
      // Also, I can still get at the "dumb" pointer so that I can pass it around
      // and not have to rewrite a bunch of my program.
      std::vector<std::unique_ptr<Entities::Entity>> m_entity_ptrs;
      std::vector<std::unique_ptr<Shapes::Geometry>> m_geometry_ptrs;
      std::vector<std::pair<const Entities::Entity *, const Shapes::Geometry*>> m_entity_geometry_pairings;
   };
}

#endif