#include "Arcsynthesis_XML_Reader.h"

#include <Shapes\Geometry.h>
#include <middleware\arcsynthesis\framework\rapidxml.hpp>
#include <middleware\arcsynthesis\framework\rapidxml_helpers.h>
#include <fstream>


#include <iostream>
using std::cout;
using std::endl;


namespace Shapes
{
   namespace Geometry_Creation
   {
      void Arcsynthesis_XML_Reader::load_xml_file(Geometry *put_geometry_here, const std::string &file_path)
      {
         // this function is heavily influenced by the arcsynthesis framework's Mesh 
         // class constructor, and some lines are copied verbatim except for the 
         // camel-case-to-underscore notaion change
         std::ifstream file_stream(file_path);
         if (!file_stream.is_open())
         {
            throw std::runtime_error("Could not find the mesh file: " + file_path);
         }

         std::vector<char> file_data;

         // reserve memory
         // Note: Reserving memory ensures contiguency of memory, which allows the 
         // rpaidxml parser to run through the data with just a pointer.
         file_data.reserve(2 * 1024 * 1024);
         file_data.insert(file_data.end(), std::istreambuf_iterator<char>(file_stream),
            std::istreambuf_iterator<char>());
         file_data.push_back('\0');

         rapidxml::xml_document<> doc;
         try
         {
            doc.parse<0>(&(file_data[0]));
         }
         catch (rapidxml::parse_error &e)
         {
            // this is a special rapidxml exception, so handle it here
            std::cout << file_path << ": Parse error in the mesh file." << std::endl;
            std::cout << e.what() << std::endl << e.where<char>() << std::endl;
            throw;
         }

         rapidxml::xml_node<> *root_node_ptr = doc.first_node("mesh");
         if (0 == root_node_ptr)
         {
            throw std::runtime_error("'mesh' node not found in mesh file: " + file_path);
         }

         // store the pointers to the attributes
         struct attribute_node_data
         {
            const rapidxml::xml_node<> *node_ptr;
            char *data;
            uint unparsed_size_bytes;
            std::string index;
            std::string type;
            std::string num_values_per;
         };
         std::vector<attribute_node_data> node_data_vector;
         node_data_vector.reserve(3);

         const rapidxml::xml_node<> *node_ptr = root_node_ptr->first_node("attribute");
         if (0 == node_ptr)
         {
            throw std::runtime_error("'mesh' node must have at least one 'attribute' child.  File: " + file_path);
         }
         for (;
            node_ptr && ("attribute" == rapidxml::make_string_name(*node_ptr));
            node_ptr = rapidxml::next_element(node_ptr))
         {
            attribute_node_data this_node_data;
            this_node_data.node_ptr = node_ptr;
            this_node_data.data = node_ptr->value();
            this_node_data.unparsed_size_bytes = node_ptr->value_size();

            this_node_data.index = rapidxml::get_attrib_string(*node_ptr, "index");
            this_node_data.type = rapidxml::get_attrib_string(*node_ptr, "type");
            this_node_data.num_values_per = rapidxml::get_attrib_string(*node_ptr, "size");

            node_data_vector.push_back(this_node_data);
            //attribute_nodes.push_back(node_ptr);
            //attribute_node_byte_size.push_back(node_ptr->va)
         }



         //char *pos_data_ptr = pos_node_ptr->value();
         //uint num_chars_in_pos_data = pos_node_ptr->value_size();

         //const rapidxml::xml_node<> *col_node_ptr = rapidxml::next_element(pos_node_ptr);


         cout << "hello" << endl;
         //const xml_node<> *pNode = pRootNode->first_node("attribute");
         //PARSE_THROW(pNode, ("`mesh` node must have at least one `attribute` child. File: " + strDataFilename));

         //for (;
         //   pNode && (make_string_name(*pNode) == "attribute");
         //   pNode = rapidxml::next_element(pNode))
         //{
         //   attribs.push_back(Attribute(*pNode));
         //   attribIndexMap[attribs.back().iAttribIx] = attribs.size() - 1;
         //}
      }

      void Arcsynthesis_XML_Reader::parse_node(const rapidxml::xml_node<> *node_ptr)
      {

      }
   }
}