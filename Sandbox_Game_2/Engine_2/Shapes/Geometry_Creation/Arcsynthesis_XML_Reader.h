#ifndef ARCSYNTHESIS_XML_READER_H
#define ARCSYNTHESIS_XML_READER_H

#include <string>
#include <vector>
#include <Utilities\Typedefs.h>

// forward declarations
namespace Shapes
{
   struct Shape_Data;
}

namespace rapidxml
{
   // copied class declaration in rapidxml.hpp
   template<class Ch = char>
   class xml_node;
}

namespace Shapes
{
   namespace Geometry_Creation
   {
      class __declspec(dllexport) Arcsynthesis_XML_Reader
      {
      public:
         static void load_xml_file(Shape_Data *put_shape_data_here, const std::string &file_path);

      private:
         // helper structure
         struct attrib_data
         {
            uint attribute_layout_index;
            uint num_floats;
            uint floats_per_vector;
            uint num_vectors;
            float *float_data_ptr;
         };

         // helper methods are private
         static void parse_attribute_node(attrib_data *put_attrib_data_here, const rapidxml::xml_node<> *node_ptr);
         
         // returns the number of characters scanned
         static uint get_next_number_string(const char *start_ptr, std::string *put_number_string_here);
      };
   }
}

#endif