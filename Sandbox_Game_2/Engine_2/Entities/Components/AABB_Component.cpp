#include "AABB_Component.h"

#include <Entities\Entity.h>
#include <Entities\Components\Physics_Component.h>
#include <Shapes\Geometry.h>
#include <Shapes\Shape_Meta_Data.h>
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

      const Shapes::Shape_Meta_Data &meta_data_ref = m_geometry_data_ptr->m_meta_data;

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


   bool AABB_Component::is_colliding_with_AABB(const AABB_Component &other_box, glm::vec3 *put_overlap_here) const
   {
      // this overlap section does not make sense without imagining the boundaries of two cubes 
      // infringing on each other on a given axis, but it works
      //bool X_overlap = (m_curr_min_X < other_box.m_curr_max_X) && (m_curr_max_X > other_box.m_curr_min_X);   // "left" is negative X
      //bool Y_overlap = (m_curr_max_Y > other_box.m_curr_min_Y) && (m_curr_min_Y < other_box.m_curr_max_Y);   // "up" is positive Y
      //bool Z_overlap = (m_curr_min_Z < other_box.m_curr_max_Z) && (m_curr_max_Z > other_box.m_curr_min_Z);   // "forward" is negative Z

      //if (X_overlap && Y_overlap && Z_overlap)
      //{
      //   //cout << "collision" << endl;
      //}

      // Note: I wanted this calculation to be a general approach, so I am using positive/negtive 
      // signs to build directions into the overlap data.  
      // Ex: A positive X overlap means that the overlap happened on the +X side of this box (as 
      // opposed to the other box).  A negative X overlap means that the overlap happened on the -X 
      // side of this box.
      float other_max_to_this_min = 0.0f;
      float this_max_to_other_min = 0.0f;
      bool X_does_overlap = false;
      bool Y_does_overlap = false;
      bool Z_does_overlap = false;

      // X overlap
      other_max_to_this_min = other_box.m_curr_max_X - m_curr_min_X;
      this_max_to_other_min = m_curr_max_X - other_box.m_curr_min_X;
      X_does_overlap = calculate_overlap_on_axis(other_max_to_this_min, this_max_to_other_min, &(put_overlap_here->x));

      // Y overlap (if X succeeded)
      if (X_does_overlap)
      {
         other_max_to_this_min = other_box.m_curr_max_Y - m_curr_min_Y;
         this_max_to_other_min = m_curr_max_Y - other_box.m_curr_min_Y;
         Y_does_overlap = calculate_overlap_on_axis(other_max_to_this_min, this_max_to_other_min, &(put_overlap_here->y));
      }

      // Z overlap (if Y succeeded)
      if (Y_does_overlap)
      {
         other_max_to_this_min = other_box.m_curr_max_Z - m_curr_min_Z;
         this_max_to_other_min = m_curr_max_Z - other_box.m_curr_min_Z;
         Z_does_overlap = calculate_overlap_on_axis(other_max_to_this_min, this_max_to_other_min, &(put_overlap_here->z));
      }

      if (X_does_overlap && Y_does_overlap && Z_does_overlap)
      {
         return true;
      }
      else
      {
         return false;
      }
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
      m_curr_min_X = initial_vert.x;
      m_curr_max_X = initial_vert.x;
      m_curr_min_Y = initial_vert.y;
      m_curr_max_Y = initial_vert.y;
      m_curr_min_Z = initial_vert.z;
      m_curr_max_Z = initial_vert.z;

      // go through all the vectors and find the min and max in all axes
      for (uint vec_index = 0; vec_index < max_vectors; vec_index++)
      {
         const glm::vec3 &vec_ref = curr_box_corners_arr[vec_index];

         if (vec_ref.x < m_curr_min_X) { m_curr_min_X = vec_ref.x; }
         else if (vec_ref.x > m_curr_max_X) { m_curr_max_X = vec_ref.x; }

         if (vec_ref.y < m_curr_min_Y) { m_curr_min_Y = vec_ref.y; }
         else if (vec_ref.y > m_curr_max_Y) { m_curr_max_Y = vec_ref.y; }

         if (vec_ref.z < m_curr_min_Z) { m_curr_min_Z = vec_ref.z; }
         else if (vec_ref.z > m_curr_max_Z) { m_curr_max_Z = vec_ref.z; }
      }
   }

   bool AABB_Component::calculate_overlap_on_axis(const float other_max_to_this_min, const float this_max_to_other_min, float *put_overlap_here) const
   {
      if ((other_max_to_this_min < 0) ||
         (this_max_to_other_min < 0))
      {
         // no overlap
         *put_overlap_here = 0.0f;
         return false;
      }
      else
      {
         if (other_max_to_this_min < this_max_to_other_min)
         {
            // collision is on the -axis side, so overlap magnitude will be negative
            *put_overlap_here = (-1.0f) * other_max_to_this_min;
            return true;
         }
         else
         {
            // collision is on the +axis side, so no sign change
            *put_overlap_here = this_max_to_other_min;
            return true;
         }
      }
   }
}