#include "Geometry_Loader.h"

#include <Shapes\Geometry.h>
#include <Shapes\Geometry_Creation\Shape_Generator.h>
#include <Shapes\Shape_Data.h>
#include <Shapes\Geometry_Meta_Data.h>

#include <glm\vec3.hpp>
#include <glm\gtc\type_ptr.hpp> // to make buffer data passing easier
#include <Utilities\Include_Helpers\GL_Version.h> // for GL typedefs
#include <Utilities\Typedefs.h>

//#include <Utilities\rapidxml.hpp>   // courtesy of Arcsynthesis
#include <middleware\arcsynthesis\framework\rapidxml.hpp>
#include <middleware\arcsynthesis\framework\rapidxml_helpers.h>
#include <fstream>
#include <vector>
#include <iostream>
using std::cout;
using std::endl;

namespace Shapes
{
   namespace Geometry_Creation
   {
      void Geometry_Loader::initialize_attributes(Geometry *geo)
      {
         // generate vertex array object and bind it
         glGenVertexArrays(1, &geo->m_VAO_ID);
         glBindVertexArray(geo->m_VAO_ID);

         // vertex buffer
         // - generate one array buffer object and bind it
         // - allocate space for the position, normal, and color data with STATIC drawing (this data doesn't change on every draw), but send no data
         // - set vertex attribute pointers 
         // - send data with glBufferSubData(...)
         glGenBuffers(1, &geo->m_vertex_buffer_ID);
         glBindBuffer(GL_ARRAY_BUFFER, geo->m_vertex_buffer_ID);
         glBufferData(GL_ARRAY_BUFFER, geo->m_shape_data.vertex_buffer_size(), geo->m_shape_data.m_verts, GL_STATIC_DRAW);


         // position = 0
         // normals = 1
         // colors = 2

         // some helper variables
         uint vertex_array_index = 0;
         uint buffer_start_offset = 0;

         // position
         glEnableVertexAttribArray(vertex_array_index);
         glVertexAttribPointer(vertex_array_index, My_Vertex::FLOATS_PER_POSITION, GL_FLOAT, GL_FALSE, My_Vertex::BYTES_PER_VERTEX, (void *)buffer_start_offset);
         buffer_start_offset += My_Vertex::BYTES_PER_POSITION;
         vertex_array_index++;

         // normal
         glEnableVertexAttribArray(vertex_array_index);
         glVertexAttribPointer(vertex_array_index, My_Vertex::FLOATS_PER_NORMAL, GL_FLOAT, GL_FALSE, My_Vertex::BYTES_PER_VERTEX, (void *)buffer_start_offset);
         buffer_start_offset += My_Vertex::BYTES_PER_NORMAL;
         vertex_array_index++;

         // color
         glEnableVertexAttribArray(vertex_array_index);
         glVertexAttribPointer(vertex_array_index, My_Vertex::FLOATS_PER_COLOR, GL_FLOAT, GL_FALSE, My_Vertex::BYTES_PER_VERTEX, (void *)buffer_start_offset);
         buffer_start_offset += My_Vertex::BYTES_PER_COLOR;
         vertex_array_index++;


         // indices buffer
         // - generate one element buffer object and bind it
         // - allocate space for the indices and send the data
         glGenBuffers(1, &geo->m_element_buffer_ID);
         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geo->m_element_buffer_ID);
         glBufferData(GL_ELEMENT_ARRAY_BUFFER, geo->m_shape_data.index_buffer_size(), geo->m_shape_data.m_indices, GL_STATIC_DRAW);


         // clean up bindings 
         glBindVertexArray(0);
         glBindBuffer(GL_ARRAY_BUFFER, 0);
         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
      }

      void Geometry_Loader::calculate_geometry_meta_data(Geometry *geo)
      {
            double sum_x = 0.0;
            double sum_y = 0.0;
            double sum_z = 0.0;

            // declarations to make dereferencing easier
            Geometry_Meta_Data &meta_data_ref = geo->m_meta_data;
            Shape_Data &shape_data_ref = geo->m_shape_data;

            // go through the geometry data and analyze it
            for (size_t index_counter = 0; index_counter < shape_data_ref.m_num_verts; index_counter++)
            {
               glm::vec3 &this_pos = shape_data_ref.m_verts[index_counter].position;

               sum_x += this_pos.x;
               sum_y += this_pos.y;
               sum_z += this_pos.z;

               if (this_pos.x < meta_data_ref.m_min_X) { meta_data_ref.m_min_X = this_pos.x; }
               else if (this_pos.x > meta_data_ref.m_max_X) { meta_data_ref.m_max_X = this_pos.x; }

               if (this_pos.y < meta_data_ref.m_min_Y) { meta_data_ref.m_min_Y = this_pos.y; }
               else if (this_pos.y > meta_data_ref.m_max_Y) { meta_data_ref.m_max_Y = this_pos.y; }

               if (this_pos.z < meta_data_ref.m_min_Z) { meta_data_ref.m_min_Z = this_pos.z; }
               else if (this_pos.z > meta_data_ref.m_max_Z) { meta_data_ref.m_max_Z = this_pos.z; }
            }

            // do this division so that you keep the precision of the double
            double inverse_num_verts = 1.0 / shape_data_ref.m_num_verts;

            // perform the double multiplication version of division in order to keep precision
            // of the double, THEN jam the result into the vec3's float
            // Note: This approach should result in minimal precision loss because the float 
            // cast happens after the high-precision sum and division operations.
            meta_data_ref.m_center_of_geometry.x = sum_x * inverse_num_verts;
            meta_data_ref.m_center_of_geometry.y = sum_y * inverse_num_verts;
            meta_data_ref.m_center_of_geometry.z = sum_z * inverse_num_verts;
      }

      void Geometry_Loader::load_cube(Geometry *put_geometry_here)
      {
         Shape_Generator::generate_cube(&(put_geometry_here->m_shape_data));
         put_geometry_here->m_render_mode = GL_TRIANGLES;

         initialize_attributes(put_geometry_here);
         calculate_geometry_meta_data(put_geometry_here);
      }

      void Geometry_Loader::load_plane(Geometry *put_geometry_here, const uint num_unit_segments_on_a_side)
      {
         Shape_Generator::generate_plane(num_unit_segments_on_a_side, &(put_geometry_here->m_shape_data));
         put_geometry_here->m_render_mode = GL_TRIANGLES;

         initialize_attributes(put_geometry_here);
         calculate_geometry_meta_data(put_geometry_here);
      }

      void Geometry_Loader::load_triangle(Geometry *put_geometry_here)
      {
         Shape_Generator::generate_triangle(&(put_geometry_here->m_shape_data));
         put_geometry_here->m_render_mode = GL_TRIANGLES;

         initialize_attributes(put_geometry_here);
         calculate_geometry_meta_data(put_geometry_here);
      }

      void Geometry_Loader::load_circle(Geometry *put_geometry_here, const uint num_arc_segments, const float radius)
      {
         Shape_Generator::generate_circle(num_arc_segments, radius, &(put_geometry_here->m_shape_data));
         put_geometry_here->m_render_mode = GL_LINE_STRIP;

         initialize_attributes(put_geometry_here);
         calculate_geometry_meta_data(put_geometry_here);
      }

      void Geometry_Loader::load_box(Geometry *put_geometry_here, const float width, const float length)
      {
         Shape_Generator::generate_box(width, length, &(put_geometry_here->m_shape_data));
         put_geometry_here->m_render_mode = GL_LINE_STRIP;

         initialize_attributes(put_geometry_here);
         calculate_geometry_meta_data(put_geometry_here);
      }

      void Geometry_Loader::load_arcsynthesis_xml_file(Geometry *put_geometry_here, const std::string &file_path)
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
   }
}