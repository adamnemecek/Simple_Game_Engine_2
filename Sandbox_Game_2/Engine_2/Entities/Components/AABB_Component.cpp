#include "AABB_Component.h"

#include <Entities\Entity.h>
#include <Shapes\Geometry.h>
#include <Shapes\My_Vertex.h>
#include <Utilities\My_Assert.h>
#include <Utilities\Quaternion_Helper.h>


namespace Entities
{
   bool AABB_Component::initialize()
   {
      memset(m_model_space_corners, 0, 8 * sizeof(glm::vec3));

      return true;
   }

   void AABB_Component::update()
   {
      float min_x = 0.0f;
      float max_x = 0.0f;
      float min_y = 0.0f;
      float max_y = 0.0f;
      float min_z = 0.0f;
      float max_z = 0.0f;

      // transform the box corners and recalculate the box's extremities
      for (uint corner_index = 0; corner_index < 8; corner_index++)
      {
         glm::fdualquat point_dq = Utilities::Quaternion_Helper::make_dual_quat_from_point(m_model_space_corners[corner_index]);

         glm::fdualquat &entity_transform = m_parent_entity_ptr->m_where_and_which_way;
         glm::fdualquat new_point_dq = entity_transform * point_dq * Utilities::Quaternion_Helper::dual_quat_conjugate(entity_transform);

         // check if the point is part of any extremities
         glm::fquat &position = new_point_dq.dual;
         if (position.x < min_x) { min_x = position.x; }
         else if (position.x > max_x) { max_x = position.x; }

         if (position.y < min_y) { min_y = position.y; }
         else if (position.y > max_x) { max_y = position.y; }

         if (position.z < min_z) { min_z = position.z; }
         else if (position.z > max_z) { max_z = position.z; }
      }

      m_model_space_corners[BOX_CORNERS::MAX_X_MAX_Y_MAX_Z] = glm::vec3(max_x, max_y, max_z);
      m_model_space_corners[BOX_CORNERS::MAX_X_MAX_Y_MIN_Z] = glm::vec3(max_x, max_y, min_z);
      m_model_space_corners[BOX_CORNERS::MAX_X_MIN_Y_MAX_Z] = glm::vec3(max_x, min_y, max_z);
      m_model_space_corners[BOX_CORNERS::MAX_X_MIN_Y_MIN_Z] = glm::vec3(max_x, min_y, min_z);
      m_model_space_corners[BOX_CORNERS::MIN_X_MAX_Y_MAX_Z] = glm::vec3(min_x, max_y, max_z);
      m_model_space_corners[BOX_CORNERS::MIN_X_MAX_Y_MIN_Z] = glm::vec3(min_x, max_y, min_z);
      m_model_space_corners[BOX_CORNERS::MIN_X_MIN_Y_MAX_Z] = glm::vec3(min_x, min_y, max_z);
      m_model_space_corners[BOX_CORNERS::MIN_X_MIN_Y_MIN_Z] = glm::vec3(min_x, min_y, min_z);
   }

   void AABB_Component::calculate_initial_bounds(const Shapes::Geometry *box_this)
   {
      MY_ASSERT(box_this != 0);

      float min_x = 0.0f;
      float max_x = 0.0f;
      float min_y = 0.0f;
      float max_y = 0.0f;
      float min_z = 0.0f;
      float max_z = 0.0f;

      // go through all the vertices and find the min and max in all axes
      Shapes::My_Vertex *vertex_data_ptr = (box_this->m_shape_data).m_verts;
      uint num_verts = (box_this->m_shape_data).m_num_verts;
      for (uint vert_counter = 0; vert_counter < num_verts; vert_counter++)
      {
         glm::vec3 &this_vert = vertex_data_ptr[vert_counter].position;

         if (this_vert.x < min_x) { min_x = this_vert.x; }
         else if (this_vert.x > max_x) { max_x = this_vert.x; }

         if (this_vert.y < min_y) { min_y = this_vert.y; }
         else if (this_vert.y > max_x) { max_y = this_vert.y; }

         if (this_vert.z < min_z) { min_z = this_vert.z; }
         else if (this_vert.z > max_z) { max_z = this_vert.z; }
      }

      m_model_space_corners[BOX_CORNERS::MAX_X_MAX_Y_MAX_Z] = glm::vec3(max_x, max_y, max_z);
      m_model_space_corners[BOX_CORNERS::MAX_X_MAX_Y_MIN_Z] = glm::vec3(max_x, max_y, min_z);
      m_model_space_corners[BOX_CORNERS::MAX_X_MIN_Y_MAX_Z] = glm::vec3(max_x, min_y, max_z);
      m_model_space_corners[BOX_CORNERS::MAX_X_MIN_Y_MIN_Z] = glm::vec3(max_x, min_y, min_z);
      m_model_space_corners[BOX_CORNERS::MIN_X_MAX_Y_MAX_Z] = glm::vec3(min_x, max_y, max_z);
      m_model_space_corners[BOX_CORNERS::MIN_X_MAX_Y_MIN_Z] = glm::vec3(min_x, max_y, min_z);
      m_model_space_corners[BOX_CORNERS::MIN_X_MIN_Y_MAX_Z] = glm::vec3(min_x, min_y, max_z);
      m_model_space_corners[BOX_CORNERS::MIN_X_MIN_Y_MIN_Z] = glm::vec3(min_x, min_y, min_z);
   }
}