#include "Physics_Component.h"

// for rotating and moving things
#include <glm\glm.hpp>
using glm::vec3;

// for the time part of accerlation and velocity calculations
#include <Timing\Game_Clock.h>
using Timing::Game_Clock;

#include <cassert>
#include <memory>
using std::memset;

namespace Entities
{
   bool Physics_Component::initialize()
   {
      m_current_force_vector_index = 0;
      m_linear_velocity = vec3();
      m_linear_acceleration = vec3();
      m_angular_velocity = vec3();
      m_angular_acceleration = vec3();

      // don't bother clearing the force vectors array because it will be overwritten on every frame

      return true;
   }

   void Physics_Component::update()
   {
      // get delta time for the last frame
      float delta_time = Game_Clock::get_instance().get_delta_time_last_frame();

      // add up the force vectors
      glm::vec3 net_force_vector(0);
      for (uint index = 0; index < m_current_force_vector_index; index++)
      {
         net_force_vector += m_force_vectors[index];
      }

      // reset the "current force vector index", but don't bother overwriting values 
      // back to 0 because they will be overwritten anyway on the next "add immediate 
      // force vector" call
      m_current_force_vector_index = 0;

      // calculate torque and rotate the entity
      m_angular_velocity += m_angular_acceleration * delta_time;

      // calculate the linear acceleraiton and move it in the new direction
   }

   void Physics_Component::add_immediate_force_vector(const glm::vec3 &force_vec)
   {
      // ??remove assertion??
      assert(m_current_force_vector_index != m_MAX_FORCE_VECTORS);
      m_force_vectors[m_current_force_vector_index++] = force_vec;
   }

   void Physics_Component::reflect_linear_velocity_around_vector(const glm::vec3 &reflection_axis)
   {
      // ??add in this code??
   }

}