#include "Scene_Loader.h"

#include <Scene\Scene_Data.h>

#include <Utilities\Typedefs.h>
#include <middleware\arcsynthesis\framework\rapidxml.hpp>
#include <middleware\arcsynthesis\framework\rapidxml_helpers.h>
#include <fstream>
#include <vector>

// for generating things from the file data
#include <Entities\Entity.h>
#include <Math\F_Quat.h>
#include <Math\F_Dual_Quat.h>

#include <iostream>


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

      uint num_entities = 0;
      uint num_lights = 0;

      const rapidxml::xml_node<> *entity_node_ptr = root_node_ptr->first_node("entity");
      if (0 != entity_node_ptr)
      {
         std::string new_entity_id_str = rapidxml::get_attrib_string(*entity_node_ptr, "id");
         Entities::Entity *new_entity_ptr = local_scene.new_entity(new_entity_id_str);

         const rapidxml::xml_node<> *shape_node_ptr = entity_node_ptr->first_node("shape");
         if (0 != shape_node_ptr)
         {
            std::string shape_type = rapidxml::get_attrib_string(*shape_node_ptr, "value");
         }

         const rapidxml::xml_node<> *transform_node_ptr = entity_node_ptr->first_node("transform");
         if (0 != transform_node_ptr)
         {
            if ("dual_quaternion" == rapidxml::get_attrib_string(*transform_node_ptr, "type"))
            {
               // "r" == "real", "d" == "dual"
               float f = 0.0f;
               float rw = rapidxml::get_attrib_float(*transform_node_ptr, "rw", f);
            }
            //std::string transform_str = rapidxml::get_attrib_string(*transform_node_ptr, "value");
            //std::string num_string = "";
            //uint chars_scanned = 0;

            //// there should be 8 characters in the dual quat transform

            //chars_scanned += get_next_number_string(transform_str.c_str(), &num_string);
            //float real_w = atof(num_string.c_str());
            //num_string.clear();
            //chars_scanned += get_next_number_string(transform_str.c_str() + chars_scanned, &num_string);
            //float real_x = atof(num_string.c_str());
            //num_string.clear();
            //chars_scanned += get_next_number_string(transform_str.c_str() + chars_scanned, &num_string);
            //float real_y = atof(num_string.c_str());
            //num_string.clear();
            //chars_scanned += get_next_number_string(transform_str.c_str() + chars_scanned, &num_string);
            //float real_z = atof(num_string.c_str());

            //chars_scanned += get_next_number_string(transform_str.c_str(), &num_string);
            //float dual_w = atof(num_string.c_str());
            //num_string.clear();
            //chars_scanned += get_next_number_string(transform_str.c_str() + chars_scanned, &num_string);
            //float dual_x = atof(num_string.c_str());
            //num_string.clear();
            //chars_scanned += get_next_number_string(transform_str.c_str() + chars_scanned, &num_string);
            //float dual_y = atof(num_string.c_str());
            //num_string.clear();
            //chars_scanned += get_next_number_string(transform_str.c_str() + chars_scanned, &num_string);
            //float dual_z = atof(num_string.c_str());

            //Math::F_Dual_Quat dq(
            //   Math::F_Quat(real_w, glm::vec3(real_x, real_y, real_z)),
            //   Math::F_Quat(dual_w, glm::vec3(dual_x, dual_y, dual_z)));


         }
      }

      return true;
   }
}