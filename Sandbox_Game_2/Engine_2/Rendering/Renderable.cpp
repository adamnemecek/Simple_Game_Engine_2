#include "Renderable.h"

#include <Entities\Entity.h>
#include <Shapes\Geometry.h>
#include <Shapes\Shape_Data.h>

namespace Rendering
{
   GLuint Renderable::get_VAO_ID()
   {
      m_geometry_ptr->get_VAO_ID();

      return m_geometry_ptr->get_VAO_ID();
   }

   glm::mat4 Renderable::get_model_to_world_matrix()
   {
      return m_entity_ptr->m_where_and_which_way.to_mat4();
   }

   const std::vector<Shapes::Index_Meta_Data>& Renderable::get_geometry_index_meta_data_collection()
   {
      return m_geometry_ptr->get_index_meta_data_list();
   }

   void Renderable::set_geometry(const Shapes::Geometry *geometry_ptr)
   {
      m_geometry_ptr = geometry_ptr;
   }

   void Renderable::set_entity(const Entities::Entity *entity_ptr)
   {
      m_entity_ptr = entity_ptr;
   }
}