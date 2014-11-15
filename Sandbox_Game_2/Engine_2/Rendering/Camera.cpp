#include "Camera.h"

#include <Utilities\Include_Helpers\GLM_Mat_Transform.h>
#include <Utilities\Include_Helpers\Default_Vectors.h>
#include <Entities\Entity.h>
#include <Utilities\My_Assert.h>
//#include <Utilities\Quaternion_Helper.h>

namespace Rendering
{
   Camera::Camera()
      : 
      //m_position(0.0f),
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
      //return glm::translate(glm::mat4_cast(m_view_orientation), -m_position);

      //glm::fdualquat dq = m_where_and_which_way;
      //dq.real = glm::conjugate(dq.real);
      //dq.dual *= -1.0f;
      Math::F_Dual_Quat dq = m_where_and_which_way;
      dq.m_real = dq.m_real.conjugate();
      dq.m_dual *= -1.0f;
      
      //return dq.to_mat4();
      //return m_where_and_which_way.to_mat4();
      return glm::mat4();

      //return Utilities::Quaternion_Helper::dual_quat_to_mat4(dq);
      //return Utilities::Quaternion_Helper::dual_quat_to_mat4(m_where_and_which_way);
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
         glm::rotate(glm::mat4(), 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));
         //m_position = (m_follow_this_entity_ptr->m_position);
         //m_view_orientation = m_follow_this_entity_ptr->m_base_orientation_quat;

         m_where_and_which_way = m_follow_this_entity_ptr->m_where_and_which_way;
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
      return glm::vec3();
      //return glm::vec3(
      //   m_where_and_which_way.dual.x,
      //   m_where_and_which_way.dual.y,
      //   m_where_and_which_way.dual.z);
   }

   void Camera::set_entity_to_follow(Entities::Entity *entity_ptr)
   {
      MY_ASSERT(entity_ptr != 0);
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