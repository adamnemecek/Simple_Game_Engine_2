#include "Physics_Component.h"

// for rotating and moving things
#include <glm\vec3.hpp>

// for the time part of accerlation and velocity calculations
#include <Timing\Game_Clock.h>

#include <Utilities\My_Assert.h>
#include <memory>

#include <Entities\Entity.h>

#include <Utilities\Include_Helpers\Default_Vectors.h>

#include <glm\gtc\matrix_transform.hpp>
#include <glm\detail\func_geometric.hpp>

#include <Math\F_Dual_Quat.h>

namespace Entities
{
   bool Physics_Component::initialize()
   {
      m_current_immediate_force_vector_index = 0;
      m_current_sustained_force_vector_index = 0;
      m_linear_velocity = glm::vec3();
      m_angular_velocity = 0.0f;

      // don't bother clearing the force vectors array because it will be overwritten on every frame

      return true;
   }

   void Physics_Component::update()
   {
      // get delta time for the last frame
      float delta_time = Timing::Game_Clock::get_instance().get_delta_time_last_frame();

      // add up the immediate and sustained force vectors
      glm::vec3 net_force_vector;
      for (uint index = 0; index < m_current_immediate_force_vector_index; index++)
      {
         net_force_vector += m_immediate_force_vectors_this_frame[index];
      }
      for (uint index = 0; index < m_current_sustained_force_vector_index; index++)
      {
         net_force_vector += m_sustained_force_vectors_this_frame[index];
      }

      // TODO: ??add DEBUG flags to the printer helper or just delete the uses when you're done debugging??

      // reset the "current force vector index", but don't bother overwriting values 
      // back to 0 because they will be overwritten anyway on the next "add immediate 
      // force vector" call
      m_current_immediate_force_vector_index = 0;



      // make crude physics code just to start things moving and testing your collision boxes, and then do t with fancy calculations later

      static const float ENTITY_MASS = 5.0f;  // TODO: make this an adjustable property of the entity, perhaps with a 'material' component or property

      // calculate torque and rotate the entity
      float angular_accel = 0.0f; //glm::length(net_force_vector) / 2.0f;
      m_angular_velocity += angular_accel * delta_time;
      float rotation_angle = m_angular_velocity * delta_time;

      // calculate the linear acceleration and move it in the new direction
      glm::vec3 linear_accel = net_force_vector;
      m_linear_velocity += net_force_vector * delta_time;
      glm::vec3 position_change = m_linear_velocity * delta_time;

      Math::F_Dual_Quat prev_state(m_parent_entity_ptr->m_where_and_which_way);
      Math::F_Dual_Quat delta_state = Math::F_Dual_Quat::generate_rotate_then_translate(Utilities::Default_Vectors::WORLD_UP_VECTOR, rotation_angle, position_change);

      m_parent_entity_ptr->m_where_and_which_way = delta_state * prev_state;
   }

   void Physics_Component::add_mass(const float additional_mass)
   {
      m_mass += additional_mass;
   }

   void Physics_Component::remove_mass(const float mass_to_go_bye_bye)
   {
      m_mass -= mass_to_go_bye_bye;
      if (m_mass < 0.0f)
      {
         m_mass = 0.0f;
      }
   }

   void Physics_Component::add_immediate_force_vector(const glm::vec3 &force_vec, const glm::vec3 &lever_arm)
   {
      // TODO: Change the "immediate force vectors" array to two arrays: a torque vector array and a 
      // linear-force-on-center-of-mass array
      // TODO: ??remove assertion and make into a try-throw check or maybe just refuse to add too many??
      MY_ASSERT(m_current_immediate_force_vector_index != m_MAX_IMMEDIATE_FORCE_VECTORS);
      m_immediate_force_vectors_this_frame[m_current_immediate_force_vector_index++] = force_vec;
   }

   void Physics_Component::add_sustained_force_vector(const glm::vec3 &force_vec)
   {
      MY_ASSERT(m_current_sustained_force_vector_index != m_MAX_SUSTAINED_FORCE_VECTORS);
      m_sustained_force_vectors_this_frame[m_current_sustained_force_vector_index++] = force_vec;
   }

   void Physics_Component::reflect_linear_velocity_around_vector(const glm::vec3 &reflection_axis)
   {
      // first vector is incident vector, the second the is the reflection axis
      // Note: Axis sign does not seem to matter.  ??why??
      m_linear_velocity = glm::reflect(m_linear_velocity, reflection_axis);
   }

}