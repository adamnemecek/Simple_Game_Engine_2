#include "Geometry_Loader.h"

#include <Shapes\Geometry.h>
#include <Shapes\Geometry_Creation\Shape_Generator.h>

#include <glm\glm.hpp>
using glm::vec3;
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
         uint attrib_byte_count = sizeof(vec3) * geo.m_shape_data.m_num_verts;
         glGenBuffers(1, &geo.m_vertex_buffer_ID);
         glBindBuffer(GL_ARRAY_BUFFER, geo.m_vertex_buffer_ID);
         glBufferData(GL_ARRAY_BUFFER, attrib_byte_count * 3, 0, GL_STATIC_DRAW);


         // position = 0
         // normals = 1
         // colors = 2
         // full transform = 3, 4, 5, 6
         // orientation only = 7, 8, 9, 10
         GLuint vertex_array_index = 0;

         void *data_ptrs[] =
         {
            geo.m_shape_data.m_position,
            geo.m_shape_data.m_normals,
            geo.m_shape_data.m_colors
         };

         for (size_t count = 0; count < 3; count++)
         {
            glEnableVertexAttribArray(vertex_array_index);
            glVertexAttribPointer(vertex_array_index, attrib_byte_count, GL_FLOAT, GL_FALSE, 0, (void *)(attrib_byte_count * count));
            glBufferSubData(GL_ARRAY_BUFFER, attrib_byte_count * count, attrib_byte_count, (void *)(data_ptrs[count]));

            vertex_array_index++;
         }


         // matrix buffer
         // - generate one array buffer and bind it 
         // - allocate space for two mat4's (full transform and orientation only) with DYNAMIC drawing, but send no data (matrix data is stored in the Renderable, not in every single geometry)
         // - set vertex attribute pointers 
         // - set vertex attribute divisors to 1 for the two matrices
         glGenBuffers(1, &geo.m_matrix_buffer_ID);
         glBindBuffer(GL_ARRAY_BUFFER, geo.m_matrix_buffer_ID);
         glBufferData(GL_ARRAY_BUFFER, sizeof(mat4) * 2, 0, GL_DYNAMIC_DRAW);

         // full transform matrix
         for (size_t count = 0; count < 4; count++)
         {
            glEnableVertexAttribArray(vertex_array_index);
            glVertexAttribPointer(vertex_array_index, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void *)(sizeof(float) * 4 * count));
            glVertexAttribDivisor(vertex_array_index, 1);

            vertex_array_index++;
         }

         // orientation only matrix
         for (size_t count = 0; count < 4; count++)
         {
            glEnableVertexAttribArray(vertex_array_index);
            glVertexAttribPointer(vertex_array_index, 4, GL_FLOAT, GL_FALSE, sizeof(mat4), (void *)(sizeof(float) * 4 * count));
            glVertexAttribDivisor(vertex_array_index, 1);

            vertex_array_index++;
         }


         // indices buffer
         // - generate one element buffer object and bind it
         // - allocate space for the indices and send the data
         attrib_byte_count = sizeof(GLushort) * geo.m_shape_data.m_num_indices;
         glGenBuffers(1, &geo.m_element_buffer_ID);
         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geo.m_element_buffer_ID);
         glBufferData(GL_ELEMENT_ARRAY_BUFFER, attrib_byte_count, geo.m_shape_data.m_indices, GL_STATIC_DRAW);


         // clean up bindings 
         glBindVertexArray(0);
         glBindBuffer(GL_ARRAY_BUFFER, 0);
         glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
      }
   }
   
}