#ifndef ARCSYNTHESIS_XML_READER_H
#define ARCSYNTHESIS_XML_READER_H

#include <string>
#include <vector>
#include <Utilities\Typedefs.h>
#include <Utilities\Include_Helpers\GL_Version.h>
#include <vector>

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
         // vertex attribute helper structure
         struct attrib_data_helper
         {
            uint attribute_layout_index;
            uint num_floats;
            uint floats_per_vector;
            uint num_vectors;
            float *float_data_ptr;
         };

         // index helper structure
         struct index_data_helper
         {
            GLenum render_mode;
            uint num_indices;
            GLushort *index_data_ptr;
         };

         // give a file path, open the file, parse it into the provided XML document 
         // class, and check that no errors happened
         static void open_xml_file(rapidxml::xml_document<> *put_parsed_xml_document_here, std::vector<char> &file_data, const std::string &file_path);


         // given a list of pointers to attribute nodes, parse them and fill out the 
         // Shape_Data structure's vertex data
         // Note: This function calls parse_attribute_node(...).
         static void load_all_attributes(Shape_Data *put_shape_data_here, const std::vector<const rapidxml::xml_node<> *> attrib_node_ptrs);

         // given a single attribute (position, color, normal, or texture coordinate), 
         // parse the data into an array of floats
         // Note: This is a helper function.  The calling function is responsible for
         // freeing the memory allocated by this one.
         static void parse_attribute_node(attrib_data_helper *put_attrib_data_here, const rapidxml::xml_node<> *node_ptr);

         // given a list of pointers to index nodes, parse them and fill out the 
         // Shape_Data structure's index data
         // Note: This function calls parse_index_node(...).
         static void load_all_index_data(Shape_Data * put_shape_data_here, const std::vector<const rapidxml::xml_node<> *> index_node_ptrs);

         // given a single index (draw command, list of indices that should be drawn with
         // this rendering mode), collect the data into an index data helper structure
         // Note: This is a helper function.  The calling function is responsible for 
         // freeing the memory allocated by this one.
         static void parse_index_node(index_data_helper *put_index_data_here, const rapidxml::xml_node<> *node_ptr);
         
         // returns the number of characters scanned
         // Note: This method only extracts a string.  It makes no asusmption about the
         // format of the number other than that the input argument string contains 
         // numbers separated by spaces.  It copies the substring that starts at the
         // starting pointer and ending with whitespace (or end of string) into the 
         // output argument, and then the method returns the number of characters 
         // scanned.
         // the next substring that ends in a space.
         // Example: Input argument string is "1.3492 56 299.3". 
         // Call this function once with the whole string will put the string "1.3492" 
         // into the output argument string and return 6 (6 characters were scanned).
         // The caller will presumably advance the start pointer by 6 characters and 
         // call this function again, and it will now copy "56" into the output 
         // argument and return 2.  Repeat again to get "299.3" and 5.
         static uint get_next_number_string(const char *start_ptr, std::string *put_number_string_here);
      };
   }
}

#endif