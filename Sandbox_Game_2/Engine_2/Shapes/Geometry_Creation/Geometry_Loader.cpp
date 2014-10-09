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
#include <Utilities\include_GL_version.h>

#include <Utilities\Typedefs.h>

namespace Shapes
{
   namespace Geometry_Creation
   {
      void Geometry_Loader::load_from_generator(GEOMETRY_LOADER_ENUMS load_type, Geometry &geo)
      {
         switch (load_type)
         {
         case Geometry_Loader::CUBE:
         {
            Shape_Generator::create_cube_data(&(geo.m_shape_data));
            geo.m_render_mode = GL_TRIANGLES;
            break;
         }
         case Geometry_Loader::PLANE:
         {
            Shape_Generator::create_plane_data(20, &(geo.m_shape_data));
            geo.m_render_mode = GL_TRIANGLES;
            break;
         }
         case Geometry_Loader::TRIANGLE_2D:
         {
            Shape_Generator::create_triangle(&(geo.m_shape_data));
            geo.m_render_mode = GL_TRIANGLES;
            break;
         }
         default:
         {
            // ??show error? throw exception??
            break;
         }
         }
         
         // generate vertex array object and bind it
         glGenVertexArrays(1, &geo.m_VAO_ID);
         glBindVertexArray(geo.m_VAO_ID);

         // vertex buffer
         // - generate one array buffer object and bind it
         // - allocate space for the position, normal, and color data with STATIC drawing (this data doesn't change on every draw), but send no data
         // - set vertex attribute pointers 
         // - send data with glBufferSubData(...)
         glGenBuffers(1, &geo.m_vertex_buffer_ID);
         glBindBuffer(GL_ARRAY_BUFFER, geo.m_vertex_buffer_ID);
         glBufferData(GL_ARRAY_BUFFER, geo.m_shape_data.vertex_buffer_size(), geo.m_shape_data.m_verts, GL_STATIC_DRAW);


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
         glGenBuffers(1, &geo.m_element_buffer_ID);
         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geo.m_element_buffer_ID);
         glBufferData(GL_ELEMENT_ARRAY_BUFFER, geo.m_shape_data.index_buffer_size(), geo.m_shape_data.m_indices, GL_STATIC_DRAW);


         // clean up bindings 
         glBindVertexArray(0);
         glBindBuffer(GL_ARRAY_BUFFER, 0);
         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


      }
   }
   
}