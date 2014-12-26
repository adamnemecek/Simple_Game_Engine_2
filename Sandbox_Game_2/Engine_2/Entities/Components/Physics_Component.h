#ifndef ENGINE_PHYSICS_COMPONENT_H
#define ENGINE_PHYSICS_COMPONENT_H

#include <Entities\Game_Component.h>
#include <glm\vec3.hpp>
#include <Utilities\Typedefs.h>

// forward declaration of friend class
namespace Collision_Detection
{
   class Collision_Handler;
}

namespace Entities
{
   class __declspec(dllexport) Physics_Component : public Game_Component
   {
   public:
      bool initialize();
      // default shutdown()
      void update();
      
      // an immediate force vector will likely induce both linear and angular acceleration, so we need 
      // to know where the force vector was applied relative to the center of mass
      void add_immediate_force_vector(const glm::vec3 &force_vec, const glm::vec3 &lever_arm);

      // a sustained force vector will be considered to act on the entire object, so we will assume
      // that it acts only on the center of mass and we therefore do not need a lever arm vector
      void add_sustained_force_vector(const glm::vec3 &force_vec);

      // this is the "bounce off surface" function
      void reflect_linear_velocity_around_vector(const glm::vec3 &reflection_axis);

      void collision_with_entity_at_location(const Physics_Component &other);
      
   private:
      // the collision handler class will need access to the entity's mass and velocity to calculate
      // the point of impact and the force exerted at that point, and then this physics class will 
      // react to the applied force
      friend class Collision_Detection::Collision_Handler;

      // private helper constants
      static const uint m_MAX_IMMEDIATE_FORCE_VECTORS = 20;
      static const uint m_MAX_SUSTAINED_FORCE_VECTORS = 5;

      // immediate force vectors are things like impacts or dynamic forces that are frequently changing
      glm::vec3 m_immediate_force_vectors_this_frame[m_MAX_IMMEDIATE_FORCE_VECTORS];
      uint m_current_immediate_force_vector_index;

      // sustained force vectors are things like gravity
      glm::vec3 m_sustained_force_vectors_this_frame[m_MAX_SUSTAINED_FORCE_VECTORS];
      uint m_current_sustained_force_vector_index;

      // these are the primary two things that this class was designed to track; everything else
      // is helper data
      glm::vec3 m_linear_velocity;
      float m_angular_velocity;
   };
}

#endif