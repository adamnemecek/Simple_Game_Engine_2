#ifndef ENGINE_AABB_COLLISION_DETECTION_H
#define ENGINE_AABB_COLLISION_DETECTION_H

#include <Entities\Components\AABB_Component.h>
#include <Utilities\Typedefs.h>

namespace Collision_Detection
{
   class __declspec(dllexport) AABB_Collision_Detector
   {
   public:
      AABB_Collision_Detector &get_instance();

      bool initialize();

      // checks for collisions between every box and every other box exactly once
      // Note: Update all AABB components BEFORE calling this method.
      // Note: The individual boxes handle the actual collision.  This class is a
      // support and organizer class.
      void update();

      // returns the index of the AABB as an ID, or -1 if no space available
      // Note: The argument is not const because there was a compiler error that I
      // couldn't resolve between a dereferenced const ptr and the const reference 
      // that the AABB component's collision method required.
      // TODO: figure out how to make it const
      int add_AABB(Entities::AABB_Component *AABB_ptr);

      // removes the specified AABB from the collection
      // Note: Contains an assertion to check that the ID is within bounds, but
      // this check will only run if the DEBUG preprocessor is defined.
      void remove_AABB(const int &AABB_ID);

   private:
      static const uint m_MAX_BOUNDING_BOXES = 10;
      uint m_num_bounding_boxes;
      Entities::AABB_Component *m_bounding_boxes[m_MAX_BOUNDING_BOXES];

      // enforce singleton-ness
      AABB_Collision_Detector();
      AABB_Collision_Detector(const AABB_Collision_Detector &);
      AABB_Collision_Detector &operator=(const AABB_Collision_Detector &);
   };
}

#endif
