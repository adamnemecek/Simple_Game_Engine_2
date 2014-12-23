#include "AABB_Component.h"

#include <Entities\Entity.h>
#include <Shapes\Geometry.h>
#include <Shapes\My_Vertex.h>
#include <memory.h>
#include <Utilities\My_Assert.h>

#include <Utilities\Printer_Helper.h>

#include <Math\F_Dual_Quat.h>

#include <iostream>
using std::cout;
using std::endl;


// a helper namespace for non-class helper things
namespace
{
   inline float avg(float min, float max)
   {
      return (max + min) / 2.0f;
   }

   inline float half_distance(float min, float max)
   {
      return (max - min) / 2.0f;
   }

}

namespace Entities
{
   AABB_Component::AABB_Component()
      : m_geometry_data_ptr(0)
   {
   }

   void AABB_Component::set_geometry(const Shapes::Geometry *box_this)
   {
      m_geometry_data_ptr = box_this;
   }

   bool AABB_Component::initialize()
   {
      // can't initialize without any geometry
      MY_ASSERT(m_geometry_data_ptr != 0);

      recalculate_all_min_max_values(
         &(m_geometry_data_ptr->m_shape_data.m_verts->position), 
         m_geometry_data_ptr->m_shape_data.m_num_verts,
         sizeof(Shapes::My_Vertex) / sizeof(glm::vec3));

      // set the default vectors so that they can be transformed on the next update
      m_default_box_corners[BOX_CORNERS::RIGHT_UPPER_FRONT] = glm::vec3(m_curr_max_x, m_curr_max_y, m_curr_min_z);
      m_default_box_corners[BOX_CORNERS::RIGHT_UPPER_BACK] = glm::vec3(m_curr_max_x, m_curr_max_y, m_curr_max_z);
      m_default_box_corners[BOX_CORNERS::LEFT_UPPER_FRONT] = glm::vec3(m_curr_min_x, m_curr_max_y, m_curr_min_z);
      m_default_box_corners[BOX_CORNERS::LEFT_UPPER_BACK] = glm::vec3(m_curr_min_x, m_curr_max_y, m_curr_max_z);
      m_default_box_corners[BOX_CORNERS::RIGHT_LOWER_BACK] = glm::vec3(m_curr_max_x, m_curr_min_y, m_curr_min_z);
      m_default_box_corners[BOX_CORNERS::RIGHT_LOWER_BACK] = glm::vec3(m_curr_max_x, m_curr_min_y, m_curr_max_z);
      m_default_box_corners[BOX_CORNERS::LEFT_LOWER_FRONT] = glm::vec3(m_curr_min_x, m_curr_min_y, m_curr_min_z);
      m_default_box_corners[BOX_CORNERS::LEFT_LOWER_BACK] = glm::vec3(m_curr_min_x, m_curr_min_y, m_curr_max_z);

      return true;
   }

   void AABB_Component::update()
   {
      glm::vec3 curr_box_corners[BOX_CORNERS::NUM_CORNERS];

      // transform the face center points
      const Math::F_Dual_Quat &transform = m_parent_entity_ptr->m_where_and_which_way;
      for (uint index = 0; index < BOX_CORNERS::NUM_CORNERS; index++)
      {
         curr_box_corners[index] = Math::F_Dual_Quat::transform(transform, m_default_box_corners[index]);
      }

      // data is closely packed, so index_stride is 1
      recalculate_all_min_max_values(curr_box_corners, BOX_CORNERS::NUM_CORNERS, 1);

      //static int count = 0;
      //printf("Z min:%.2f, Z max:%.2f, Z diff:%.2f - ", m_curr_min_z, m_curr_max_z, m_curr_max_z - m_curr_min_z);
      //count += 1;
      //if (2 == count)
      //{
      //   count = 0;
      //   printf("\n");
      //}
   }


   glm::vec3 AABB_Component::is_colliding_with_AABB(const AABB_Component &other_box)
   {
      // this overlap section does not make sense without imagining the boundaries of two cubes 
      // infringing on each other on a given axis, but it works
      bool X_overlap = (m_curr_min_x < other_box.m_curr_max_x) && (m_curr_max_x > other_box.m_curr_min_x);   // "left" is negative X
      bool Y_overlap = (m_curr_max_y > other_box.m_curr_min_y) && (m_curr_min_y < other_box.m_curr_max_y);   // "up" is positive Y
      bool Z_overlap = (m_curr_min_z < other_box.m_curr_max_z) && (m_curr_max_z > other_box.m_curr_min_z);   // "forward" is negative Z

      if (X_overlap)
      {
         //cout << " - X overlap";
      }
      
      if (Y_overlap)
      {
         //cout << " - Y overlap";
      }

      if (Z_overlap)
      {
         cout << " - Z overlap";
      }

      if (X_overlap || Y_overlap || Z_overlap)
      {
         cout << endl;
      }


      return glm::vec3();
   }


   void AABB_Component::recalculate_all_min_max_values(const glm::vec3 *vec_data_source, const uint max_vectors, const uint index_stride)
   {
      // give the min/max values initial data that is within the boundaries of the data
      const glm::vec3 &initial_vert = *vec_data_source;
      m_curr_min_x = initial_vert.x;
      m_curr_max_x = initial_vert.x;
      m_curr_min_y = initial_vert.y;
      m_curr_max_y = initial_vert.y;
      m_curr_min_z = initial_vert.z;
      m_curr_max_z = initial_vert.z;

      // go through all the vectors and find the min and max in all axes
      for (uint vec_counter = 0; vec_counter < max_vectors; vec_counter++)
      {
         // calculate the index location of the vector data
         // Ex: In My_Vertex, there are position, normal, and color vec3s, so the
         // first position index (in vec3 land, not in My_Vertex land) is [0], the
         // next is [3], the next is [6], etc.
         // Ex: In m_default_face_centers, the vectors are closely packed, so the
         // first position is at index [0], the next at [1], the next at [2], etc.
         uint vec_index = vec_counter * index_stride;

         const glm::vec3 &vec_ref = vec_data_source[vec_index];

         if (vec_ref.x < m_curr_min_x) { m_curr_min_x = vec_ref.x; }
         else if (vec_ref.x > m_curr_max_x) { m_curr_max_x = vec_ref.x; }

         if (vec_ref.y < m_curr_min_y) { m_curr_min_y = vec_ref.y; }
         else if (vec_ref.y > m_curr_max_y) { m_curr_max_y = vec_ref.y; }

         if (vec_ref.z < m_curr_min_z) { m_curr_min_z = vec_ref.z; }
         else if (vec_ref.z > m_curr_max_z) { m_curr_max_z = vec_ref.z; }
      }
   }
}