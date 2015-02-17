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
   // starts with the following:
   // - 0 kg of mass
   // - 0 m/s linear velocity on all axes
   // - 0 radians/s angular velocity
   class __declspec(dllexport) Physics_Component : public Game_Component
   {
   public:
      // for setting starting values to 0
      // Note: The initialize() method formerly held this responsibility, but I ran into
      // trouble when I was implementing the scene loading.  Basically, I didn't have the 
      // luxury (without breaking cohesion) of making the physics component, adding it to
      // the entity, initializing it, and THEN adding a sustained force vector.  The 
      // initialize() method was originally designed as a way to start up a Game_Component
      // object AFTER it had been added to an entity, plus it could do double duty in 
      // resetting the component (I don't know when I would use this, but Jamie King said
      // that it might be useful).  I will now reject that last design idea and only use 
      // initialize() for things that happen AFTER the object is added to its entity.
      Physics_Component();

      // default initialize();
      // default shutdown()
      void update();

      // self-explanatory
      void add_mass(const float additional_mass);

      // removes mass from the entity
      // Note: Will not allow the mass to drop below 0.
      void remove_mass(const float mass_to_go_bye_bye);
      
      // an immediate force vector will likely induce both linear and angular acceleration, so we need 
      // to know where the force vector was applied relative to the center of mass
      void add_immediate_force_vector(const glm::vec3 &force_vec, const glm::vec3 &lever_arm);

      // a sustained force vector will be considered to act on the entire object, so we will assume
      // that it acts only on the center of mass and we therefore do not need a lever arm vector
      void add_sustained_force_vector(const glm::vec3 &force_vec);

      // this is the "bounce off surface" function
      void reflect_linear_velocity_around_vector(const glm::vec3 &reflection_axis);

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

      // these are the primary things that this class was designed to track; everything else
      // is helper data
      glm::vec3 m_linear_velocity;
      float m_angular_velocity;
      float m_mass;
   };
}

#endif