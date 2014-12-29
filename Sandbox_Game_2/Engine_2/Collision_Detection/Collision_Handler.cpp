#include "Collision_Handler.h"

#include <Entities\Entity.h>
#include <Entities\Components\AABB_Component.h>
#include <Entities\Components\Physics_Component.h>
#include <Utilities\My_Assert.h>
#include <memory.h>

#include <iostream>
using std::cout;
using std::endl;


namespace Collision_Detection
{
   Collision_Handler &Collision_Handler::get_instance()
   {
      static Collision_Handler instance;

      return instance;
   }

   bool Collision_Handler::initialize()
   {
      // erase all pointer data that might exist
      memset(m_bounding_box_ptr_arr, 0, sizeof(Entities::AABB_Component *) * m_MAX_ENTITIES);
      memset(m_physics_ptr_arr, 0, sizeof(Entities::Physics_Component *) * m_MAX_ENTITIES);

      // all indices are available at the start
      // Note: Clear the data structure anyway in the event that initialization is called 
      // sometime after it was already initialized and used.
      m_available_indices.clear();
      for (uint entity_counter = 0; entity_counter < m_MAX_ENTITIES; entity_counter++)
      {
         m_available_indices.push_back(entity_counter);
      }

      return true;
   }

   void Collision_Handler::update()
   {
      uint num_current_collision_IDs = m_MAX_ENTITIES - m_available_indices.size();

      // check each box against every other box once
      // Note: The following nested for-loop conditions will only run if there are at least 2 boxes.
      // If there is only 1 box, then the inner loop's end condition will be triggered before it 
      // even starts, and if there are 0 boxes then the outer loop's end condition will be triggered
      // before it even starts.
      for (uint this_index = 0; this_index < num_current_collision_IDs; this_index++)
      {
         for (uint other_index = this_index + 1; other_index < num_current_collision_IDs; other_index++)
         {
            //m_bounding_boxes[this_BB_index]->is_colliding_with_AABB(*m_bounding_boxes[other_BB_index]);

            const Entities::AABB_Component *this_BB_ptr = m_bounding_box_ptr_arr[this_index];
            const Entities::AABB_Component *other_BB_ptr = m_bounding_box_ptr_arr[other_index];
            const Entities::Physics_Component *this_physics_ptr = m_physics_ptr_arr[this_index];
            const Entities::Physics_Component *other_physics_ptr = m_physics_ptr_arr[other_index];

            glm::vec3 overlap_vector;
            bool collision_is_a_thing = this_BB_ptr->is_colliding_with_AABB(*other_BB_ptr, &overlap_vector);

            if (!collision_is_a_thing)
            {
               continue;
            }

            // calculate where the boxes' overlap corners should have been at the collision
            glm::vec3 this_collision_corner;
            glm::vec3 other_collision_corner;
            locate_overlap_corners(
               this_BB_ptr, &this_collision_corner,
               other_BB_ptr, &other_collision_corner,
               overlap_vector);

            float time = calculate_time_from_collision_until_now(
               this_physics_ptr->m_linear_velocity,
               other_physics_ptr->m_linear_velocity,
               overlap_vector);

            // back off this' bounding box corner from now (overlap) until the exact point of collision
            glm::vec3 this_backoff_vector = this_physics_ptr->m_linear_velocity * time;
            glm::vec3 this_corner_at_collision = this_collision_corner + this_backoff_vector;

            // back off the other bounding box's corner in the same way
            glm::vec3 other_backoff_vector = other_physics_ptr->m_linear_velocity * time;
            glm::vec3 other_corner_at_collision = other_collision_corner + other_backoff_vector;

            // the collision point will be fudged and will be at the center of the plane of the colliding corners
            glm::vec3 collision_point = (this_corner_at_collision + other_corner_at_collision) * 0.5f;

            if (collision_is_a_thing)
            {
               static int counter = 0;
               cout << "collision! - " << counter++ << " - <" << collision_point.x << ", " << collision_point.y << ", " << collision_point.z << ">" << endl;
            }

            // check for collision
            // if collision, then 
            //    locate collision point (requires velocity and bounding box overlap)
            //    calculate force of impact (requires mass and velocity)
            //    add force vector and point-of-impact vector to physics component, which will handle what happens in the next frame
            // TODO: Be a bit more sophisticated and calculate backoff vectors for the entities after they overlap, then fudge a backoff transform,
            // then calculate where the entities would be at after the force is applied.  This approach is based on the idea that overlapping
            // bounding boxes have already collided, so the entities should already be moving in the other direction.  PROBLEM: A spinning entity
            // will change the size of the bounding box, even if it isn't moving, so my backoff vectors might be problematic.
         }
      }
   }

   int Collision_Handler::add_collision_data(const Entities::AABB_Component *bounding_box_ptr, const Entities::Physics_Component *physics_ptr)
   {
      MY_ASSERT(bounding_box_ptr != 0);
      MY_ASSERT(physics_ptr != 0);

      if (0 == m_available_indices.size())
      {
         // don't even bother looking for a spot
         // TODO:: ??change to exceptions instead??
         return -1;
      }

      // get the next open index
      uint index = m_available_indices[0];
      m_available_indices.pop_front();

      // insert the pointers
      m_bounding_box_ptr_arr[index] = bounding_box_ptr;
      m_physics_ptr_arr[index] = physics_ptr;

      // return the index as an ID
      return index;
   }

   void Collision_Handler::remove_collision_data(const int &collision_data_ID)
   {
      MY_ASSERT(collision_data_ID < m_MAX_ENTITIES);

      // nullify the pointers
      m_bounding_box_ptr_arr[collision_data_ID] = 0;
      m_physics_ptr_arr[collision_data_ID] = 0;

      // add the index back into the pool of available indices
      m_available_indices.push_back(collision_data_ID);
   }

   float Collision_Handler::calculate_time_from_collision_until_now(
      const glm::vec3 &this_velocity, 
      const glm::vec3 &other_velocity, 
      const glm::vec3 &overlap_vector) const
   {
      // Note: This is a velocity-distance problem:
      // velocity_B * time - velocity_A * time = overlap
      // 
      // Solving for time results in a vector that tells me the time that it would take for the 
      // combined velocities on an axis to cover the overlap distance on that axis.  The smallest 
      // time tells me the axis that was responsible for the final overlap that caused the collision.
      // Any two axes can be overlapping and not cause a collision, but when the third axis 
      // collides, then I have a collision.  The most recent axis collision tells me the delta 
      // time from the collision until this frame.
      // Also, it is not simply a matter of the smallest overlap distance because a high combined 
      // velocity could cover a large overlap distance in a short time.

      glm::vec3 time = overlap_vector / (other_velocity - this_velocity);

      // find the minimum time
      float abs_time_x = fabs(time.x);
      float abs_time_y = fabs(time.y);
      float abs_time_z = fabs(time.z);
      if (abs_time_x <= abs_time_y)
      {
         if (abs_time_x <= abs_time_z)
         {
            // X is smallest time, so return the time with the positive/negative sign
            return time.x;
         }
         else  // read, "time Z < time X"
         {
            return time.z;
         }
      }
      else  // read, "time Y < time X"
      {
         if (abs_time_y <= abs_time_z)
         {
            // Y is smallest time, so return the time with the positive/negative sign
            return time.y;
         }
         else  // read, "time Z < time Y"
         {
            return time.z;
         }
      }
   }

   void Collision_Handler::locate_overlap_corners(
      const Entities::AABB_Component *this_BB_ptr, glm::vec3 *put_this_collision_corner_here,
      const Entities::AABB_Component *other_BB_ptr, glm::vec3 *put_other_collision_corner_here,
      const glm::vec3 &overlap_vector) const
   {
      if (overlap_vector.x < 0)
      {
         // collision is on the -X side of this AABB
         put_this_collision_corner_here->x = this_BB_ptr->m_curr_min_X;
         put_other_collision_corner_here->x = other_BB_ptr->m_curr_max_X;
      }
      else
      {
         // collision is on the +X side of this AABB
         put_this_collision_corner_here->x = this_BB_ptr->m_curr_max_X;
         put_other_collision_corner_here->x = other_BB_ptr->m_curr_min_X;
      }

      if (overlap_vector.y < 0)
      {
         // collision is on the -Y side of this AABB
         put_this_collision_corner_here->y = this_BB_ptr->m_curr_min_Y;
         put_other_collision_corner_here->y = other_BB_ptr->m_curr_max_Y;
      }
      else
      {
         // collision is on the +Y side of this AABB
         put_this_collision_corner_here->y = this_BB_ptr->m_curr_max_Y;
         put_other_collision_corner_here->y = other_BB_ptr->m_curr_min_Y;
      }

      if (overlap_vector.z < 0)
      {
         // collision is on the -Z side of this AABB
         put_this_collision_corner_here->z = this_BB_ptr->m_curr_min_Z;
         put_other_collision_corner_here->z = other_BB_ptr->m_curr_max_Z;
      }
      else
      {
         // collision is on the +Z side of this AABB
         put_this_collision_corner_here->z = this_BB_ptr->m_curr_max_Z;
         put_other_collision_corner_here->z = other_BB_ptr->m_curr_min_Z;
      }
   }
}