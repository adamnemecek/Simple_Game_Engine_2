#ifndef ENGINE_BORDER_HANDLER_COMPONENT_H
#define ENGINE_BORDER_HANDLER_COMPONENT_H

#include <Entities\Game_Component.h>
#include <Misc\Typedefs.h>
#include <glm\glm.hpp>

namespace Entities
{
   class Physics_Component;

   class __declspec(dllexport) Border_Handler_Component : public Game_Component
   {
   public:
      Border_Handler_Component();

      bool initialize();
      // default shutdown()
      void update();

      // we need vertex data for the boundaries, but we are not using exceptions and we can't 
      // modify initialize() so that the containing entity can blindly initialize all its components,
      // so we'll make this function
      void set_data(const glm::vec3 *border_verts_positions_arr, const glm::vec3 *border_verts_normals_ptr, uint num_boundary_verts);

   private:
      // this value cannot be constant because it takes a user-defined value from set_data(),
      // but type it in all caps to STRONGLY SUGGEST THAT IT SHOULD NOT BE CHANGED ELSEWHERE
      uint m_NUM_BOUNDARY_VERTS;

      const glm::vec3 *m_border_verts_ptr;
      const glm::vec3 *m_border_vert_normals_ptr;

      Physics_Component *m_physics_sibling_ptr;
      glm::vec3 m_prev_entity_position;
   };
}


#endif