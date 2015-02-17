#ifndef ENGINE_CAMERA_H
#define ENGINE_CAMERA_H

#include <glm\vec2.hpp>
#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>
#include <Math\F_Dual_Quat.h>
#include <string>

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
      void set_entity_to_follow(const Entities::Entity *entity_ptr);

      bool set_id(const std::string& new_id);
      std::string get_id();

   private:
      std::string m_id;

      Math::F_Dual_Quat m_where_and_which_way;

      glm::vec2 m_prev_mouse_position;

      const Entities::Entity *m_follow_this_entity_ptr;

      // this vector is used to reposition the camera relative to the entity being followed
      // TODO: ??make this a dual quaternion as well??
      glm::vec3 m_position_relative_to_entity;

      float m_camera_move_speed;
   };
}


#endif
