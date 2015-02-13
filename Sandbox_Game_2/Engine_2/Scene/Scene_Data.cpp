#include "Scene_Data.h"

#include <Utilities\My_Assert.h>
#include <Utilities\Typedefs.h>
#include <middleware\arcsynthesis\framework\rapidxml.hpp>
#include <middleware\arcsynthesis\framework\rapidxml_helpers.h>
#include <fstream>
#include <vector>

// for generating things from the file data
#include <Entities\Entity.h>
#include <Shapes\Shape_Data.h>
#include <Shapes\Geometry_Creation\Shape_Generator.h>
#include <Shapes\Geometry.h>
#include <Math\F_Quat.h>
#include <Math\F_Dual_Quat.h>

#include <iostream>
using std::cout;
using std::endl;


// empty namespace for helper functions
namespace
{
   void helper_open_xml_file(rapidxml::xml_document<> *put_parsed_xml_document_here, std::vector<char> &file_data, const std::string &file_path)
   {
      // this function is heavily influenced by the arcsynthesis framework's Mesh 
      // class constructor, and some lines are copied verbatim except for the 
      // camel-case-to-underscore notaion change
      std::ifstream file_stream(file_path);
      if (!file_stream.is_open())
      {
         throw std::runtime_error("Could not find the mesh file: " + file_path);
      }

      //std::vector<char> file_data;

      // reserve memory
      // Note: Reserving memory ensures contiguency of memory, which allows the 
      // rapidxml parser to run through the data with just a pointer.
      file_data.reserve(2 * 1024 * 1024);
      file_data.insert(file_data.end(), std::istreambuf_iterator<char>(file_stream),
         std::istreambuf_iterator<char>());
      file_data.push_back('\0');

      try
      {
         (*put_parsed_xml_document_here).parse<0>(&(file_data[0]));
      }
      catch (rapidxml::parse_error &e)
      {
         // this is a special rapidxml exception, so handle it here
         std::cout << file_path << ": Parse error in the mesh file." << std::endl;
         std::cout << e.what() << std::endl << e.where<char>() << std::endl;
         throw;
      }
   }

   Shapes::Geometry *helper_load_geometry(const rapidxml::xml_node<> *shape_node_ptr, Scene::Scene_Data *load_into_this_scene)
   {
      if (0 != shape_node_ptr)
      {
         // no shape, so return a null pointer
         return 0;
      }

      // check if the geometry has already been loaded
      std::string shape_id_str = rapidxml::get_attrib_string(*shape_node_ptr, "id");
      Shapes::Geometry *geometry_ptr = load_into_this_scene->geometry_already_loaded(shape_id_str);

      if (0 != geometry_ptr)
      {
         // geometry has already been loaded, so return a pointer to it
         return geometry_ptr;
      }

      // geometry has not been loaded, so pick apart the node to figure out 
      // what shape to make, and then make it
      std::string shape_type_str = rapidxml::get_attrib_string(*shape_node_ptr, "value");
      Shapes::Shape_Data new_shape;
      Shapes::Geometry_Creation::Shape_Generator& shape_generator_ref = Shapes::Geometry_Creation::Shape_Generator::get_instance();
      float f_return_value_if_parameter_not_found = 0.0f;
      int i_return_value_if_parameter_not_found = 0;

      if ("triangle" == shape_type_str)
      {
         shape_generator_ref.generate_triangle(&new_shape);
         cout << "loading triangle" << endl;
      }
      else if ("plane" == shape_type_str)
      {
         shape_generator_ref.generate_plane(
            rapidxml::get_attrib_float(*shape_node_ptr, "width", f_return_value_if_parameter_not_found),
            rapidxml::get_attrib_float(*shape_node_ptr, "length", f_return_value_if_parameter_not_found),
            rapidxml::get_attrib_int(*shape_node_ptr, "width_segments", i_return_value_if_parameter_not_found),
            rapidxml::get_attrib_int(*shape_node_ptr, "length_segments", i_return_value_if_parameter_not_found),
            &new_shape);
         cout << "loading plane" << endl;
      }
      else if ("box" == shape_type_str)
      {
         shape_generator_ref.generate_box(
            rapidxml::get_attrib_float(*shape_node_ptr, "width", f_return_value_if_parameter_not_found),
            rapidxml::get_attrib_float(*shape_node_ptr, "length", f_return_value_if_parameter_not_found),
            &new_shape);
         cout << "loading box" << endl;
      }
      else if ("circle" == shape_type_str)
      {
         shape_generator_ref.generate_circle(
            rapidxml::get_attrib_int(*shape_node_ptr, "num_arc_segments", i_return_value_if_parameter_not_found),
            rapidxml::get_attrib_float(*shape_node_ptr, "radius", f_return_value_if_parameter_not_found),
            &new_shape);
         cout << "loading circle" << endl;
      }
      else if ("cube" == shape_type_str)
      {
         shape_generator_ref.generate_cube(&new_shape);
         cout << "loading cube" << endl;
      }
      else if ("cylinder" == shape_type_str)
      {
         shape_generator_ref.generate_cylinder(
            rapidxml::get_attrib_int(*shape_node_ptr, "num_arc_segments", i_return_value_if_parameter_not_found),
            rapidxml::get_attrib_float(*shape_node_ptr, "radius", f_return_value_if_parameter_not_found),
            rapidxml::get_attrib_int(*shape_node_ptr, "num_vertical_segments", i_return_value_if_parameter_not_found),
            rapidxml::get_attrib_float(*shape_node_ptr, "height", f_return_value_if_parameter_not_found),
            &new_shape);

         cout << "loading cylinder" << endl;
      }
      else if ("sphere" == shape_type_str)
      {
         shape_generator_ref.generate_sphere(
            rapidxml::get_attrib_int(*shape_node_ptr, "num_arc_segments", i_return_value_if_parameter_not_found),
            rapidxml::get_attrib_float(*shape_node_ptr, "radius", f_return_value_if_parameter_not_found),
            rapidxml::get_attrib_int(*shape_node_ptr, "num_vertical_segments", i_return_value_if_parameter_not_found),
            &new_shape);

         cout << "loading sphere" << endl;
      }
      else
      {
         // not a valid shape
         cout << "'" << shape_type_str << "' is not recognized and cannot be generated" << endl;
         return 0;
      }

      geometry_ptr = load_into_this_scene->new_geometry(new_shape);

      return geometry_ptr;
   }

   Math::F_Dual_Quat helper_get_transform(const rapidxml::xml_node<> *transform_node_ptr)
   {
      if (0 == transform_node_ptr)
      {
         // bad!
         // TODO: do something about the bad
         cout << "no transform node" << endl;
      }

      if ("dual_quaternion" == rapidxml::get_attrib_string(*transform_node_ptr, "type"))
      {
         // "r" == "real", "d" == "dual"
         float ret_val_in_case_of_failure = 0.0f;
         float rw = rapidxml::get_attrib_float(*transform_node_ptr, "rw", ret_val_in_case_of_failure);
         float rx = rapidxml::get_attrib_float(*transform_node_ptr, "rx", ret_val_in_case_of_failure);
         float ry = rapidxml::get_attrib_float(*transform_node_ptr, "ry", ret_val_in_case_of_failure);
         float rz = rapidxml::get_attrib_float(*transform_node_ptr, "rz", ret_val_in_case_of_failure);
         float dw = rapidxml::get_attrib_float(*transform_node_ptr, "dw", ret_val_in_case_of_failure);
         float dx = rapidxml::get_attrib_float(*transform_node_ptr, "dx", ret_val_in_case_of_failure);
         float dy = rapidxml::get_attrib_float(*transform_node_ptr, "dy", ret_val_in_case_of_failure);
         float dz = rapidxml::get_attrib_float(*transform_node_ptr, "dz", ret_val_in_case_of_failure);

         return Math::F_Dual_Quat(
            Math::F_Quat(rw, glm::vec3(rx, ry, rz)),
            Math::F_Quat(dw, glm::vec3(dx, dy, dz)));
      }
      else
      {
         // ??support another type??
      }

      // return a null transform
      return Math::F_Dual_Quat();
   }
}

namespace Scene
{
   bool Scene_Data::initialize()
   {
      MY_ASSERT(m_renderer.initialize());

      m_entities.clear();
      m_geometries.clear();

      return true;
   }

   bool Scene_Data::load(const std::string& file_path)
   {
      //std::string file_path = "C:/Users/John/Documents/GitHub/Simple_Game_Engine_2/scene_save_exp.xml";

      // open the file
      rapidxml::xml_document<> doc;
      std::vector<char> file_data;
      helper_open_xml_file(&doc, file_data, file_path);

      // make sure that there is a "scene" root node
      rapidxml::xml_node<> *root_node_ptr = doc.first_node("scene");
      if (0 == root_node_ptr)
      {
         throw std::runtime_error("'scene' node not found in mesh file: " + file_path);
      }

      // cycle through the list of entities and generate them one by one
      const rapidxml::xml_node<> *entity_node_ptr = root_node_ptr->first_node("entity");
      while (0 != entity_node_ptr)
      {
         // get the entity's ID string (it should have this, so let it blow up if it doesn't)
         std::string new_entity_id_str = rapidxml::get_attrib_string(*entity_node_ptr, "id");
         cout << new_entity_id_str << endl;

         // create the entity
         Entities::Entity *new_entity_ptr = this->new_entity(new_entity_id_str);

         // load the transform (it should have this, so let it blow up if it doesn't)
         const rapidxml::xml_node<> *transform_node_ptr = entity_node_ptr->first_node("transform");
         new_entity_ptr->m_where_and_which_way = helper_get_transform(transform_node_ptr);

         // check for geometry
         const rapidxml::xml_node<> *shape_node_ptr = entity_node_ptr->first_node("shape");
         if (0 != shape_node_ptr)
         {
            // shape data available, so load it and make a renderable out of it
            Shapes::Geometry *new_geometry_ptr = helper_load_geometry(shape_node_ptr, this);
            m_renderer.configure_new_renderable(new_geometry_ptr, new_entity_ptr);


         }

         // get the next entity in the node hierarchy
         entity_node_ptr = entity_node_ptr->next_sibling("entity");
      }

      return true;
   }

   bool Scene_Data::save(const std::string& file_path)
   {
      return true;
   }

   void Scene_Data::draw_scene()
   {
      m_renderer.render_scene();
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

   void Scene_Data::new_entity_geometry_pairing(const Entities::Entity *entity_ptr, const Shapes::Geometry *geo_ptr)
   {
      m_entity_geometry_pairings.push_back(std::pair<const Entities::Entity *, const Shapes::Geometry *>(entity_ptr, geo_ptr));
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