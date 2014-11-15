#ifndef ENGINE_PHYSICS_COMPONENT_H
#define ENGINE_PHYSICS_COMPONENT_H

#include <Entities\Game_Component.h>
#include <glm\vec3.hpp>
#include <Utilities\Typedefs.h>

namespace Entities
{
   class __declspec(dllexport) Physics_Component : public Game_Component
   {
   public:
      bool initialize();
      // default shutdown()
      void update();
      
      void add_immediate_force_vector(const glm::vec3 &force_vec);
      void add_sustained_force_vector(const glm::vec3 &force_vec);

      // this is the "bounce off surface" function
      void reflect_linear_velocity_around_vector(const glm::vec3 &reflection_axis);
      
   private:
      static const uint m_MAX_IMMEDIATE_FORCE_VECTORS = 20;
      static const uint m_MAX_SUSTAINED_FORCE_VECTORS = 5;

      // immediate force vectors are things like impacts or dynamic forces that are frequently changing
      glm::vec3 m_immediate_force_vectors_this_frame[m_MAX_IMMEDIATE_FORCE_VECTORS];
      uint m_current_immediate_force_vector_index;

      // sustained force vectors are things like gravity
      glm::vec3 m_sustained_force_vectors_this_frame[m_MAX_SUSTAINED_FORCE_VECTORS];
      uint m_current_sustained_force_vector_index;

      glm::vec3 m_linear_velocity;
      glm::vec3 m_linear_acceleration;

      glm::vec3 m_angular_rotation_vector;
      float m_angular_velocity;
      float m_angular_acceleration;
   };
}

#endif