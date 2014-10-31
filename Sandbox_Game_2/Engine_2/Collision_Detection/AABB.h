#ifndef ENGINE_COLLISION_DETECTION_AABB_H
#define ENGINE_COLLISION_DETECTION_AABB_H


#include <glm\gtx\dual_quaternion.hpp>
#include <glm\vec3.hpp>

namespace Shapes
{
   class Geometry;
}

namespace Collision_Detection
{
   // class for an Axis-aligned Bounding Box
   class __declspec(dllexport) AABB
   {
   public:
      AABB(const Shapes::Geometry *box_this);

      void recalculate(const glm::fdualquat &new_orientation);

   private:
      // contains information on the box's location and orientation
      glm::vec3 m_position;
   };
}

#endif
