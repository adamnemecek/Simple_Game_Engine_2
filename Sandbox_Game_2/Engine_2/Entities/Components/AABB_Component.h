#ifndef ENGINE_COLLISION_DETECTION_AABB_H
#define ENGINE_COLLISION_DETECTION_AABB_H


#include <glm\vec3.hpp>
#include <Entities\Game_Component.h>

namespace Shapes
{
   class Geometry;
}

namespace Entities
{
   // class for an Axis-aligned Bounding Box
   class __declspec(dllexport) AABB_Component : public Game_Component
   {
   public:
      bool initialize();
      // default shutdown
      void update();

      void calculate_initial_bounds(const Shapes::Geometry *box_this);

   private:
      // a box has 8 corners, so just magically declare them
      glm::vec3 m_model_space_corners[8];

      enum BOX_CORNERS
      {
         MAX_X_MAX_Y_MAX_Z = 0,
         MAX_X_MAX_Y_MIN_Z,
         MAX_X_MIN_Y_MAX_Z,
         MAX_X_MIN_Y_MIN_Z,
         MIN_X_MAX_Y_MAX_Z,
         MIN_X_MAX_Y_MIN_Z,
         MIN_X_MIN_Y_MAX_Z,
         MIN_X_MIN_Y_MIN_Z
      };
   };
}

#endif
