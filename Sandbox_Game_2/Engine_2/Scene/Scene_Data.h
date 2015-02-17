#ifndef ENGINE_SCENE_DATA_H
#define ENGINE_SCENE_DATA_H

// for storing std:unique_ptr
#include <memory>
#include <vector>

// I can't forward declare these things for the std::unique_ptr, so I 
// have to include the whole definition
#include <Entities\Entity.h>
#include <Entities\Components\Physics_Component.h>
#include <Entities\Components\AABB_Component.h>
#include <Entities\Components\Controller_Component.h>
#include <Shapes\Geometry.h>

// misc
#include <string>
#include <Rendering\Camera.h>

// forward declarations
namespace rapidxml
{
   template<class Ch = char>
   class xml_document;
}

namespace Shapes
{
   class Geometry;
}

namespace Rendering
{
   class Renderer;
}

namespace Scene
{
   class __declspec(dllexport) Scene_Data
   {
   public:
      // I explicitly hide the copy constructor and assignment operator,
      // so I also have to explicitly declare a public constructor 
      Scene_Data() {}

      bool initialize();
      void set_render(Rendering::Renderer *renderer_ptr);

      void update();

      bool load(const std::string& file_path);
      bool save(const std::string& file_path);

      Entities::Entity *new_entity(const std::string& new_entity_id_str);
      Entities::Physics_Component *new_physics_component();
      Entities::AABB_Component *new_AABB_component();
      Entities::Controller_Component *new_controller_component();
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
      // Note: Wherever the renderer is kept it should be in the same scope as the
      // scene so that they both die at the same time.  The renderer makes use of
      // entity transforms, and if those entities die and the renderer doesn't, then
      // there will be trouble.
      Rendering::Renderer *m_renderer_ptr;

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
      std::vector<std::unique_ptr<Entities::Physics_Component>> m_physics_components;
      std::vector<std::unique_ptr<Entities::AABB_Component>> m_AABB_components;
      std::vector<std::unique_ptr<Entities::Controller_Component>> m_controller_components;
      std::vector<std::unique_ptr<Shapes::Geometry>> m_geometry_ptrs;
      std::vector<std::pair<const Entities::Entity *, const Shapes::Geometry*>> m_entity_geometry_pairings;
   };
}

#endif