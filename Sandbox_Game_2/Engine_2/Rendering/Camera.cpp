#include "Camera.h"

#include <Utilities\Include_Helper_GLM_Mat_Transform.h>
#include <Utilities\Include_Helper_Default_Vectors.h>
#include <Entities\Entity.h>
#include <cassert>
#include <glm\gtc\quaternion.hpp>

namespace Rendering
{
   Camera::Camera()
      : //m_position(0.0f),
      m_position(8.0f, 6.0f, 0.0f),
      m_view_direction(-m_position),
      //m_view_orientation(1.0f, 0.0f, 0.0f, 0.0f),
      //m_view_direction(Utilities::Default_Vectors::WORLD_FORWARD),
      //m_strafe_direction(glm::cross(m_view_direction, Utilities::WORLD_UP_VECTOR)),
      m_prev_mouse_position(0.0f),
      m_camera_move_speed(0.3f),
      m_follow_this_entity_ptr(0),
      m_entity_backoff_distance(0.0f)
   {
   }

   glm::mat4 Camera::get_world_to_view_matrix() const
   {
      return glm::lookAt(m_position, m_position + m_view_direction, Utilities::Default_Vectors::WORLD_UP_VECTOR);

      //??normalize??
      //return glm::mat4_cast(m_view_orientation);
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
         //m_view_direction = glm::normalize(m_follow_this_entity_ptr->m_base_orientation);

         //m_view_direction = glm::mat3(glm::mat4_cast(m_follow_this_entity_ptr->m_base_orientation_quat)) * Utilities::Default_Vectors::WORLD_FORWARD;
         //glm::vec3 backoff_vector = m_view_direction * m_entity_backoff_distance;

         //m_position = (m_follow_this_entity_ptr->m_position) - backoff_vector;

         //m_view_orientation = m_follow_this_entity_ptr->m_base_orientation_quat;
         //m_position = m_follow_this_entity_ptr->m_position;
      }
   }

   glm::vec3 Camera::get_strafe_vector()
   {
      //return m_strafe_direction;
      return glm::vec3(0.0f);
   }

   glm::vec3 Camera::get_forward_vector()
   {
      //return m_view_direction;
      return glm::vec3(0.0f);
   }

   glm::vec3 Camera::get_position()
   {
      return m_position;
   }

   void Camera::set_entity_to_follow(Entities::Entity *entity_ptr)
   {
      assert(entity_ptr != 0);
      m_follow_this_entity_ptr = entity_ptr;
   }

   //void Camera::move_forward()
   //{
   //   m_position += m_view_direction * m_camera_move_speed;
   //}

   //void Camera::move_back()
   //{
   //   m_position -= m_view_direction * m_camera_move_speed;
   //}

   //void Camera::strafe_left()
   //{
   //   m_position -= m_strafe_direction * m_camera_move_speed;
   //}

   //void Camera::strafe_right()
   //{
   //   m_position += m_strafe_direction * m_camera_move_speed;
   //}

   //void Camera::move_up()
   //{
   //   m_position += m_WORLD_UP_VECTORtor * m_camera_move_speed;
   //}

   //void Camera::move_down()
   //{
   //   m_position -= m_WORLD_UP_VECTORtor * m_camera_move_speed;
   //}


}