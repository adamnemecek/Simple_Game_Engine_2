#include "Shape_Generator.h"

#include <Shapes\Shape_Data.h>
#include <Shapes\My_Vertex.h>
#include <Shapes\Index_Meta_Data.h>

#include <glm\vec3.hpp>
#include <glm\detail\func_geometric.hpp>

#include <stdlib.h>
#include <Utilities\Include_Helpers\GL_Version.h>
#include <Utilities\Typedefs.h>
#include <math.h>


// helper functions for this file
namespace
{
   glm::vec3 random_color()
   {
      glm::vec3 ret;
      ret.x = rand() / (float)(RAND_MAX);
      ret.y = rand() / (float)(RAND_MAX);
      ret.z = rand() / (float)(RAND_MAX);
      return ret;
   }

   // generate vertices that make up a circle, allocate memory, and return a pointer
   // Note: It is the calling function's responsibility to free this memory.
   // Note: Algorithm courtesy of http://slabode.exofire.net/circle_draw.shtml .
   Shapes::My_Vertex* helper_generate_horizontal_circle(const uint num_arc_segments, const float radius, const float Y_height)
   {
      // there is one vertex for each line segment
      // Note: The calling function is responsible for assigning indices.
      uint num_verts = num_arc_segments;
      Shapes::My_Vertex *ret_ptr = new Shapes::My_Vertex[num_verts];

      // pre-loop calculations
      float theta = 2 * 3.1415926f / float(num_arc_segments);
      float tangetial_factor = tanf(theta);
      float radial_factor = cosf(theta);

      float x = radius;//we start at angle = 0 
      float z = 0;

      for (uint segment_count = 0; segment_count < num_arc_segments; segment_count++)
      {
         Shapes::My_Vertex& this_vert = ret_ptr[segment_count];
         this_vert.position = glm::vec3(x, Y_height, z);
         //this_vert.position = glm::vec3(x, z, Y_height);
         this_vert.color = random_color();

         // the normals, by default, point outward radially like spikes on a wheel
         this_vert.normal = glm::normalize(glm::vec3(x, 0.0f, z));

         // calculate the next vertex
         //calculate the tangential vector 
         //remember, the radial vector is (x, y) 
         //to get the tangential vector we flip those coordinates and negate one of them 

         float tx = (-z) * tangetial_factor;
         float tz = (x) * tangetial_factor;

         //add the tangential vector 
         x += tx;
         z += tz;

         //correct using the radial factor 
         x *= radial_factor;
         z *= radial_factor;
      }

      return ret_ptr;
   }
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
            glm::vec3(-0.5f, -0.5f, -1.0f),          // left bottom corner
            glm::vec3(+0.0f, +0.0f, +1.0f),             // normal points out of screen
            glm::vec3(+1.0f, +0.0f, +0.0f),          // red

            glm::vec3(+0.5f, -0.5f, -1.0f),          // right bottom corner
            glm::vec3(+0.0f, +0.0f, +1.0f),          // normal points out of screen
            glm::vec3(+0.0f, +1.0f, +0.0f),          // green

            glm::vec3(+0.0f, +0.5f, -1.0f),          // center top
            glm::vec3(+0.0f, +0.0f, +1.0f),          // normal points out of screen
            glm::vec3(+0.0f, +0.0f, +1.0f),          // blue
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
               this_vert.normal = glm::vec3(+0.0f, +1.0f, +0.0f);
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
         // there is one vertex for each line segment
         // Note: These vertices are calculated for a Line Strip, and the indices will
         // handle the closing of the loop.
         uint num_verts = num_arc_segments;
         put_data_here->m_num_verts = num_verts;
         put_data_here->m_verts = new My_Vertex[num_verts];

         // remember that one vector is generated for each arc segment
         My_Vertex *circle_verts_ptr = helper_generate_horizontal_circle(num_arc_segments, radius, 0.0f);
         for (uint arc_segment_count = 0; arc_segment_count < num_arc_segments; arc_segment_count++)
         {
            // copy the data into the memory that's leaving this function
            put_data_here->m_verts[arc_segment_count] = circle_verts_ptr[arc_segment_count];
         }

         // free the temporary memory
         free(circle_verts_ptr);

         uint index_count = num_arc_segments * 2;
         put_data_here->m_num_total_indices = index_count;
         put_data_here->m_indices = new GLushort[index_count];
         int index_counter = 0;
         for (uint segment_count = 0; segment_count < num_arc_segments; segment_count++)
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

      void Shape_Generator::generate_cylinder(const uint num_arc_segments, const float radius, const uint num_vertical_segments, const float height, Shape_Data *put_data_here)
      {
         // allocate space for the total number of vertices required
         // Note: The top and bottom of the cylinder will use the same calculation.  
         // The top and bottom vertices will be calculated as a Triangle Fan.
         // The top and bottom will have 1 vertex for each arc segment + 1 for the center.  The indices will close the loops.
         // The cylinder body will be calculated as a Triangle Strip.
         // The cylinder body will be composed of a bunch of circles, so 1 vertex for each circle segment.  The indices will close the loops.
         // The cylinder body will have 1 circle for each vertical segment, then 1 more.  Two are needed for 1 vertical segment, 3 for 2, etc.
         // Note: If 0 vertical arc segments or or 0 vertical segments are specified, then the cylinder will simply not generate any vertices.
         uint num_verts_on_one_cap = num_arc_segments + 1;
         uint num_verts_on_body = (num_vertical_segments + 1) * num_arc_segments;
         uint num_verts = (num_verts_on_one_cap * 2) + num_verts_on_body;
         put_data_here->m_num_verts = num_verts;
         put_data_here->m_verts = new My_Vertex[num_verts];


         // cylinder caps
         {
            // top

            // center vertex
            My_Vertex& this_vert = put_data_here->m_verts[0];
            this_vert.position = glm::vec3(0.0f, height, 0.0f);
            this_vert.color = random_color();
            this_vert.normal = glm::vec3(0.0f, +1.0f, 0.0f);

            // the outer vertices
            // Note: Remember that one vector is generated for each arc segment.
            My_Vertex *circle_verts_ptr = helper_generate_horizontal_circle(num_arc_segments, radius, height);
            for (uint arc_segment_count = 0; arc_segment_count < num_arc_segments; arc_segment_count++)
            {
               // the normals on the top point up, so override the defaults
               circle_verts_ptr[arc_segment_count].normal = glm::vec3(0.0f, 1.0f, 0.0f);

               // copy the modified vertex into the memory that's leaving this function
               // Note: Add 1 because the center vertex has already been added
               put_data_here->m_verts[1 + arc_segment_count] = circle_verts_ptr[arc_segment_count];
            }

            // free the temporary memory
            free(circle_verts_ptr);
         }
         {
            // bottom

            // center vertex
            My_Vertex& this_vert = put_data_here->m_verts[num_verts_on_one_cap];
            this_vert.position = glm::vec3(0.0f, 0.0f, 0.0f);
            this_vert.color = random_color();
            this_vert.normal = glm::vec3(0.0f, -1.0f, 0.0f);

            // the outer vertices
            // Note: Remember that one vector is generated for each arc segment.
            My_Vertex *circle_verts_ptr = helper_generate_horizontal_circle(num_arc_segments, radius, 0.0f);
            for (uint arc_segment_count = 0; arc_segment_count < num_arc_segments; arc_segment_count++)
            {
               // the normals on the bottom point down, so override the defaults
               circle_verts_ptr[arc_segment_count].normal = glm::vec3(0.0f, -1.0f, 0.0f);

               // copy the modified vertex into the memory that's leaving this function
               // Note: Add 1 because the center vertex has already been added
               put_data_here->m_verts[num_verts_on_one_cap + 1 + arc_segment_count] = circle_verts_ptr[arc_segment_count];
            }

            // free the temporary memory
            free(circle_verts_ptr);
         }

         // cylinder body
         // Note: If there is 1 vertical segment, then two circles will be created.
         // If there are 2 vertical segments, then three circles will be created.
         // If someone tries to be funny and specifies 0 vertical segments, then 
         // 0 circles will be created and there will be no cylinder body.
         float vertical_segment_height = height / num_vertical_segments;
         for (uint vertical_segment_counter = 0; vertical_segment_counter < (num_vertical_segments + 1); vertical_segment_counter++)
         {
            if (0 == num_vertical_segments)
            {
               // someone is being silly
               break;
            }

            // construct it from the ground (height = 0) up
            My_Vertex *vertical_segment_verts_ptr = helper_generate_horizontal_circle(num_arc_segments, radius, (vertical_segment_height * vertical_segment_counter));

            // copy the data into memory
            uint bytes_to_copy = num_arc_segments * sizeof(My_Vertex);
            uint vertex_offset = (num_verts_on_one_cap * 2) + (num_arc_segments * vertical_segment_counter);
            My_Vertex *dest_ptr = put_data_here->m_verts + vertex_offset;
            memcpy(dest_ptr, vertical_segment_verts_ptr, bytes_to_copy);

            // free the memory created by the helper function
            free(vertical_segment_verts_ptr);
         }

         // each triangle fan will use one index for each vertex + 1 more to close it
         uint indices_on_one_cap = num_verts_on_one_cap + 1;

         // the body will be composed of a triangle strips
         // Note: Every vertical segemnt is composed of two connected circles (requires one index
         // per segment, similar to a triangle fan or a line strip), plus two triangles to close 
         // the loop (read, "plus 2 more indices between the upp and lower circles").
         uint indices_on_body = (num_vertical_segments * num_arc_segments + num_vertical_segments) * 2;

         // calculate the total resources required for all indices
         uint index_count = (indices_on_one_cap * 2) + indices_on_body;
         put_data_here->m_num_total_indices = index_count;
         put_data_here->m_indices = new GLushort[index_count];

         // for convenience
         GLushort *indices_ptr = put_data_here->m_indices;

         int index_counter_top = 0;
         int index_counter_bottom = indices_on_one_cap;

         // cap centers
         {
            // top
            indices_ptr[index_counter_top++] = 0;
         }
         {
            // bottom
            indices_ptr[index_counter_bottom++] = num_verts_on_one_cap;
         }

         // Note: Remember that the circle is created CCW on a plane in which one positive axis 
         // is to the right and the other is upwards.  Lay this plane down on whatever 
         // horizontal plane that you need to and then figure out your index ordering from there.
         for (uint arc_segment_count = 0; arc_segment_count < num_arc_segments; arc_segment_count++)
         {
            // top vertices need to be ordered CCW when seen from positive Y to avoid culling
            // Note: The circle is created in the X-Z plane, so when viewed from above, vertex
            // creation order would naturally be CW.  I therefore need to reverse the vertex 
            // creation order for the top cap.
            {
               // add 1 because the index for the center vert was already specified
               indices_ptr[index_counter_top++] = num_verts_on_one_cap - 1 - arc_segment_count;
               if (arc_segment_count == (num_arc_segments - 1))
               {
                  // last vertex, so loop back to first one AFTER the cap center
                  indices_ptr[index_counter_top++] = num_verts_on_one_cap - 1;
               }
            }

            // bottom vertices need to be ordered CCW when seen from negative Y 
            // (alternatively, CW from positive Y) to avoid culling
            {
               // specify the next vertex in the bottom fan
               indices_ptr[index_counter_bottom++] = 1 + arc_segment_count + num_verts_on_one_cap;
               if (arc_segment_count == (num_arc_segments - 1))
               {
                  // last vertex, so loop back to first one AFTER the cap center
                  indices_ptr[index_counter_bottom++] = num_verts_on_one_cap + 1;
               }
            }
         }

         uint body_index_counter = 0;

         // this loop is also immune to the "0 vertical segments" attempt
         // Note: Each vertical segment is built by connecting the vertices on one circle 
         // to the circle below it.  I can therefore have a loop condition that is dependent
         // upon the number of vertical segments only (unlike the circle creation loop).
         for (uint body_circle_counter = 0; body_circle_counter < num_vertical_segments; body_circle_counter++)
         {
            // in pairs of top circle followed by bottom circle
            for (uint arc_segment_counter = 0; arc_segment_counter < num_arc_segments; arc_segment_counter++)
            {
               uint index_offset = indices_on_one_cap * 2;
               uint top_vertex_offset = (num_verts_on_one_cap * 2) + (body_circle_counter * num_arc_segments);
               uint bottom_vertex_offset = (num_verts_on_one_cap * 2) + ((body_circle_counter + 1) * num_arc_segments);


               // connect the upper circle to the lower one
               // Note: Remember to keep in mind culling because these circles are created in the X-Z plane.
               // Construction is from the ground up, so the first circle will be at height = 0, and the next
               // circle will be above it.
               indices_ptr[index_offset + body_index_counter++] = top_vertex_offset + arc_segment_counter;
               indices_ptr[index_offset + body_index_counter++] = bottom_vertex_offset + arc_segment_counter;

               // at the last arc segment, reconnect it to the first segment 
               // through the first vertex in the upper circle
               if (arc_segment_counter == num_arc_segments - 1)
               {
                  indices_ptr[index_offset + body_index_counter++] = top_vertex_offset;
                  indices_ptr[index_offset + body_index_counter++] = bottom_vertex_offset;
               }
            }
         }

         // create the index meta data and add it to the shape data
         // Note: Remember that the first indices were the top cap, then the
         // bottom cap, and now the body.
         
         // top cap
         Index_Meta_Data top_i_meta_data(GL_TRIANGLE_FAN, indices_on_one_cap);
         put_data_here->m_index_meta_data.push_back(top_i_meta_data);
         
         // bottom cap
         Index_Meta_Data bottom_i_meta_data(GL_TRIANGLE_FAN, indices_on_one_cap);
         put_data_here->m_index_meta_data.push_back(bottom_i_meta_data);

         // body
         Index_Meta_Data body_i_meta_data(GL_TRIANGLE_STRIP, indices_on_body);
         put_data_here->m_index_meta_data.push_back(body_i_meta_data);

      }

      void Shape_Generator::generate_arcysynthesis_cylinder(Shape_Data *put_data_here)
      {
         My_Vertex local_verts[] =
         {
            glm::vec3(0.0f, 0.5f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(1.0f, 1.0f, 1.0f),

            glm::vec3(0.5f, 0.5f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.9f, 0.8f, 0.8f),

            glm::vec3(0.5f, -0.5f, 0.0f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.9f, 0.8f, 0.8f),

            glm::vec3(0.48907381875731f, 0.5f, 0.1039557588888f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.84f, 0.76f, 0.74f),

            glm::vec3(0.48907381875731f, -0.5f, 0.1039557588888f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.84f, 0.76f, 0.74f),

            glm::vec3(0.45677280077542f, 0.5f, 0.20336815992623f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.78f, 0.72f, 0.68f),

            glm::vec3(0.45677280077542f, -0.5f, 0.20336815992623f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.78f, 0.72f, 0.68f),

            glm::vec3(0.40450865316151f, 0.5f, 0.29389241146627f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.72f, 0.68f, 0.62f),

            glm::vec3(0.40450865316151f, -0.5f, 0.29389241146627f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.72f, 0.68f, 0.62f),

            glm::vec3(0.33456556611288f, 0.5f, 0.37157217599218f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.66f, 0.64f, 0.56f),

            glm::vec3(0.33456556611288f, -0.5f, 0.37157217599218f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.66f, 0.64f, 0.56f),

            glm::vec3(0.2500003830126f, 0.5f, 0.43301248075957f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.6f, 0.6f, 0.5f),

            glm::vec3(0.2500003830126f, -0.5f, 0.43301248075957f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.6f, 0.6f, 0.5f),

            glm::vec3(0.15450900193016f, 0.5f, 0.47552809414644f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.66f, 0.64f, 0.56f),

            glm::vec3(0.15450900193016f, -0.5f, 0.47552809414644f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.66f, 0.64f, 0.56f),

            glm::vec3(0.052264847412855f, 0.5f, 0.49726088296277f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.72f, 0.68f, 0.62f),

            glm::vec3(0.052264847412855f, -0.5f, 0.49726088296277f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.72f, 0.68f, 0.62f),

            glm::vec3(-0.052263527886268f, 0.5f, 0.49726102165048f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.78f, 0.72f, 0.68f),

            glm::vec3(-0.052263527886268f, -0.5f, 0.49726102165048f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.78f, 0.72f, 0.68f),

            glm::vec3(-0.15450774007312f, 0.5f, 0.47552850414828f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.84f, 0.76f, 0.74f),

            glm::vec3(-0.15450774007312f, -0.5f, 0.47552850414828f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.84f, 0.76f, 0.74f),

            glm::vec3(-0.24999923397422f, 0.5f, 0.43301314415651f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.9f, 0.8f, 0.8f),

            glm::vec3(-0.24999923397422f, -0.5f, 0.43301314415651f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.9f, 0.8f, 0.8f),

            glm::vec3(-0.33456458011157f, 0.5f, 0.37157306379065f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.84f, 0.76f, 0.74f),

            glm::vec3(-0.33456458011157f, -0.5f, 0.37157306379065f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.84f, 0.76f, 0.74f),

            glm::vec3(-0.40450787329018f, 0.5f, 0.29389348486527f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.78f, 0.72f, 0.68f),

            glm::vec3(-0.40450787329018f, -0.5f, 0.29389348486527f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.78f, 0.72f, 0.68f),

            glm::vec3(-0.45677226111814f, 0.5f, 0.20336937201315f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.72f, 0.68f, 0.62f),

            glm::vec3(-0.45677226111814f, -0.5f, 0.20336937201315f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.72f, 0.68f, 0.62f),

            glm::vec3(-0.48907354289964f, 0.5f, 0.10395705668972f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.66f, 0.64f, 0.56f),

            glm::vec3(-0.48907354289964f, -0.5f, 0.10395705668972f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.66f, 0.64f, 0.56f),

            glm::vec3(-0.49999999999824f, 0.5f, 1.3267948966764e-006f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.6f, 0.6f, 0.5f),

            glm::vec3(-0.49999999999824f, -0.5f, 1.3267948966764e-006f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.6f, 0.6f, 0.5f),

            glm::vec3(-0.48907409461153f, 0.5f, -0.10395446108714f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.66f, 0.64f, 0.56f),

            glm::vec3(-0.48907409461153f, -0.5f, -0.10395446108714f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.66f, 0.64f, 0.56f),

            glm::vec3(-0.45677334042948f, 0.5f, -0.20336694783787f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.72f, 0.68f, 0.62f),

            glm::vec3(-0.45677334042948f, -0.5f, -0.20336694783787f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.72f, 0.68f, 0.62f),

            glm::vec3(-0.40450943302999f, 0.5f, -0.2938913380652f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.78f, 0.72f, 0.68f),

            glm::vec3(-0.40450943302999f, -0.5f, -0.2938913380652f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.78f, 0.72f, 0.68f),

            glm::vec3(-0.33456655211184f, 0.5f, -0.3715712881911f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.84f, 0.76f, 0.74f),

            glm::vec3(-0.33456655211184f, -0.5f, -0.3715712881911f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.84f, 0.76f, 0.74f),

            glm::vec3(-0.25000153204922f, 0.5f, -0.43301181735958f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.9f, 0.8f, 0.8f),

            glm::vec3(-0.25000153204922f, -0.5f, -0.43301181735958f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.9f, 0.8f, 0.8f),

            glm::vec3(-0.15451026378611f, 0.5f, -0.47552768414126f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.84f, 0.76f, 0.74f),

            glm::vec3(-0.15451026378611f, -0.5f, -0.47552768414126f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.84f, 0.76f, 0.74f),

            glm::vec3(-0.052266166939075f, 0.5f, -0.49726074427155f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.78f, 0.72f, 0.68f),

            glm::vec3(-0.052266166939075f, -0.5f, -0.49726074427155f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.78f, 0.72f, 0.68f),

            glm::vec3(0.052262208359312f, 0.5f, -0.4972611603347f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.72f, 0.68f, 0.62f),

            glm::vec3(0.052262208359312f, -0.5f, -0.4972611603347f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.72f, 0.68f, 0.62f),

            glm::vec3(0.15450647821499f, 0.5f, -0.47552891414676f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.66f, 0.64f, 0.56f),

            glm::vec3(0.15450647821499f, -0.5f, -0.47552891414676f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.66f, 0.64f, 0.56f),

            glm::vec3(0.24999808493408f, 0.5f, -0.4330138075504f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.6f, 0.6f, 0.5f),

            glm::vec3(0.24999808493408f, -0.5f, -0.4330138075504f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.6f, 0.6f, 0.5f),

            glm::vec3(0.3345635941079f, 0.5f, -0.37157395158649f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.66f, 0.64f, 0.56f),

            glm::vec3(0.3345635941079f, -0.5f, -0.37157395158649f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.66f, 0.64f, 0.56f),

            glm::vec3(0.40450709341601f, 0.5f, -0.2938945582622f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.72f, 0.68f, 0.62f),

            glm::vec3(0.40450709341601f, -0.5f, -0.2938945582622f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.72f, 0.68f, 0.62f),

            glm::vec3(0.45677172145764f, 0.5f, -0.20337058409865f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.78f, 0.72f, 0.68f),

            glm::vec3(0.45677172145764f, -0.5f, -0.20337058409865f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.78f, 0.72f, 0.68f),

            glm::vec3(0.48907326703854f, 0.5f, -0.10395835448992f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.84f, 0.76f, 0.74f),

            glm::vec3(0.48907326703854f, -0.5f, -0.10395835448992f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.84f, 0.76f, 0.74f),

            glm::vec3(0.0f, -0.5f, 0.0f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(1.0f, 1.0f, 1.0f),

            glm::vec3(0.5f, 0.5f, 0.0f),
            glm::vec3(1.0f, 0.0f, 0.0f),
            glm::vec3(0.9f, 0.8f, 0.8f),

            glm::vec3(0.5f, -0.5f, 0.0f),
            glm::vec3(1.0f, 0.0f, 0.0f),
            glm::vec3(0.9f, 0.8f, 0.8f),

            glm::vec3(0.48907381875731f, 0.5f, 0.1039557588888f),
            glm::vec3(0.97814763751461f, 0.0f, 0.20791151777759f),
            glm::vec3(0.84f, 0.76f, 0.74f),

            glm::vec3(0.48907381875731f, -0.5f, 0.1039557588888f),
            glm::vec3(0.97814763751461f, 0.0f, 0.20791151777759f),
            glm::vec3(0.84f, 0.76f, 0.74f),

            glm::vec3(0.45677280077542f, 0.5f, 0.20336815992623f),
            glm::vec3(0.91354560155084f, 0.0f, 0.40673631985245f),
            glm::vec3(0.78f, 0.72f, 0.68f),

            glm::vec3(0.45677280077542f, -0.5f, 0.20336815992623f),
            glm::vec3(0.91354560155084f, 0.0f, 0.40673631985245f),
            glm::vec3(0.78f, 0.72f, 0.68f),

            glm::vec3(0.40450865316151f, 0.5f, 0.29389241146627f),
            glm::vec3(0.80901730632302f, 0.0f, 0.58778482293254f),
            glm::vec3(0.72f, 0.68f, 0.62f),

            glm::vec3(0.40450865316151f, -0.5f, 0.29389241146627f),
            glm::vec3(0.80901730632302f, 0.0f, 0.58778482293254f),
            glm::vec3(0.72f, 0.68f, 0.62f),

            glm::vec3(0.33456556611288f, 0.5f, 0.37157217599218f),
            glm::vec3(0.66913113222576f, 0.0f, 0.74314435198437f),
            glm::vec3(0.66f, 0.64f, 0.56f),

            glm::vec3(0.33456556611288f, -0.5f, 0.37157217599218f),
            glm::vec3(0.66913113222576f, 0.0f, 0.74314435198437f),
            glm::vec3(0.66f, 0.64f, 0.56f),

            glm::vec3(0.2500003830126f, 0.5f, 0.43301248075957f),
            glm::vec3(0.5000007660252f, 0.0f, 0.86602496151913f),
            glm::vec3(0.6f, 0.6f, 0.5f),

            glm::vec3(0.2500003830126f, -0.5f, 0.43301248075957f),
            glm::vec3(0.5000007660252f, 0.0f, 0.86602496151913f),
            glm::vec3(0.6f, 0.6f, 0.5f),

            glm::vec3(0.15450900193016f, 0.5f, 0.47552809414644f),
            glm::vec3(0.30901800386032f, 0.0f, 0.95105618829288f),
            glm::vec3(0.66f, 0.64f, 0.56f),

            glm::vec3(0.15450900193016f, -0.5f, 0.47552809414644f),
            glm::vec3(0.30901800386032f, 0.0f, 0.95105618829288f),
            glm::vec3(0.66f, 0.64f, 0.56f),

            glm::vec3(0.052264847412855f, 0.5f, 0.49726088296277f),
            glm::vec3(0.10452969482571f, 0.0f, 0.99452176592553f),
            glm::vec3(0.72f, 0.68f, 0.62f),

            glm::vec3(0.052264847412855f, -0.5f, 0.49726088296277f),
            glm::vec3(0.10452969482571f, 0.0f, 0.99452176592553f),
            glm::vec3(0.72f, 0.68f, 0.62f),

            glm::vec3(-0.052263527886268f, 0.5f, 0.49726102165048f),
            glm::vec3(-0.10452705577254f, 0.0f, 0.99452204330096f),
            glm::vec3(0.78f, 0.72f, 0.68f),

            glm::vec3(-0.052263527886268f, -0.5f, 0.49726102165048f),
            glm::vec3(-0.10452705577254f, 0.0f, 0.99452204330096f),
            glm::vec3(0.78f, 0.72f, 0.68f),

            glm::vec3(-0.15450774007312f, 0.5f, 0.47552850414828f),
            glm::vec3(-0.30901548014624f, 0.0f, 0.95105700829655f),
            glm::vec3(0.84f, 0.76f, 0.74f),

            glm::vec3(-0.15450774007312f, -0.5f, 0.47552850414828f),
            glm::vec3(-0.30901548014624f, 0.0f, 0.95105700829655f),
            glm::vec3(0.84f, 0.76f, 0.74f),

            glm::vec3(-0.24999923397422f, 0.5f, 0.43301314415651f),
            glm::vec3(-0.49999846794844f, 0.0f, 0.86602628831301f),
            glm::vec3(0.9f, 0.8f, 0.8f),

            glm::vec3(-0.24999923397422f, -0.5f, 0.43301314415651f),
            glm::vec3(-0.49999846794844f, 0.0f, 0.86602628831301f),
            glm::vec3(0.9f, 0.8f, 0.8f),

            glm::vec3(-0.33456458011157f, 0.5f, 0.37157306379065f),
            glm::vec3(-0.66912916022314f, 0.0f, 0.7431461275813f),
            glm::vec3(0.84f, 0.76f, 0.74f),

            glm::vec3(-0.33456458011157f, -0.5f, 0.37157306379065f),
            glm::vec3(-0.66912916022314f, 0.0f, 0.7431461275813f),
            glm::vec3(0.84f, 0.76f, 0.74f),

            glm::vec3(-0.40450787329018f, 0.5f, 0.29389348486527f),
            glm::vec3(-0.80901574658037f, 0.0f, 0.58778696973054f),
            glm::vec3(0.78f, 0.72f, 0.68f),

            glm::vec3(-0.40450787329018f, -0.5f, 0.29389348486527f),
            glm::vec3(-0.80901574658037f, 0.0f, 0.58778696973054f),
            glm::vec3(0.78f, 0.72f, 0.68f),

            glm::vec3(-0.45677226111814f, 0.5f, 0.20336937201315f),
            glm::vec3(-0.91354452223627f, 0.0f, 0.40673874402631f),
            glm::vec3(0.72f, 0.68f, 0.62f),

            glm::vec3(-0.45677226111814f, -0.5f, 0.20336937201315f),
            glm::vec3(-0.91354452223627f, 0.0f, 0.40673874402631f),
            glm::vec3(0.72f, 0.68f, 0.62f),

            glm::vec3(-0.48907354289964f, 0.5f, 0.10395705668972f),
            glm::vec3(-0.97814708579929f, 0.0f, 0.20791411337945f),
            glm::vec3(0.66f, 0.64f, 0.56f),

            glm::vec3(-0.48907354289964f, -0.5f, 0.10395705668972f),
            glm::vec3(-0.97814708579929f, 0.0f, 0.20791411337945f),
            glm::vec3(0.66f, 0.64f, 0.56f),

            glm::vec3(-0.49999999999824f, 0.5f, 1.3267948966764e-006f),
            glm::vec3(-0.99999999999648f, 0.0f, 2.6535897933527e-006f),
            glm::vec3(0.6f, 0.6f, 0.5f),

            glm::vec3(-0.49999999999824f, -0.5f, 1.3267948966764e-006f),
            glm::vec3(-0.99999999999648f, 0.0f, 2.6535897933527e-006f),
            glm::vec3(0.6f, 0.6f, 0.5f),

            glm::vec3(-0.48907409461153f, 0.5f, -0.10395446108714f),
            glm::vec3(-0.97814818922305f, 0.0f, -0.20790892217427f),
            glm::vec3(0.66f, 0.64f, 0.56f),

            glm::vec3(-0.48907409461153f, -0.5f, -0.10395446108714f),
            glm::vec3(-0.97814818922305f, 0.0f, -0.20790892217427f),
            glm::vec3(0.66f, 0.64f, 0.56f),

            glm::vec3(-0.45677334042948f, 0.5f, -0.20336694783787f),
            glm::vec3(-0.91354668085897f, 0.0f, -0.40673389567574f),
            glm::vec3(0.72f, 0.68f, 0.62f),

            glm::vec3(-0.45677334042948f, -0.5f, -0.20336694783787f),
            glm::vec3(-0.91354668085897f, 0.0f, -0.40673389567574f),
            glm::vec3(0.72f, 0.68f, 0.62f),

            glm::vec3(-0.40450943302999f, 0.5f, -0.2938913380652f),
            glm::vec3(-0.80901886605998f, 0.0f, -0.58778267613041f),
            glm::vec3(0.78f, 0.72f, 0.68f),

            glm::vec3(-0.40450943302999f, -0.5f, -0.2938913380652f),
            glm::vec3(-0.80901886605998f, 0.0f, -0.58778267613041f),
            glm::vec3(0.78f, 0.72f, 0.68f),

            glm::vec3(-0.33456655211184f, 0.5f, -0.3715712881911f),
            glm::vec3(-0.66913310422368f, 0.0f, -0.74314257638221f),
            glm::vec3(0.84f, 0.76f, 0.74f),

            glm::vec3(-0.33456655211184f, -0.5f, -0.3715712881911f),
            glm::vec3(-0.66913310422368f, 0.0f, -0.74314257638221f),
            glm::vec3(0.84f, 0.76f, 0.74f),

            glm::vec3(-0.25000153204922f, 0.5f, -0.43301181735958f),
            glm::vec3(-0.50000306409843f, 0.0f, -0.86602363471916f),
            glm::vec3(0.9f, 0.8f, 0.8f),

            glm::vec3(-0.25000153204922f, -0.5f, -0.43301181735958f),
            glm::vec3(-0.50000306409843f, 0.0f, -0.86602363471916f),
            glm::vec3(0.9f, 0.8f, 0.8f),

            glm::vec3(-0.15451026378611f, 0.5f, -0.47552768414126f),
            glm::vec3(-0.30902052757222f, 0.0f, -0.95105536828251f),
            glm::vec3(0.84f, 0.76f, 0.74f),

            glm::vec3(-0.15451026378611f, -0.5f, -0.47552768414126f),
            glm::vec3(-0.30902052757222f, 0.0f, -0.95105536828251f),
            glm::vec3(0.84f, 0.76f, 0.74f),

            glm::vec3(-0.052266166939075f, 0.5f, -0.49726074427155f),
            glm::vec3(-0.10453233387815f, 0.0f, -0.9945214885431f),
            glm::vec3(0.78f, 0.72f, 0.68f),

            glm::vec3(-0.052266166939075f, -0.5f, -0.49726074427155f),
            glm::vec3(-0.10453233387815f, 0.0f, -0.9945214885431f),
            glm::vec3(0.78f, 0.72f, 0.68f),

            glm::vec3(0.052262208359312f, 0.5f, -0.4972611603347f),
            glm::vec3(0.10452441671862f, 0.0f, -0.99452232066939f),
            glm::vec3(0.72f, 0.68f, 0.62f),

            glm::vec3(0.052262208359312f, -0.5f, -0.4972611603347f),
            glm::vec3(0.10452441671862f, 0.0f, -0.99452232066939f),
            glm::vec3(0.72f, 0.68f, 0.62f),

            glm::vec3(0.15450647821499f, 0.5f, -0.47552891414676f),
            glm::vec3(0.30901295642998f, 0.0f, -0.95105782829353f),
            glm::vec3(0.66f, 0.64f, 0.56f),

            glm::vec3(0.15450647821499f, -0.5f, -0.47552891414676f),
            glm::vec3(0.30901295642998f, 0.0f, -0.95105782829353f),
            glm::vec3(0.66f, 0.64f, 0.56f),

            glm::vec3(0.24999808493408f, 0.5f, -0.4330138075504f),
            glm::vec3(0.49999616986816f, 0.0f, -0.8660276151008f),
            glm::vec3(0.6f, 0.6f, 0.5f),

            glm::vec3(0.24999808493408f, -0.5f, -0.4330138075504f),
            glm::vec3(0.49999616986816f, 0.0f, -0.8660276151008f),
            glm::vec3(0.6f, 0.6f, 0.5f),

            glm::vec3(0.3345635941079f, 0.5f, -0.37157395158649f),
            glm::vec3(0.66912718821581f, 0.0f, -0.74314790317299f),
            glm::vec3(0.66f, 0.64f, 0.56f),

            glm::vec3(0.3345635941079f, -0.5f, -0.37157395158649f),
            glm::vec3(0.66912718821581f, 0.0f, -0.74314790317299f),
            glm::vec3(0.66f, 0.64f, 0.56f),

            glm::vec3(0.40450709341601f, 0.5f, -0.2938945582622f),
            glm::vec3(0.80901418683202f, 0.0f, -0.5877891165244f),
            glm::vec3(0.72f, 0.68f, 0.62f),

            glm::vec3(0.40450709341601f, -0.5f, -0.2938945582622f),
            glm::vec3(0.80901418683202f, 0.0f, -0.5877891165244f),
            glm::vec3(0.72f, 0.68f, 0.62f),

            glm::vec3(0.45677172145764f, 0.5f, -0.20337058409865f),
            glm::vec3(0.91354344291528f, 0.0f, -0.40674116819729f),
            glm::vec3(0.78f, 0.72f, 0.68f),

            glm::vec3(0.45677172145764f, -0.5f, -0.20337058409865f),
            glm::vec3(0.91354344291528f, 0.0f, -0.40674116819729f),
            glm::vec3(0.78f, 0.72f, 0.68f),

            glm::vec3(0.48907326703854f, 0.5f, -0.10395835448992f),
            glm::vec3(0.97814653407707f, 0.0f, -0.20791670897984f),
            glm::vec3(0.84f, 0.76f, 0.74f),

            glm::vec3(0.48907326703854f, -0.5f, -0.10395835448992f),
            glm::vec3(0.97814653407707f, 0.0f, -0.20791670897984f),
            glm::vec3(0.84f, 0.76f, 0.74f),
         };

         // copy away!
         uint array_size_bytes = sizeof(local_verts);
         uint num_verts = array_size_bytes / sizeof(*local_verts);
         put_data_here->m_num_verts = num_verts;
         put_data_here->m_verts = new My_Vertex[num_verts];
         memcpy(put_data_here->m_verts, local_verts, array_size_bytes);


         // start a new block so that I can re-use some variable names
         {
            // for the top (a fan configuration)
            GLushort local_indices[] =
            {
               0, 1, 3, 5, 7, 9,
               11, 13, 15, 17, 19, 21,
               23, 25, 27, 29, 31, 33,
               35, 37, 39, 41, 43, 45,
               47, 49, 51, 53, 55, 57,
               59, 1,
            };

            // copy away!
            array_size_bytes = sizeof(local_indices);
            uint num_indices = array_size_bytes / sizeof(*local_indices);
            put_data_here->m_num_total_indices = num_indices;
            put_data_here->m_indices = new GLushort[num_indices];
            memcpy(put_data_here->m_indices, local_indices, array_size_bytes);

            Index_Meta_Data i_meta_data(GL_TRIANGLE_FAN, num_indices);
            put_data_here->m_index_meta_data.push_back(i_meta_data);
         }

         // start a new block so that I can re-use some variable names
         {
            // for the bottom (a fan configuration)
            GLushort local_indices[] =
            {
               61, 60, 58, 56, 54, 52,
               50, 48, 46, 44, 42, 40,
               38, 36, 34, 32, 30, 28,
               26, 24, 22, 20, 18, 16,
               14, 12, 10, 8, 6, 4, 2,
               60,
            };

            // copy away!
            array_size_bytes = sizeof(local_indices);
            uint num_indices = array_size_bytes / sizeof(*local_indices);
            put_data_here->m_num_total_indices = num_indices;
            put_data_here->m_indices = new GLushort[num_indices];
            memcpy(put_data_here->m_indices, local_indices, array_size_bytes);

            Index_Meta_Data i_meta_data(GL_TRIANGLE_FAN, num_indices);
            put_data_here->m_index_meta_data.push_back(i_meta_data);

         }

         // start a new block so that I can re-use some variable names
         {
            // for the rest of it
            GLushort local_indices[] =
            {
               62, 63, 64, 65, 66, 67,
               //68, 69, 70, 71, 72, 73,
               //74, 75, 76, 77, 78, 79,
               //80, 81, 82, 83, 84, 85,
               //86, 87, 88, 89, 90, 91,
               //92, 93, 94, 95, 96, 97,
               //98, 99, 100, 101, 102, 103,
               //104, 105, 106, 107, 108, 109,
               //110, 111, 112, 113, 114, 115,
               //116, 117, 118, 119, 120, 121,
               //62, 63,
            };

            // copy away!
            array_size_bytes = sizeof(local_indices);
            uint num_indices = array_size_bytes / sizeof(*local_indices);
            put_data_here->m_num_total_indices = num_indices;
            put_data_here->m_indices = new GLushort[num_indices];
            memcpy(put_data_here->m_indices, local_indices, array_size_bytes);

            Index_Meta_Data i_meta_data(GL_TRIANGLE_STRIP, num_indices);
            put_data_here->m_index_meta_data.push_back(i_meta_data);
         }
      }

      void Shape_Generator::generate_box(const float width, const float length, Shape_Data *put_data_here)
      {
         float half_width = width / 2.0f;
         float half_length = length / 2.0f;
         
         glm::vec3 common_normal = glm::vec3(0.0f, 1.0f, 0.0f);

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
            glm::vec3(-1.0f, +1.0f, +1.0f),  // 0
            glm::vec3(+0.0f, +1.0f, +0.0f),  // Normal
            random_color(),
            glm::vec3(+1.0f, +1.0f, +1.0f),  // 1
            glm::vec3(+0.0f, +1.0f, +0.0f),  // Normal
            random_color(),
            glm::vec3(+1.0f, +1.0f, -1.0f),  // 2
            glm::vec3(+0.0f, +1.0f, +0.0f),  // Normal
            random_color(),
            glm::vec3(-1.0f, +1.0f, -1.0f),  // 3
            glm::vec3(+0.0f, +1.0f, +0.0f),  // Normal
            random_color(),

            glm::vec3(-1.0f, +1.0f, -1.0f),  // 4
            glm::vec3(+0.0f, +0.0f, -1.0f),  // Normal
            random_color(),
            glm::vec3(+1.0f, +1.0f, -1.0f),  // 5
            glm::vec3(+0.0f, +0.0f, -1.0f),  // Normal
            random_color(),
            glm::vec3(+1.0f, -1.0f, -1.0f),  // 6
            glm::vec3(+0.0f, +0.0f, -1.0f),  // Normal
            random_color(),
            glm::vec3(-1.0f, -1.0f, -1.0f),  // 7
            glm::vec3(+0.0f, +0.0f, -1.0f),  // Normal
            random_color(),

            glm::vec3(+1.0f, +1.0f, -1.0f),  // 8
            glm::vec3(+1.0f, +0.0f, +0.0f),  // Normal
            random_color(),
            glm::vec3(+1.0f, +1.0f, +1.0f),  // 9
            glm::vec3(+1.0f, +0.0f, +0.0f),  // Normal
            random_color(),
            glm::vec3(+1.0f, -1.0f, +1.0f),  // 10
            glm::vec3(+1.0f, +0.0f, +0.0f),  // Normal
            random_color(),
            glm::vec3(+1.0f, -1.0f, -1.0f),  // 11
            glm::vec3(+1.0f, +0.0f, +0.0f),  // Normal
            random_color(),

            glm::vec3(-1.0f, +1.0f, +1.0f),  // 12
            glm::vec3(-1.0f, +0.0f, +0.0f),  // Normal
            random_color(),
            glm::vec3(-1.0f, +1.0f, -1.0f),  // 13
            glm::vec3(-1.0f, +0.0f, +0.0f),  // Normal
            random_color(),
            glm::vec3(-1.0f, -1.0f, -1.0f),  // 14
            glm::vec3(-1.0f, +0.0f, +0.0f),  // Normal
            random_color(),
            glm::vec3(-1.0f, -1.0f, +1.0f),  // 15
            glm::vec3(-1.0f, +0.0f, +0.0f),  // Normal
            random_color(),

            glm::vec3(+1.0f, +1.0f, +1.0f),  // 16
            glm::vec3(+0.0f, +0.0f, +1.0f),  // Normal
            random_color(),
            glm::vec3(-1.0f, +1.0f, +1.0f),  // 17
            glm::vec3(+0.0f, +0.0f, +1.0f),  // Normal
            random_color(),
            glm::vec3(-1.0f, -1.0f, +1.0f),  // 18
            glm::vec3(+0.0f, +0.0f, +1.0f),  // Normal
            random_color(),
            glm::vec3(+1.0f, -1.0f, +1.0f),  // 19
            glm::vec3(+0.0f, +0.0f, +1.0f),  // Normal
            random_color(),

            glm::vec3(+1.0f, -1.0f, -1.0f),  // 20
            glm::vec3(+0.0f, -1.0f, +0.0f),  // Normal
            random_color(),
            glm::vec3(-1.0f, -1.0f, -1.0f),  // 21
            glm::vec3(+0.0f, -1.0f, +0.0f),  // Normal
            random_color(),
            glm::vec3(-1.0f, -1.0f, +1.0f),  // 22
            glm::vec3(+0.0f, -1.0f, +0.0f),  // Normal
            random_color(),
            glm::vec3(+1.0f, -1.0f, +1.0f),  // 23
            glm::vec3(+0.0f, -1.0f, +0.0f),  // Normal
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