#include "Border_Handler_Component.h"

// for getting at the physics component
#include <Entities\Components\Physics_Component.h>
#include <Entities\Entity.h>

namespace Entities
{
   Border_Handler_Component::Border_Handler_Component()
      : m_border_verts_arr(0),
      m_border_vert_normals_arr(0),
      m_physics_sibling_ptr(0),
      m_prev_entity_position(0)  // initialize vector to all 0s
   {

   }

   bool Border_Handler_Component::initialize()
   {
      m_physics_sibling_ptr = m_parent_entity_ptr->get_component_ptr<Physics_Component>();

      if (0 == m_physics_sibling_ptr)
      {
         // no physics sibling, so border control can't happen
         return false;
      }

      // reset pointers to 0 (this is in case "initialize" is called again after initial setup)
      m_border_verts_arr = 0;
      m_border_vert_normals_arr = 0;
      m_physics_sibling_ptr = 0;

      return true;
   }

   void Border_Handler_Component::update()
   {
      // ??handle 3D collisions??
   }

   void Border_Handler_Component::set_data(const glm::vec3 *border_verts_positions_arr, const glm::vec3 *border_verts_normals_ptr, uint num_boundary_verts)
   {
      m_border_verts_arr = border_verts_positions_arr;
      m_border_vert_normals_arr = border_verts_normals_ptr;
      m_NUM_BOUNDARY_VERTS = num_boundary_verts;
   }
}

