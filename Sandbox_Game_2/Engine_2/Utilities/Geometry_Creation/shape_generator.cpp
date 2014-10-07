#include "Shape_Generator.h"

#include <Rendering\Geometry.h>

#include <vector>
using std::vector;

#include <glm\glm.hpp>
using glm::vec3;

// for GL typedefs
#include <Utilities\include_GL_version.h>

vec3 random_color()
{
   vec3 ret;
   ret.x = rand() / (float)(RAND_MAX);
   ret.y = rand() / (float)(RAND_MAX);
   ret.z = rand() / (float)(RAND_MAX);
   return ret;
}

namespace Utility
{
   namespace Geometry_Creation
   {
      void Shape_Generator::create_cube_data(std::vector<glm::vec3> &put_vertex_data_here, std::vector<GLushort> &put_index_data_here)
      {
         // Note: This data is an excerpt from Jamie King's my_shape_generator.cpp.

         vector<vec3> local_verts = 
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
         put_vertex_data_here = local_verts;


         vector<GLushort> local_indices =
         {
            0, 1, 2, 0, 2, 3,       // top
            4, 5, 6, 4, 6, 7,       // front
            8, 9, 10, 8, 10, 11,    // right
            12, 13, 14, 12, 14, 15, // left
            16, 17, 18, 16, 18, 19, // back
            20, 22, 21, 20, 23, 22, // bottom
         };

         // copy away!
         put_index_data_here = local_indices;
      }
   }
}