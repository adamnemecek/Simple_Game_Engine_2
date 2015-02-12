#ifndef ENGINE_COLLISION_DETECTION_AABB_H
#define ENGINE_COLLISION_DETECTION_AABB_H


#include <glm\vec3.hpp>
#include <Entities\Game_Component.h>
#include <Utilities\Typedefs.h>

// forward declaration of friend classes
namespace Shapes
{
   class Geometry;
}

namespace Collision_Detection
{
   class Collision_Handler;
}


namespace Entities
{
   // class for an Axis-aligned Bounding Box
   class __declspec(dllexport) AABB_Component : public Game_Component
   {
   public:
      // for ensuring that pointers and array data get set to 0
      AABB_Component();

      void set_geometry(const Shapes::Geometry *box_this);

      bool initialize();
      // default shutdown
      void update();

      // figures out if/where the two are colliding and true (collision) / false (no collsion)
      // Note: This method cannot calculate the exact point of collision.  Doing that requires 
      // calculating backoff vectors for each axis, and those calculations require the entity 
      // velocity from the physics component.  That is not handled here.
      bool is_colliding_with_AABB(const AABB_Component &other_box, glm::vec3 *put_overlap_here) const;

   private:
      // the collision handler class will need access to the entity's mass and velocity to calculate
      // the point of impact and the force exerted at that point, and then this physics class will 
      // react to the applied force
      friend class Collision_Detection::Collision_Handler;
      
      // keep this pointer around in case you need to re-calculate something from the geometry
      const Shapes::Geometry *m_geometry_data_ptr;

      // a box has six faces, so just magically declare them
      enum BOX_CORNERS
      {
         RIGHT_UPPER_FRONT,
         RIGHT_UPPER_BACK,
         LEFT_UPPER_FRONT,
         LEFT_UPPER_BACK,
         RIGHT_LOWER_FRONT,
         RIGHT_LOWER_BACK,
         LEFT_LOWER_FRONT,
         LEFT_LOWER_BACK,
         NUM_CORNERS
      };

      // use vec3s for the default face centers and floats for the current extremities of the box
      // Note: I chose this strategy because min/max values on each axis are easy to compute and
      // compare, but I can only translate vectors, so I create the default face vectors as vec3s
      // that can be easily translated with the entity's latest position and orientation on every
      // frame (the update() method).
      glm::vec3 m_default_box_corners[BOX_CORNERS::NUM_CORNERS];
      float m_curr_min_X;
      float m_curr_max_X;
      float m_curr_min_Y;
      float m_curr_max_Y;
      float m_curr_min_Z;
      float m_curr_max_Z;

      // helper functions are private
      void recalculate_all_min_max_values(const glm::vec3 *curr_box_corners_arr, const uint max_vectors);
      bool calculate_overlap_on_axis(const float other_max_to_this_min, const float this_max_to_other_min, float *put_overlap_here) const;
   };
}

#endif
