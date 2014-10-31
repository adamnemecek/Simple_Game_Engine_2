#include "Geometry_Loader.h"

#include <Shapes\Geometry.h>
#include <Shapes\Geometry_Creation\Shape_Generator.h>

#include <glm\vec3.hpp>
using glm::vec3;

#include <glm\mat4x4.hpp>
using glm::mat4;

// to make buffer data passing easier
#include <glm\gtc\type_ptr.hpp>
using glm::value_ptr;

// for GL typedefs
#include <Utilities\Include_Helpers\GL_Version.h>

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

      void Geometry_Loader::load_cube(Geometry *put_geometry_here)
      {
         Shape_Generator::generate_cube(&(put_geometry_here->m_shape_data));
         put_geometry_here->m_render_mode = GL_TRIANGLES;

         initialize_attributes(put_geometry_here);
      }

      void Geometry_Loader::load_plane(Geometry *put_geometry_here, const uint num_unit_segments_on_a_side)
      {
         Shape_Generator::generate_plane(num_unit_segments_on_a_side, &(put_geometry_here->m_shape_data));
         put_geometry_here->m_render_mode = GL_TRIANGLES;

         initialize_attributes(put_geometry_here);
      }

      void Geometry_Loader::load_triangle(Geometry *put_geometry_here)
      {
         Shape_Generator::generate_triangle(&(put_geometry_here->m_shape_data));
         put_geometry_here->m_render_mode = GL_TRIANGLES;

         initialize_attributes(put_geometry_here);
      }

      void Geometry_Loader::load_circle(Geometry *put_geometry_here, const uint num_arc_segments, const float radius)
      {
         Shape_Generator::generate_circle(num_arc_segments, radius, &(put_geometry_here->m_shape_data));
         put_geometry_here->m_render_mode = GL_LINE_STRIP;

         initialize_attributes(put_geometry_here);

      }

      void Geometry_Loader::load_box(Geometry *put_geometry_here, const float width, const float length)
      {
         Shape_Generator::generate_box(width, length, &(put_geometry_here->m_shape_data));
         put_geometry_here->m_render_mode = GL_LINE_STRIP;

         initialize_attributes(put_geometry_here);

      }




      //void Geometry_Loader::load_from_generator(GEOMETRY_PRIMITIVE_ENUMS load_type, Geometry *geo)
      //{
      //   //switch (load_type)
      //   //{
      //   //case Geometry_Loader::CUBE:
      //   //{
      //   //   Shape_Generator::create_cube_data(&(geo->m_shape_data));
      //   //   geo->m_render_mode = GL_TRIANGLES;
      //   //   break;
      //   //}
      //   //case Geometry_Loader::PLANE:
      //   //{
      //   //   Shape_Generator::create_plane(20, &(geo->m_shape_data));
      //   //   geo->m_render_mode = GL_TRIANGLES;
      //   //   break;
      //   //}
      //   //case Geometry_Loader::TRIANGLE:
      //   //{
      //   //   Shape_Generator::create_triangle(&(geo->m_shape_data));
      //   //   geo->m_render_mode = GL_TRIANGLES;
      //   //   break;
      //   //}
      //   //case Geometry_Loader::CIRCLE:
      //   //{
      //   //   Shape_Generator::create_circle(glm::vec3(), 16.0f, 2.2f, &(geo->m_shape_data));
      //   //   geo->m_render_mode = GL_LINE_STRIP;
      //   //   break;
      //   //}
      //   //case Geometry_Loader::RECTANGLE:
      //   //{
      //   //   Shape_Generator::create_rectangle(glm::vec3(), );
      //   //   geo->m_render_mode = GL_LINE_STRIP;
      //   //   break;
      //   //}
      //   //default:
      //   //{
      //   //   // ??show error? throw exception??
      //   //   break;
      //   //}
      //   //}
      //   //
      //   //// generate vertex array object and bind it
      //   //glGenVertexArrays(1, &geo->m_VAO_ID);
      //   //glBindVertexArray(geo->m_VAO_ID);

      //   //// vertex buffer
      //   //// - generate one array buffer object and bind it
      //   //// - allocate space for the position, normal, and color data with STATIC drawing (this data doesn't change on every draw), but send no data
      //   //// - set vertex attribute pointers 
      //   //// - send data with glBufferSubData(...)
      //   //glGenBuffers(1, &geo->m_vertex_buffer_ID);
      //   //glBindBuffer(GL_ARRAY_BUFFER, geo->m_vertex_buffer_ID);
      //   //glBufferData(GL_ARRAY_BUFFER, geo->m_shape_data.vertex_buffer_size(), geo->m_shape_data.m_verts, GL_STATIC_DRAW);


      //   //// position = 0
      //   //// normals = 1
      //   //// colors = 2
      //   //
      //   //// some helper variables
      //   //uint vertex_array_index = 0;
      //   //uint buffer_start_offset = 0;

      //   //// position
      //   //glEnableVertexAttribArray(vertex_array_index);
      //   //glVertexAttribPointer(vertex_array_index, My_Vertex::FLOATS_PER_POSITION, GL_FLOAT, GL_FALSE, My_Vertex::BYTES_PER_VERTEX, (void *)buffer_start_offset);
      //   //buffer_start_offset += My_Vertex::BYTES_PER_POSITION;
      //   //vertex_array_index++;

      //   //// normal
      //   //glEnableVertexAttribArray(vertex_array_index);
      //   //glVertexAttribPointer(vertex_array_index, My_Vertex::FLOATS_PER_NORMAL, GL_FLOAT, GL_FALSE, My_Vertex::BYTES_PER_VERTEX, (void *)buffer_start_offset);
      //   //buffer_start_offset += My_Vertex::BYTES_PER_NORMAL;
      //   //vertex_array_index++;

      //   //// color
      //   //glEnableVertexAttribArray(vertex_array_index);
      //   //glVertexAttribPointer(vertex_array_index, My_Vertex::FLOATS_PER_COLOR, GL_FLOAT, GL_FALSE, My_Vertex::BYTES_PER_VERTEX, (void *)buffer_start_offset);
      //   //buffer_start_offset += My_Vertex::BYTES_PER_COLOR;
      //   //vertex_array_index++;


      //   //// indices buffer
      //   //// - generate one element buffer object and bind it
      //   //// - allocate space for the indices and send the data
      //   //glGenBuffers(1, &geo->m_element_buffer_ID);
      //   //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geo->m_element_buffer_ID);
      //   //glBufferData(GL_ELEMENT_ARRAY_BUFFER, geo->m_shape_data.index_buffer_size(), geo->m_shape_data.m_indices, GL_STATIC_DRAW);


      //   //// clean up bindings 
      //   //glBindVertexArray(0);
      //   //glBindBuffer(GL_ARRAY_BUFFER, 0);
      //   //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
      //}
   }
   
}