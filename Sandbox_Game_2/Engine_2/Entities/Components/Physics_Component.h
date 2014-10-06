#ifndef ENGINE_PHYSICS_COMPONENT_H
#define ENGINE_PHYSICS_COMPONENT_H

#include <Entities\Game_Component.h>
#include <glm\glm.hpp>
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

      // this is the "bounce off surface" function
      void reflect_linear_velocity_around_vector(const glm::vec3 &reflection_axis);
      
   private:
      static const uint m_MAX_FORCE_VECTORS = 10;
      glm::vec3 m_force_vectors[m_MAX_FORCE_VECTORS];
      uint m_current_force_vector_index;

      glm::vec3 m_linear_velocity;
      glm::vec3 m_linear_acceleration;
      glm::vec3 m_angular_velocity;
      glm::vec3 m_angular_acceleration;
   };
}

#endif