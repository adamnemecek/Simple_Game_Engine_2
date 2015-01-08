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
   // these are copied class declarations from rapidxml.hpp

   template<class Ch = char>
   class xml_node;

   template<class Ch = char>
   class xml_document;
}

namespace Shapes
{
   namespace Geometry_Creation
   {
      class __declspec(dllexport) Arcsynthesis_XML_Reader
      {
      public:
         // given a path to an Arcsynthesis file, open it, parse the data inside, and
         // fill out a Shape_Data structure
         // Note: This method calls parse_attribute_node(...) and is responsible for 
         // freeing the memory that was malloc'd by that function for its attribute 
         // data arrays.
         static void load_from_xml_file(Shape_Data *put_shape_data_here, const std::string &file_path);

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

         // give a file path, open the file, parse it into the provided XML document 
         // class, and check that no errors happened
         static void open_xml_file(rapidxml::xml_document<> *put_parsed_xml_document_here, std::vector<char> &file_data, const std::string &file_path);

         // given a single attribute (position, color, normal, or texture coordinate), 
         // parse the data into an array of floats
         // Note: This is a helper function to load_xml_file(...), which is responsible 
         // for freeing the memory allocated by this function.
         static void parse_attribute_node(attrib_data *put_attrib_data_here, const rapidxml::xml_node<> *node_ptr);
         
         // returns the number of characters scanned
         static uint get_next_number_string(const char *start_ptr, std::string *put_number_string_here);
      };
   }
}

#endif