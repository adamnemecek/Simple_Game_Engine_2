#include "Shape_Generator.h"

#include <Shapes\Shape_Data.h>
#include <Shapes\My_Vertex.h>

#include <glm\vec3.hpp>
using glm::vec3;

#include <stdlib.h>

// for GL typedefs
#include <Utilities\include_GL_version.h>

#include <Utilities\Typedefs.h>



vec3 random_color()
{
   vec3 ret;
   ret.x = rand() / (float)(RAND_MAX);
   ret.y = rand() / (float)(RAND_MAX);
   ret.z = rand() / (float)(RAND_MAX);
   return ret;
}


namespace Shapes
{
   namespace Geometry_Creation
   {
      void Shape_Generator::create_triangle(Shape_Data *put_data_here)
      {
         My_Vertex local_verts[] = 
         {
            vec3(-1.0f, -1.0f, -1.0f),          // left bottom corner
            vec3(+0.0f, +0.0f, +1.0f),             // normal points out of screen
            vec3(+1.0f, +0.0f, +0.0f),          // red

            vec3(+1.0f, -1.0f, -1.0f),          // right bottom corner
            vec3(+0.0f, +0.0f, +1.0f),          // normal points out of screen
            vec3(+0.0f, +1.0f, +0.0f),          // green

            vec3(+0.0f, +1.0f, -1.0f),          // center top
            vec3(+0.0f, +0.0f, +1.0f),          // normal points out of screen
            vec3(+0.0f, +0.0f, +1.0f),          // blue
         };

         // copy away!
         uint array_size_bytes = sizeof(local_verts);
         uint num_verts = array_size_bytes / sizeof(*local_verts);
         put_data_here->m_num_verts = num_verts;
         put_data_here->m_verts = new My_Vertex[num_verts];
         memcpy(put_data_here->m_verts, local_verts, array_size_bytes);


         GLushort local_indices[] =
         {
            0, 1, 2, 
         };

         // copy away!
         array_size_bytes = sizeof(local_indices);
         uint num_indices = array_size_bytes / sizeof(*local_indices);
         put_data_here->m_num_indices = num_indices;
         put_data_here->m_indices = new GLushort[num_indices];
         memcpy(put_data_here->m_indices, local_indices, array_size_bytes);

      }


      void Shape_Generator::create_cube_data(Shape_Data *put_data_here)
      {
         if (0 == put_data_here)
         {
            return;
         }

         My_Vertex local_verts[] = 
         {
            vec3(-1.0f, +1.0f, +1.0f),  // 0
            vec3(+0.0f, +1.0f, +0.0f),  // Normal
            random_color(),
            vec3(+1.0f, +1.0f, +1.0f),  // 1
            vec3(+0.0f, +1.0f, +0.0f),  // Normal
            random_color(),
            vec3(+1.0f, +1.0f, -1.0f),  // 2
            vec3(+0.0f, +1.0f, +0.0f),  // Normal
            random_color(),
            vec3(-1.0f, +1.0f, -1.0f),  // 3
            vec3(+0.0f, +1.0f, +0.0f),  // Normal
            random_color(),

            vec3(-1.0f, +1.0f, -1.0f),  // 4
            vec3(+0.0f, +0.0f, -1.0f),  // Normal
            random_color(),
            vec3(+1.0f, +1.0f, -1.0f),  // 5
            vec3(+0.0f, +0.0f, -1.0f),  // Normal
            random_color(),
            vec3(+1.0f, -1.0f, -1.0f),  // 6
            vec3(+0.0f, +0.0f, -1.0f),  // Normal
            random_color(),
            vec3(-1.0f, -1.0f, -1.0f),  // 7
            vec3(+0.0f, +0.0f, -1.0f),  // Normal
            random_color(),

            vec3(+1.0f, +1.0f, -1.0f),  // 8
            vec3(+1.0f, +0.0f, +0.0f),  // Normal
            random_color(),
            vec3(+1.0f, +1.0f, +1.0f),  // 9
            vec3(+1.0f, +0.0f, +0.0f),  // Normal
            random_color(),
            vec3(+1.0f, -1.0f, +1.0f),  // 10
            vec3(+1.0f, +0.0f, +0.0f),  // Normal
            random_color(),
            vec3(+1.0f, -1.0f, -1.0f),  // 11
            vec3(+1.0f, +0.0f, +0.0f),  // Normal
            random_color(),

            vec3(-1.0f, +1.0f, +1.0f),  // 12
            vec3(-1.0f, +0.0f, +0.0f),  // Normal
            random_color(),
            vec3(-1.0f, +1.0f, -1.0f),  // 13
            vec3(-1.0f, +0.0f, +0.0f),  // Normal
            random_color(),
            vec3(-1.0f, -1.0f, -1.0f),  // 14
            vec3(-1.0f, +0.0f, +0.0f),  // Normal
            random_color(),
            vec3(-1.0f, -1.0f, +1.0f),  // 15
            vec3(-1.0f, +0.0f, +0.0f),  // Normal
            random_color(),

            vec3(+1.0f, +1.0f, +1.0f),  // 16
            vec3(+0.0f, +0.0f, +1.0f),  // Normal
            random_color(),
            vec3(-1.0f, +1.0f, +1.0f),  // 17
            vec3(+0.0f, +0.0f, +1.0f),  // Normal
            random_color(),
            vec3(-1.0f, -1.0f, +1.0f),  // 18
            vec3(+0.0f, +0.0f, +1.0f),  // Normal
            random_color(),
            vec3(+1.0f, -1.0f, +1.0f),  // 19
            vec3(+0.0f, +0.0f, +1.0f),  // Normal
            random_color(),

            vec3(+1.0f, -1.0f, -1.0f),  // 20
            vec3(+0.0f, -1.0f, +0.0f),  // Normal
            random_color(),
            vec3(-1.0f, -1.0f, -1.0f),  // 21
            vec3(+0.0f, -1.0f, +0.0f),  // Normal
            random_color(),
            vec3(-1.0f, -1.0f, +1.0f),  // 22
            vec3(+0.0f, -1.0f, +0.0f),  // Normal
            random_color(),
            vec3(+1.0f, -1.0f, +1.0f),  // 23
            vec3(+0.0f, -1.0f, +0.0f),  // Normal
            random_color(),
         };

         // copy away!
         uint array_size_bytes = sizeof(local_verts);
         uint num_verts = array_size_bytes / sizeof(*local_verts);
         put_data_here->m_num_verts = num_verts;
         put_data_here->m_verts = new My_Vertex[num_verts];
         memcpy(put_data_here->m_verts, local_verts, array_size_bytes);


         GLushort local_indices[] =
         {
            0, 1, 2, 0, 2, 3,       // top
            4, 5, 6, 4, 6, 7,       // front
            8, 9, 10, 8, 10, 11,    // right
            12, 13, 14, 12, 14, 15, // left
            16, 17, 18, 16, 18, 19, // back
            20, 22, 21, 20, 23, 22, // bottom
         };

         // copy away!
         array_size_bytes = sizeof(local_indices);
         uint num_indices = array_size_bytes / sizeof(*local_indices);
         put_data_here->m_num_indices = num_indices;
         put_data_here->m_indices = new GLushort[num_indices];
         memcpy(put_data_here->m_indices, local_indices, array_size_bytes);
      }

      void Shape_Generator::create_plane_data(uint side_length, Shape_Data *put_data_here)
      {
         // make a plane that is centered on the origin

         uint vert_count = side_length * side_length;
         put_data_here->m_num_verts = vert_count;
         put_data_here->m_verts = new My_Vertex[vert_count];

         int row_max_count = side_length;
         float row_half_length = (float)side_length / 2;
         int col_max_count = side_length;
         float col_half_length = (float)side_length / 2;

         for (int row_count = 0; row_count < row_max_count; row_count++)
         {
            for (int col_count = 0; col_count < col_max_count; col_count++)
            {
               // start at upper left (-X, +Y) and work to lower right (+X, -Y)
               My_Vertex& this_vert = put_data_here->m_verts[row_count * row_max_count + col_count];
               this_vert.position.x = col_count - col_half_length;
               this_vert.position.y = 0.0f;
               this_vert.position.z = row_half_length - row_count;
               this_vert.color = random_color();
               this_vert.normal = vec3(+0.0f, +1.0f, +0.0f);
            }
         }

         // 6 indices to draw a square (every adjacent set of 4 vertices (including overlap))
         // Note: 3x3 is a 2x2 set of 4 vertices, 4x4 is a 3x3 set of 4, etc.
         uint index_count = (row_max_count - 1) * (col_max_count - 1) * 6;
         put_data_here->m_num_indices = index_count;
         put_data_here->m_indices = new GLushort[index_count];
         int index_counter = 0;
         for (int row_count = 0; row_count < (row_max_count - 1); row_count++)
         {
            for (int col_count = 0; col_count < (col_max_count - 1); col_count++)
            {
               put_data_here->m_indices[index_counter++] = row_count * row_max_count + col_count;
               put_data_here->m_indices[index_counter++] = row_count * row_max_count + (col_count + 1);
               put_data_here->m_indices[index_counter++] = (row_count + 1) * row_max_count + (col_count + 1);
               
               put_data_here->m_indices[index_counter++] = row_count * row_max_count + col_count;
               put_data_here->m_indices[index_counter++] = (row_count + 1) * row_max_count + (col_count + 1);
               put_data_here->m_indices[index_counter++] = (row_count + 1) * row_max_count + col_count;
            }
         }
      }
   }
}