#ifndef ENGINE_CAMERA_H
#define ENGINE_CAMERA_H

#include <glm\vec2.hpp>
#include <glm\mat4x4.hpp>
#include <Math\F_Dual_Quat.h>

namespace Entities
{
   class Entity;
}

namespace Rendering
{
   class __declspec(dllexport) Camera
   {
   public:
      Camera();
      glm::mat4 get_world_to_view_matrix() const;
      //void mouse_update(const glm::vec2& new_mouse_position);

      // updates the camera to follow the assigned entity (provided one exists)
      void update();

      // use this to assign an entity to follow
      // Note: If no entity is assigned, then the camera will remain at the origin and not move.
      void set_entity_to_follow(Entities::Entity *entity_ptr);


   private:
      Math::F_Dual_Quat m_where_and_which_way;

      glm::vec2 m_prev_mouse_position;

      Entities::Entity *m_follow_this_entity_ptr;
      float m_entity_backoff_distance;

      float m_camera_move_speed;
   };
}


#endif
