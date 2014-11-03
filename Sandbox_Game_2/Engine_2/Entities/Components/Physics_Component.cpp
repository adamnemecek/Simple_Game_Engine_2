#include "Physics_Component.h"

// for rotating and moving things
#include <glm\vec3.hpp>

// for the time part of accerlation and velocity calculations
#include <Timing\Game_Clock.h>

#include <Utilities\My_Assert.h>
#include <memory>

#include <Entities\Entity.h>

#include <Utilities\Quaternion_Helper.h>
#include <Utilities\Include_Helpers\Default_Vectors.h>

namespace Entities
{
   bool Physics_Component::initialize()
   {
      m_current_force_vector_index = 0;
      m_linear_velocity = glm::vec3();
      m_linear_acceleration = glm::vec3();
      m_angular_rotation_vector = glm::vec3();
      m_angular_velocity = 0.0f;
      m_angular_acceleration = 0.0f;

      // don't bother clearing the force vectors array because it will be overwritten on every frame

      return true;
   }

   void Physics_Component::update()
   {
      // get delta time for the last frame
      float delta_time = Timing::Game_Clock::get_instance().get_delta_time_last_frame();

      // add up the force vectors
      glm::vec3 net_force_vector;
      for (uint index = 0; index < m_current_force_vector_index; index++)
      {
         net_force_vector += m_force_vectors[index];
      }

      // reset the "current force vector index", but don't bother overwriting values 
      // back to 0 because they will be overwritten anyway on the next "add immediate 
      // force vector" call
      m_current_force_vector_index = 0;

      // make crude physics code just to start things moving and testing your collision boxes, and then do t with fancy calculations later

      static const uint ENTITY_MASS = 5.0f;  // TODO: make this an adjustable property of the entity, perhaps with a 'material' component or property

      // calculate torque and rotate the entity
      m_angular_acceleration = glm::length(net_force_vector) / 5.0f;
      m_angular_velocity += m_angular_acceleration * delta_time;
      float rotation_angle = m_angular_velocity * delta_time;
      
      glm::fquat orientation_change;
      Utilities::Quaternion_Helper::orientation_offset(Utilities::Default_Vectors::WORLD_UP_VECTOR, rotation_angle, orientation_change);

      // calculate the linear acceleraiton and move it in the new direction
      m_linear_acceleration = net_force_vector;
      m_linear_velocity += net_force_vector * delta_time;
      glm::vec3 position_change = m_linear_velocity * delta_time;

      glm::fdualquat previous_where_and_which_way = m_parent_entity_ptr->m_where_and_which_way;
      m_parent_entity_ptr->m_where_and_which_way = Utilities::Quaternion_Helper::make_dual_quat(orientation_change, position_change) * previous_where_and_which_way;
   }

   void Physics_Component::add_immediate_force_vector(const glm::vec3 &force_vec)
   {
      // ??remove assertion and make into a try-throw check or maybe just refuse to add too many??
      MY_ASSERT(m_current_force_vector_index != m_MAX_FORCE_VECTORS);
      m_force_vectors[m_current_force_vector_index++] = force_vec;
   }

   void Physics_Component::reflect_linear_velocity_around_vector(const glm::vec3 &reflection_axis)
   {
      // ??add in this code??
   }

}