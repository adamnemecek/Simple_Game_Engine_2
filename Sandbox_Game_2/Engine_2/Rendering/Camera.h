#ifndef ENGINE_CAMERA_H
#define ENGINE_CAMERA_H

#include <glm\vec3.hpp>
#include <glm\vec2.hpp>
#include <glm\mat4x4.hpp>
#include <Utilities\Include_Helper_GLM_Quaternion.h>

namespace Entities
{
   class Entity;
}

namespace Rendering
{
   class Camera
   {
   public:
      Camera();
      glm::mat4 get_world_to_view_matrix() const;
      //void mouse_update(const glm::vec2& new_mouse_position);

      // updates the camera to follow the assigned entity (provided one exists)
      void update();

      // these getters are useful for manipulating entities relative to the camera
      glm::vec3 get_strafe_vector();
      glm::vec3 get_forward_vector();
      glm::vec3 get_position();

      // use this to assign an entity to follow
      // Note: If no entity is assigned, then the camera will remain at the origin and not move.
      void set_entity_to_follow(Entities::Entity *entity_ptr);

      // basic camera controls
      //void move_forward();
      //void move_back();
      //void strafe_left();
      //void strafe_right();
      //void move_up();
      //void move_down();


   private:
      glm::vec3 m_position;

      glm::fquat m_view_orientation;
      glm::vec2 m_prev_mouse_position;

      Entities::Entity *m_follow_this_entity_ptr;
      float m_entity_backoff_distance;

      float m_camera_move_speed;
   };
}


#endif
