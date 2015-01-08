#ifndef ARCSYNTHESIS_XML_READER_H
#define ARCSYNTHESIS_XML_READER_H

#include <string>
#include <vector>

// forward declarations
namespace Shapes
{
   class Geometry;
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
         static void load_xml_file(Geometry *put_geometry_here, const std::string &file_path);

      private:
         // helper methods are private
         static void parse_node(const rapidxml::xml_node<> *node_ptr);
      };
   }
}

#endif