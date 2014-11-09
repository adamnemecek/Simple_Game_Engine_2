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
      // for ensuring that pointers and array data get set to 0
      AABB_Component();

      void set_geometry(const Shapes::Geometry *box_this);

      bool initialize();
      // default shutdown
      void update();

      // figures out where the two are coliding and returns the point of collision
      // Note: If there is no collision, it returns 0 on all axes.
      glm::vec3 is_colliding_with_AABB(const AABB_Component &other_box);

   private:
      // keep this pointer around in case you need to re-calculate something from the geometry
      const Shapes::Geometry *m_geometry_data_ptr;

      // a box has six faces, so just magically declare them
      enum BOX_FACES
      {
         TOP_CENTER = 0,   // max Y
         BOTTOM_CENTER,    // min Y
         LEFT_CENTER,      // min X
         RIGHT_CENTER,     // max X
         FRONT_CENTER,     // min Z
         BACK_CENTER,      // MAX Z
         NUM_FACES
      };

      glm::vec3 m_face_centers[BOX_FACES::NUM_FACES];

      // a helper function that needs access to private data members
      void set_faces(
         float min_x, float max_x,
         float min_y, float max_y,
         float min_z, float max_z);


      //// a box has 8 corners, so just magically declare them
      //glm::vec3 m_center;
      //float m_half_length;    // center to front / center to back
      //float m_half_width;     // center to left / center to right
      //float m_half_height;    // center to top / center to bottom


      //glm::vec3 m_model_space_corners[8];

      //enum BOX_CORNERS
      //{
      //   MAX_X_MAX_Y_MAX_Z = 0,
      //   MAX_X_MAX_Y_MIN_Z,
      //   MAX_X_MIN_Y_MAX_Z,
      //   MAX_X_MIN_Y_MIN_Z,
      //   MIN_X_MAX_Y_MAX_Z,
      //   MIN_X_MAX_Y_MIN_Z,
      //   MIN_X_MIN_Y_MAX_Z,
      //   MIN_X_MIN_Y_MIN_Z
      //};
   };
}

#endif
