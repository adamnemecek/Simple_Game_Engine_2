#include "Geometry_Loader.h"

#include <Shapes\Geometry.h>
#include <Shapes\Geometry_Creation\Shape_Generator.h>
#include <Shapes\Geometry_Creation\Arcsynthesis_XML_Reader.h>
#include <Shapes\Shape_Data.h>
#include <Shapes\Geometry_Meta_Data.h>

#include <glm\vec3.hpp>
#include <glm\gtc\type_ptr.hpp> // to make buffer data passing easier
#include <Utilities\Include_Helpers\GL_Version.h> // for GL typedefs
#include <Utilities\Typedefs.h>

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
            for (size_t vertex_counter = 0; vertex_counter < shape_data_ref.m_num_verts; vertex_counter++)
            {
               glm::vec3 &this_pos = shape_data_ref.m_verts[vertex_counter].position;

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
         //put_geometry_here->m_render_mode = GL_TRIANGLES;

         initialize_attributes(put_geometry_here);
         calculate_geometry_meta_data(put_geometry_here);
      }

      void Geometry_Loader::load_plane(Geometry *put_geometry_here, const uint num_unit_segments_on_a_side)
      {
         Shape_Generator::generate_plane(num_unit_segments_on_a_side, &(put_geometry_here->m_shape_data));
         //put_geometry_here->m_render_mode = GL_TRIANGLES;

         initialize_attributes(put_geometry_here);
         calculate_geometry_meta_data(put_geometry_here);
      }

      void Geometry_Loader::load_triangle(Geometry *put_geometry_here)
      {
         Shape_Generator::generate_triangle(&(put_geometry_here->m_shape_data));
         //put_geometry_here->m_render_mode = GL_TRIANGLES;

         initialize_attributes(put_geometry_here);
         calculate_geometry_meta_data(put_geometry_here);
      }

      void Geometry_Loader::load_circle(Geometry *put_geometry_here, const uint num_arc_segments, const float radius)
      {
         Shape_Generator::generate_circle(num_arc_segments, radius, &(put_geometry_here->m_shape_data));
         //put_geometry_here->m_render_mode = GL_LINE_STRIP;

         initialize_attributes(put_geometry_here);
         calculate_geometry_meta_data(put_geometry_here);
      }

      void Geometry_Loader::load_box(Geometry *put_geometry_here, const float width, const float length)
      {
         Shape_Generator::generate_box(width, length, &(put_geometry_here->m_shape_data));
         //put_geometry_here->m_render_mode = GL_LINE_STRIP;

         initialize_attributes(put_geometry_here);
         calculate_geometry_meta_data(put_geometry_here);
      }

      void Geometry_Loader::load_cylinder(Geometry *put_geometry_here)
      {
         //Shape_Generator::generate_arcysynthesis_cylinder(&put_geometry_here->m_shape_data);
         Shape_Generator::generate_cylinder(6, 1.5f, 3, 3, &(put_geometry_here->m_shape_data));

         initialize_attributes(put_geometry_here);
         calculate_geometry_meta_data(put_geometry_here);
      }

      void Geometry_Loader::load_arcsynthesis_xml_file(Geometry *put_geometry_here, const std::string &file_path)
      {
         Arcsynthesis_XML_Reader::load_from_xml_file(&(put_geometry_here->m_shape_data), file_path);
         initialize_attributes(put_geometry_here);
         calculate_geometry_meta_data(put_geometry_here);
      }
   }
}