#include "AABB_Component.h"

#include <Entities\Entity.h>
#include <Entities\Components\Physics_Component.h>
#include <Shapes\Geometry.h>
#include <Shapes\Geometry_Meta_Data.h>
#include <Shapes\My_Vertex.h>
#include <memory.h>
#include <Utilities\My_Assert.h>

#include <Utilities\Printer_Helper.h>

#include <Math\F_Dual_Quat.h>

#include <iostream>
using std::cout;
using std::endl;


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

      const Shapes::Geometry_Meta_Data &meta_data_ref = m_geometry_data_ptr->m_meta_data;

      // set the default vectors so that they can be transformed on the next update
      m_default_box_corners[BOX_CORNERS::RIGHT_UPPER_FRONT] = glm::vec3(meta_data_ref.m_max_X, meta_data_ref.m_max_Y, meta_data_ref.m_min_Z);
      m_default_box_corners[BOX_CORNERS::RIGHT_UPPER_BACK] = glm::vec3(meta_data_ref.m_max_X, meta_data_ref.m_max_Y, meta_data_ref.m_max_Z);
      m_default_box_corners[BOX_CORNERS::LEFT_UPPER_FRONT] = glm::vec3(meta_data_ref.m_min_X, meta_data_ref.m_max_Y, meta_data_ref.m_min_Z);
      m_default_box_corners[BOX_CORNERS::LEFT_UPPER_BACK] = glm::vec3(meta_data_ref.m_min_X, meta_data_ref.m_max_Y, meta_data_ref.m_max_Z);
      m_default_box_corners[BOX_CORNERS::RIGHT_LOWER_BACK] = glm::vec3(meta_data_ref.m_max_X, meta_data_ref.m_min_Y, meta_data_ref.m_min_Z);
      m_default_box_corners[BOX_CORNERS::RIGHT_LOWER_BACK] = glm::vec3(meta_data_ref.m_max_X, meta_data_ref.m_min_Y, meta_data_ref.m_max_Z);
      m_default_box_corners[BOX_CORNERS::LEFT_LOWER_FRONT] = glm::vec3(meta_data_ref.m_min_X, meta_data_ref.m_min_Y, meta_data_ref.m_min_Z);
      m_default_box_corners[BOX_CORNERS::LEFT_LOWER_BACK] = glm::vec3(meta_data_ref.m_min_X, meta_data_ref.m_min_Y, meta_data_ref.m_max_Z);

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
      recalculate_all_min_max_values(curr_box_corners, BOX_CORNERS::NUM_CORNERS);
   }


   glm::vec3 AABB_Component::is_colliding_with_AABB(const AABB_Component &other_box) const
   {
      // this overlap section does not make sense without imagining the boundaries of two cubes 
      // infringing on each other on a given axis, but it works
      bool X_overlap = (m_curr_min_x < other_box.m_curr_max_x) && (m_curr_max_x > other_box.m_curr_min_x);   // "left" is negative X
      bool Y_overlap = (m_curr_max_y > other_box.m_curr_min_y) && (m_curr_min_y < other_box.m_curr_max_y);   // "up" is positive Y
      bool Z_overlap = (m_curr_min_z < other_box.m_curr_max_z) && (m_curr_max_z > other_box.m_curr_min_z);   // "forward" is negative Z

      if (X_overlap && Y_overlap && Z_overlap)
      {
         //cout << "collision" << endl;
      }

      return glm::vec3();
   }

   void AABB_Component::recalculate_all_min_max_values(const glm::vec3 *curr_box_corners_arr, const int max_vectors)
   {
      // start the min/max values with an initial data point that is within the boundaries 
      // of the data
      // Note: If I don't, then you may start this comparison with a minimum value that 
      // is less than all of the box's actual corner positions.  For example, the entity
      // that this box is bounding can move up.  The current box corners are arguments to
      // this function, and if the box moved up, then they will be above all the previous 
      // minimum values.  If I don't reset the minimum value with one of the box's current
      // corners, then I could have an erroneously low minimum Y value.  Ditto for all 
      // box corner extremeties.
      const glm::vec3 &initial_vert = *curr_box_corners_arr;
      m_curr_min_x = initial_vert.x;
      m_curr_max_x = initial_vert.x;
      m_curr_min_y = initial_vert.y;
      m_curr_max_y = initial_vert.y;
      m_curr_min_z = initial_vert.z;
      m_curr_max_z = initial_vert.z;

      // go through all the vectors and find the min and max in all axes
      for (uint vec_index = 0; vec_index < max_vectors; vec_index++)
      {
         const glm::vec3 &vec_ref = curr_box_corners_arr[vec_index];

         if (vec_ref.x < m_curr_min_x) { m_curr_min_x = vec_ref.x; }
         else if (vec_ref.x > m_curr_max_x) { m_curr_max_x = vec_ref.x; }

         if (vec_ref.y < m_curr_min_y) { m_curr_min_y = vec_ref.y; }
         else if (vec_ref.y > m_curr_max_y) { m_curr_max_y = vec_ref.y; }

         if (vec_ref.z < m_curr_min_z) { m_curr_min_z = vec_ref.z; }
         else if (vec_ref.z > m_curr_max_z) { m_curr_max_z = vec_ref.z; }
      }
   }
}