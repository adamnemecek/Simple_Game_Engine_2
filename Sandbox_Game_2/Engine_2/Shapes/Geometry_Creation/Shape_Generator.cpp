#include "Shape_Generator.h"

#include <Shapes\Shape_Data.h>
#include <Shapes\My_Vertex.h>
#include <Shapes\Index_Meta_Data.h>

#include <glm\vec3.hpp>
using glm::vec3;

#include <stdlib.h>
#include <Utilities\Include_Helpers\GL_Version.h>
#include <Utilities\Typedefs.h>
#include <math.h>


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
      // 2D shapes

      void Shape_Generator::generate_triangle(Shape_Data *put_data_here)
      {
         My_Vertex local_verts[] = 
         {
            vec3(-0.5f, -0.5f, -1.0f),          // left bottom corner
            vec3(+0.0f, +0.0f, +1.0f),             // normal points out of screen
            vec3(+1.0f, +0.0f, +0.0f),          // red

            vec3(+0.5f, -0.5f, -1.0f),          // right bottom corner
            vec3(+0.0f, +0.0f, +1.0f),          // normal points out of screen
            vec3(+0.0f, +1.0f, +0.0f),          // green

            vec3(+0.0f, +0.5f, -1.0f),          // center top
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
         put_data_here->m_num_total_indices = num_indices;
         put_data_here->m_indices = new GLushort[num_indices];
         memcpy(put_data_here->m_indices, local_indices, array_size_bytes);

         Index_Meta_Data i_meta_data(GL_TRIANGLES, num_indices);
         put_data_here->m_index_meta_data.push_back(i_meta_data);
      }


      void Shape_Generator::generate_plane(const uint number_segments_on_side, Shape_Data *put_data_here)
      {
         uint vert_count = number_segments_on_side * number_segments_on_side;
         put_data_here->m_num_verts = vert_count;
         put_data_here->m_verts = new My_Vertex[vert_count];

         int row_max_count = number_segments_on_side;
         float row_half_length = (float)number_segments_on_side / 2;
         int col_max_count = number_segments_on_side;
         float col_half_length = (float)number_segments_on_side / 2;

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
         put_data_here->m_num_total_indices = index_count;
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

         Index_Meta_Data i_meta_data(GL_TRIANGLES, index_count);
         put_data_here->m_index_meta_data.push_back(i_meta_data);
      }


      void Shape_Generator::generate_circle(const uint num_arc_segments, const float radius, Shape_Data *put_data_here)
      {
         // there are two vertices for each line representing an arc segment except for the 
         // last one, which only has one vertex that connects to the first one
         uint num_verts = num_arc_segments + 1;
         put_data_here->m_num_verts = num_verts;
         put_data_here->m_verts = new My_Vertex[num_verts];


         // pre-loop calculations
         float theta = 2 * 3.1415926f / float(num_arc_segments);
         float tangetial_factor = tanf(theta);
         float radial_factor = cosf(theta);

         float x = radius;//we start at angle = 0 
         float z = 0;

         for (int vertex_count = 0; vertex_count < num_arc_segments; vertex_count++)
         {
            My_Vertex& this_vert = put_data_here->m_verts[vertex_count];
            this_vert.position.x = x;
            this_vert.position.y = 0.0f;
            this_vert.position.z = z;

            this_vert.color = random_color();
            this_vert.normal = vec3(+0.0f, +1.0f, +0.0f);

            //calculate the tangential vector 
            //remember, the radial vector is (x, y) 
            //to get the tangential vector we flip those coordinates and negate one of them 

            float tx = (-z) * tangetial_factor;
            float tz = (x)* tangetial_factor;

            //add the tangential vector 
            x += tx;
            z += tz;

            //correct using the radial factor 
            x *= radial_factor;
            z *= radial_factor;
         }

         uint index_count = num_arc_segments * 2;
         put_data_here->m_num_total_indices = index_count;
         put_data_here->m_indices = new GLushort[index_count];
         int index_counter = 0;
         for (int segment_count = 0; segment_count < num_arc_segments; segment_count++)
         {
            if (segment_count == (num_arc_segments - 1))
            {
               // last vertex, then loop back to first one
               put_data_here->m_indices[index_counter++] = segment_count;
               put_data_here->m_indices[index_counter++] = 0;
            }
            else
            {
               // ex: segment 0 is vertices 0 and 1; segment 1 is vertices 1 and 2; segment 2 is vertices 2 and 3, etc.
               put_data_here->m_indices[index_counter++] = segment_count;
               put_data_here->m_indices[index_counter++] = segment_count + 1;
            }
         }

         Index_Meta_Data i_meta_data(GL_LINE_STRIP, index_count);
         put_data_here->m_index_meta_data.push_back(i_meta_data);
      }


      void Shape_Generator::generate_box(const float width, const float length, Shape_Data *put_data_here)
      {
         float half_width = width / 2.0f;
         float half_length = length / 2.0f;
         
         vec3 common_normal = vec3(0.0f, 1.0f, 0.0f);

         // generate vertices clockwise, starting with the upper right, when looking at an X-Z 
         // plane in which X is width and Z is length (or height depending on how you look at it)
         My_Vertex local_verts[] =
         {
            // upper right
            glm::vec3(+half_width, 0.0f, +half_length),
            common_normal,
            random_color(),
            
            // lower right
            glm::vec3(+half_width, 0.0f, -half_length),
            common_normal,
            random_color(),
            
            // lower left
            glm::vec3(-half_width, 0.0f, -half_length),
            common_normal,
            random_color(),
            
            // upper left
            glm::vec3(-half_width, 0.0f, +half_length),
            common_normal,
            random_color(),
         };

         // this is a rectangle, so go ahead and use magic numbers for the vertex count
         put_data_here->m_num_verts = 4;
         put_data_here->m_verts = new My_Vertex[4];
         memcpy(put_data_here->m_verts, local_verts, sizeof(local_verts));


         GLushort local_indices[] =
         {
            0, 1,
            1, 2,
            2, 3,
            3, 0
         };
         uint num_indices = sizeof(local_indices) / sizeof(GLushort);

         // this is a rectangle, so go ahead and use magic numbers for the index count
         put_data_here->m_num_total_indices = num_indices;
         put_data_here->m_indices = new GLushort[num_indices];
         memcpy(put_data_here->m_indices, local_indices, sizeof(local_indices));

         Index_Meta_Data i_meta_data(GL_LINE_STRIP, num_indices);
         put_data_here->m_index_meta_data.push_back(i_meta_data);
      }


      // 3D shapes
      void Shape_Generator::generate_cube(Shape_Data *put_data_here)
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
         put_data_here->m_num_total_indices = num_indices;
         put_data_here->m_indices = new GLushort[num_indices];
         memcpy(put_data_here->m_indices, local_indices, array_size_bytes);

         Index_Meta_Data i_meta_data(GL_TRIANGLES, num_indices);
         put_data_here->m_index_meta_data.push_back(i_meta_data);
      }

   }
}