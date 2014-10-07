#include "Shape_Generator.h"

#include <Shapes\Shape_Data.h>

#include <glm\glm.hpp>
using glm::vec3;

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
      void Shape_Generator::create_cube_data(Shape_Data *put_data_here)
      {
         if (0 == put_data_here)
         {
            return;
         }

         vec3 local_verts[] = 
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

         // there are three vec3's per vertex
         uint num_verts = sizeof(local_verts) / (sizeof(*local_verts) * 3);

         put_data_here->m_num_verts = num_verts;
         put_data_here->m_position = (vec3 *)malloc(sizeof(vec3) * num_verts);   // ??check if malloc failed??
         put_data_here->m_normals = (vec3 *)malloc(sizeof(vec3) * num_verts);
         put_data_here->m_colors = (vec3 *)malloc(sizeof(vec3) * num_verts);

         // copy away!
         for (size_t vert_index = 0; vert_index < num_verts; vert_index++)
         {
            // the local vertex data is organized in sets of three for mental book keeping purposes,
            // but it needs to be split up in the shape data structure
            put_data_here->m_position[vert_index] = local_verts[(vert_index * 3) + 0];
            put_data_here->m_normals[vert_index] = local_verts[(vert_index * 3) + 1];
            put_data_here->m_colors[vert_index] = local_verts[(vert_index * 3) + 2];
         }

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
         put_data_here->m_num_indices = sizeof(local_indices) / sizeof(*local_indices);
         put_data_here->m_indices = (GLushort *)malloc(sizeof(local_indices));  // ??check if malloc failed??
      }
   }
}