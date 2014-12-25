#include "Collision_Handler.h"

#include <Utilities\My_Assert.h>
#include <memory.h>

namespace Collision_Detection
{
   Collision_Handler &Collision_Handler::get_instance()
   {
      static Collision_Handler instance;

      return instance;
   }

   bool Collision_Handler::initialize()
   {
      // erase all pointer data that might be in the box
      memset(m_bounding_boxes, 0, sizeof(Entities::AABB_Component *) * m_MAX_BOUNDING_BOXES);

      m_num_bounding_boxes = 0;

      return true;
   }

   void Collision_Handler::update()
   {
      // check each box against every other box once
      // Note: The following nested for-loop conditions will only run if there are at least 2 boxes.
      // If there is only 1 box, then the inner loop's end condition will be triggered before it 
      // even starts, and if there are 0 boxes then the outer loop's end condition will be triggered
      // before it even starts.
      for (uint this_BB_index = 0; this_BB_index < m_num_bounding_boxes; this_BB_index++)
      {
         for (uint other_BB_index = this_BB_index + 1; other_BB_index < m_num_bounding_boxes; other_BB_index++)
         {
            m_bounding_boxes[this_BB_index]->is_colliding_with_AABB(*m_bounding_boxes[other_BB_index]);
         }
      }
   }


   int Collision_Handler::add_AABB(Entities::AABB_Component *AABB_ptr)
   {
      MY_ASSERT(AABB_ptr != 0);

      if (m_num_bounding_boxes == m_MAX_BOUNDING_BOXES)
      {
         // don't even bother looking for a spot
         return -1;
      }

      // find the next open spot
      int index = 0;
      for (index = 0; index < m_MAX_BOUNDING_BOXES; index++)
      {
         if (0 == m_bounding_boxes[index])
         {
            m_bounding_boxes[index] = AABB_ptr;
            m_num_bounding_boxes++;
            break;
         }
      }

      return index;
   }

   void Collision_Handler::remove_AABB(const int &AABB_ID)
   {
      MY_ASSERT(AABB_ID < m_MAX_BOUNDING_BOXES);

      m_bounding_boxes[AABB_ID] = 0;
      m_num_bounding_boxes--;
   }

}