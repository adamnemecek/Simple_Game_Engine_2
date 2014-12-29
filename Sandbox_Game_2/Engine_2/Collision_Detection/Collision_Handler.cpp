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
      for (uint this_BB_index = 0; this_BB_index < num_current_collision_IDs; this_BB_index++)
      {
         for (uint other_BB_index = this_BB_index + 1; other_BB_index < num_current_collision_IDs; other_BB_index++)
         {
            //m_bounding_boxes[this_BB_index]->is_colliding_with_AABB(*m_bounding_boxes[other_BB_index]);
            glm::vec3 overlap_vector;
            bool collision_is_a_thing = m_bounding_box_ptr_arr[this_BB_index]->is_colliding_with_AABB(*m_bounding_box_ptr_arr[other_BB_index], &overlap_vector);

            if (collision_is_a_thing)
            {
               static int counter = 0;
               cout << "collision! - " << counter++ << " - <" << overlap_vector.x << ", " << overlap_vector.y << ", " << overlap_vector.z << ">" << endl;
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

}