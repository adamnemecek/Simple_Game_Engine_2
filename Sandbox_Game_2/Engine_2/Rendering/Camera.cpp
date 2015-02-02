#include "Camera.h"

#include <Utilities\Include_Helpers\Default_Vectors.h>
#include <Entities\Entity.h>
#include <Utilities\My_Assert.h>


namespace Rendering
{
   Camera::Camera()
      :
      m_prev_mouse_position(0.0f),
      m_camera_move_speed(0.3f),
      m_follow_this_entity_ptr(0),
      m_position_relative_to_entity(0.0f, +1.0f, -10.0f)
   {
   }

   glm::mat4 Camera::get_world_to_view_matrix() const
   {
      // why take the conjugate?  I don't know
      return m_where_and_which_way.conjugate().to_mat4();
   }

   //void Camera::mouse_update(const glm::vec2 &new_mouse_position)
   //{
   //   glm::vec2 mouse_delta = new_mouse_position - m_prev_mouse_position;

   //   // attempt to prevent "camera jump" when the mouse leaves the window and re-enters by only
   //   // performing this transform if the movement is under a limit
   //   // Note: Be mindful of whether you are using degrees or radians when you included the GLM matrix transform header.
   //   if (glm::length(mouse_delta) < 50.0f)
   //   {
   //      const float ROTATION_SENSITIVITY = 0.5f;
   //      const float PI_DEG_CONVERSION = (2.0f * 3.14159f) / 360.0f;
   //      
   //      // treat the mouse delta as a change in degrees
   //      float rotate_angle_rad_x = -mouse_delta.x * PI_DEG_CONVERSION;
   //      float rotate_angle_rad_y = -mouse_delta.y * PI_DEG_CONVERSION;

   //      // take the cross product of the current view direction and the world up vector to get the 
   //      // strafe vector, then rotate around Y, then rotate around the strafe vector
   //      // ??good candidate for quaternion computation??
   //      m_strafe_direction = glm::cross(m_view_direction, m_WORLD_UP_VECTORtor);
   //      glm::mat4 rotator_mat =
   //         glm::rotate(glm::mat4(), rotate_angle_rad_x * ROTATION_SENSITIVITY, m_WORLD_UP_VECTORtor) *
   //         glm::rotate(glm::mat4(), rotate_angle_rad_y * ROTATION_SENSITIVITY, m_strafe_direction);

   //      // cast the mat4 down to a mat3 rather than casting twice casting the view direction to a 
   //      // vec4 and back to a vec3
   //      m_view_direction = glm::mat3(rotator_mat) * m_view_direction;
   //   }

   //   m_prev_mouse_position = new_mouse_position;
   //}

   void Camera::update()
   {
      // only do something if you have an entity to follow
      if (m_follow_this_entity_ptr != 0)
      {
         glm::vec3 world_space_backoff_vector(
            Utilities::Default_Vectors::WORLD_LEFT_VECTOR * m_position_relative_to_entity.x +
            Utilities::Default_Vectors::WORLD_UP_VECTOR * m_position_relative_to_entity.y +
            Utilities::Default_Vectors::WORLD_FORWARD_VECTOR * m_position_relative_to_entity.z);
         Math::F_Dual_Quat backoff = Math::F_Dual_Quat::generate_translate_only(world_space_backoff_vector);
         m_where_and_which_way = m_follow_this_entity_ptr->m_where_and_which_way * backoff;
      }
   }

   void Camera::set_entity_to_follow(Entities::Entity *entity_ptr)
   {
      MY_ASSERT(entity_ptr != 0);
      m_follow_this_entity_ptr = entity_ptr;
   }

}