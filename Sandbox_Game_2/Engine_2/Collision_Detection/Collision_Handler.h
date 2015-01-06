#ifndef ENGINE_AABB_COLLISION_DETECTION_H
#define ENGINE_AABB_COLLISION_DETECTION_H

#include <Utilities\Typedefs.h>
#include <deque>
#include <glm\vec3.hpp>

// forward declarations
namespace Entities
{
   class AABB_Component;
   class Physics_Component;
}

namespace Collision_Detection
{
   // a go-between class for collision detection and resolution
   // Note: Individual AABBs handle the collision detection, and the physics components
   // handle forces and moments, but collision requires both.  Collison resolution 
   // requires figuring out the exact point of collision and the force exerted by each 
   // entity on the other at that point.  Calculating these requires overlap data from 
   // the bounding box components and velocity and mass from the physics component. 
   class __declspec(dllexport) Collision_Handler
   {
   public:
      static Collision_Handler &get_instance();

      bool initialize();

      // checks for collisions between every box and every other box exactly once
      // Note: Update physics and bounding box components BEFORE calling this method.
      void update();

      // adds some information to a list of collidable objects
      // Note: This function returns the index of the entity as an ID, or -1 if the list
      // is full.
      // Note: It is the responsibility of the caller to make sure that both of these correspond 
      // to the same entity.
      int add_collision_data(const Entities::AABB_Component *bounding_box_ptr, const Entities::Physics_Component *physics_ptr);

      // removes the specified entity's collision data from the collection
      // Note: Contains an assertion to check that the ID is within bounds, but
      // this check will only run if the DEBUG preprocessor is defined.
      void remove_collision_data(const int &collision_data_ID);

   private:
      static const uint m_MAX_ENTITIES = 10;

      // Note: I went with this approach instead of a simple "number of current entities" counter because
      // the deque approach allowed for much easier finding of available indices.  I would have had to 
      // search through the entire pointer array for a null pointer if I was using the counter, and the 
      // counter is redundant if I am using a deque, so I went with the vector of available indices 
      // instead of the counter for number of current entities.  Note that these two values are additive
      // inverses.
      std::deque<uint> m_available_indices;
      const Entities::AABB_Component *m_bounding_box_ptr_arr[m_MAX_ENTITIES];
      const Entities::Physics_Component *m_physics_ptr_arr[m_MAX_ENTITIES];

      // enforce singleton-ness
      Collision_Handler() {}
      Collision_Handler(const Collision_Handler &);
      Collision_Handler &operator=(const Collision_Handler &);

      
      // helper functions are private

      // returns the delta time from the most recent axis overlap (the one that caused the collision)
      // Note: This function assumes that a collision already occured.
      // Note: This is a general helper function, and as such the "time" is mostly a mathematical 
      // placeholder with the units of seconds.  The "time" value can be negative.  Positive/negative
      // signs are useful in determining the correct backoff vector.
      float calculate_time_from_collision_until_now(
         const glm::vec3 &this_velocity, 
         const glm::vec3 &other_velocity, 
         const glm::vec3 &overlap_vector) const;

      void locate_overlap_corners(
         const Entities::AABB_Component *this_BB_ptr, glm::vec3 *put_this_collision_corner_here,
         const Entities::AABB_Component *other_BB_ptr, glm::vec3 *put_other_collision_corner_here,
         const glm::vec3 &overlap_vector) const;
   };
}

#endif