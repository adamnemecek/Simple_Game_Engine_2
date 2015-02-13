#include "Scene_Loader.h"

#include <Scene\Scene_Data.h>

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
   void open_xml_file(rapidxml::xml_document<> *put_parsed_xml_document_here, std::vector<char> &file_data, const std::string &file_path)
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

   uint get_next_number_string(const char *start_ptr, std::string *put_number_string_here)
   {
      uint characters_scanned = 0;

      put_number_string_here->clear();
      if (0 == *start_ptr)
      {
         // already at end of data set; don't read anything
         return 0;
      }

      // scan through white space 
      const char *c_ptr = start_ptr;
      char c = *c_ptr;
      while ('\n' == c || '\r' == c || '\t' == c || ' ' == c)
      {
         c_ptr += 1;
         c = *c_ptr;
         characters_scanned++;
      }

      // scan through non-white space characters and put them into the number string
      while (1)
      {
         if (0 == c || '\n' == c || '\r' == c || '\t' == c || ' ' == c)
         {
            // 0 means end of data set, and white space means end of number but the data set still
            // has values in it, but both cases indicate the end of this number
            return characters_scanned;
         }

         // not a white-space character, so record it
         (*put_number_string_here) += c;

         c_ptr += 1;
         c = *c_ptr;
         characters_scanned++;
      }
   }

   Shapes::Geometry *load_geometry(const rapidxml::xml_node<> *shape_node_ptr)
   {
      std::string shape_type_str = rapidxml::get_attrib_string(*shape_node_ptr, "value");

      if ("triangle" == shape_type_str)
      {
         cout << "loading triangle" << endl;
      }
      else if ("plane" == shape_type_str)
      {
         cout << "loading plane" << endl;
      }
      else if ("box" == shape_type_str)
      {
         cout << "loading box" << endl;
      }
      else if ("circle" == shape_type_str)
      {
         cout << "loading circle" << endl;
      }
      else if ("cube" == shape_type_str)
      {
         cout << "loading cube" << endl;
      }
      else if ("cylinder" == shape_type_str)
      {
         cout << "loading cylinder" << endl;
      }
      else if ("sphere" == shape_type_str)
      {
         cout << "loading sphere" << endl;
      }
      else
      {
         // not a valid shape
         cout << "'" << shape_type_str << "' is not recognized and cannot be generated" << endl;
         return 0;
      }

      return 0;
   }
}

namespace Scene
{
   Scene_Loader& Scene_Loader::get_instance()
   {
      static Scene_Loader instance;

      return instance;
   }

   bool Scene_Loader::load_scene()
   {
      Scene::Scene_Data local_scene;
      if (!local_scene.initialize())
      {
         return false;
      }

      std::string file_path = "C:/Users/John/Documents/GitHub/Simple_Game_Engine_2/scene_save_exp.xml";

      // open the file
      rapidxml::xml_document<> doc;
      std::vector<char> file_data;
      open_xml_file(&doc, file_data, file_path);

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
         // get the ID of the entity
         std::string new_entity_id_str = rapidxml::get_attrib_string(*entity_node_ptr, "id");
         cout << new_entity_id_str << endl;

         // create the entity
         Entities::Entity *new_entity_ptr = local_scene.new_entity(new_entity_id_str);

         // check for geometry
         const rapidxml::xml_node<> *shape_node_ptr = entity_node_ptr->first_node("shape");
         if (0 != shape_node_ptr)
         {
            Shapes::Geometry *new_geometry_ptr = load_geometry(shape_node_ptr);
               
            // make a renderable for this geometry

         }

         const rapidxml::xml_node<> *transform_node_ptr = entity_node_ptr->first_node("transform");
         if (0 == transform_node_ptr)
         {
            // bad!
            // TODO: do something about the bad
            cout << "no transform node" << endl;
         }
         else
         {
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

               Math::F_Dual_Quat dq(
                  Math::F_Quat(rw, glm::vec3(rx, ry, rz)),
                  Math::F_Quat(dw, glm::vec3(dx, dy, dz)));

               new_entity_ptr->m_where_and_which_way = dq;
            }


         }

         entity_node_ptr = entity_node_ptr->next_sibling("entity");
      }
      if (0 != entity_node_ptr)
      {

      }

      return true;
   }
}