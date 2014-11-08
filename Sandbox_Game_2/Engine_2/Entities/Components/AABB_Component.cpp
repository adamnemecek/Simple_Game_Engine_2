#include "AABB_Component.h"

#include <Entities\Entity.h>
#include <Shapes\Geometry.h>
#include <Shapes\My_Vertex.h>
#include <Utilities\My_Assert.h>
#include <Utilities\Quaternion_Helper.h>

// a helper namespace for helper things
namespace
{
   enum BOX_FACES
   {
      TOP_CENTER = 0,   // max Y
      BOTTOM_CENTER,    // min Y
      LEFT_CENTER,      // min X
      RGIHT_CENTER,     // max X
      FRONT_CENTER,     // min Z
      BACK_CENTER,      // MAX Z
   };

   static const uint NUM_FACES = 6;
}

namespace Entities
{
   bool AABB_Component::initialize()
   {
      //memset(m_model_space_corners, 0, 8 * sizeof(glm::vec3));

      return true;
   }

   void AABB_Component::update()
   {
      // give the min/max values some initial values
      float min_x = m_center.x;
      float max_x = m_center.x;
      float min_y = m_center.y;
      float max_y = m_center.y;
      float min_z = m_center.z;
      float max_z = m_center.z;

      glm::fdualquat face_centers[NUM_FACES];


      // make vectors out of the center planes of each side of the box, transform them by
      // the entity's latest transform, transform the center, then recalculate the length, 
      // width, and height of the box

      // vectors out of the previous center points of the 6 box faces
      face_centers[BOX_FACES::TOP_CENTER] = Utilities::Quaternion_Helper::make_dual_quat_from_point(m_center + glm::vec3(0.0f, +m_half_height, 0.0f));
      face_centers[BOX_FACES::BOTTOM_CENTER] = Utilities::Quaternion_Helper::make_dual_quat_from_point(m_center + glm::vec3(0.0f, -m_half_height, 0.0f));

      face_centers[BOX_FACES::LEFT_CENTER] = Utilities::Quaternion_Helper::make_dual_quat_from_point(m_center + glm::vec3(-m_half_width, 0.0f, 0.0f));
      face_centers[BOX_FACES::RGIHT_CENTER] = Utilities::Quaternion_Helper::make_dual_quat_from_point(m_center + glm::vec3(+m_half_width, 0.0f, 0.0f));

      face_centers[BOX_FACES::FRONT_CENTER] = Utilities::Quaternion_Helper::make_dual_quat_from_point(m_center + glm::vec3(0.0f, 0.0f, -m_half_length));
      face_centers[BOX_FACES::BACK_CENTER] = Utilities::Quaternion_Helper::make_dual_quat_from_point(m_center + glm::vec3(0.0f, 0.0f, +m_half_length));

      // finally, make a vector out of the center
      glm::fdualquat center = Utilities::Quaternion_Helper::make_dual_quat_from_point(m_center);

      // transform all the previous center points of the box's faces and recalculate the etremities
      // Note: The transforms are declared const to emphasize that they are not to be changed once created.
      const glm::fdualquat &entity_transform = m_parent_entity_ptr->m_where_and_which_way;
      const glm::fdualquat entity_transform_conjugate = Utilities::Quaternion_Helper::dual_quat_conjugate(entity_transform);

      for (uint face_center_index = 0; face_center_index < NUM_FACES; face_center_index++)
      {
         glm::fdualquat &dq_ref = face_centers[face_center_index];
         dq_ref = entity_transform * dq_ref * entity_transform_conjugate;

         // check if the point is part of any extremities
         glm::fquat &position = dq_ref.dual;
         if (position.x < min_x) { min_x = position.x; }
         else if (position.x > max_x) { max_x = position.x; }

         if (position.y < min_y) { min_y = position.y; }
         else if (position.y > max_x) { max_y = position.y; }

         if (position.z < min_z) { min_z = position.z; }
         else if (position.z > max_z) { max_z = position.z; }
      }

      // transform the center, then recalculate the half length, width, and height
      center = entity_transform * center * entity_transform_conjugate;
      m_center.x = center.dual.x;
      m_center.y = center.dual.y;
      m_center.z = center.dual.z;

      m_half_width = (max_x - min_x) / 2.0f;
      m_half_length = (max_z - min_z) / 2.0f;
      m_half_height = (max_y - min_y) / 2.0f;


      // recalculate the box's extremities




      //// transform the box corners and recalculate the box's extremities
      //for (uint corner_index = 0; corner_index < 8; corner_index++)
      //{
      //   glm::fdualquat point_dq = Utilities::Quaternion_Helper::make_dual_quat_from_point(m_model_space_corners[corner_index]);

      //   glm::fdualquat &entity_transform = m_parent_entity_ptr->m_where_and_which_way;
      //   glm::fdualquat new_point_dq = entity_transform * point_dq * Utilities::Quaternion_Helper::dual_quat_conjugate(entity_transform);

      //   // check if the point is part of any extremities
      //   glm::fquat &position = new_point_dq.dual;
      //   if (position.x < min_x) { min_x = position.x; }
      //   else if (position.x > max_x) { max_x = position.x; }

      //   if (position.y < min_y) { min_y = position.y; }
      //   else if (position.y > max_x) { max_y = position.y; }

      //   if (position.z < min_z) { min_z = position.z; }
      //   else if (position.z > max_z) { max_z = position.z; }
      //}

      //m_model_space_corners[BOX_CORNERS::MAX_X_MAX_Y_MAX_Z] = glm::vec3(max_x, max_y, max_z);
      //m_model_space_corners[BOX_CORNERS::MAX_X_MAX_Y_MIN_Z] = glm::vec3(max_x, max_y, min_z);
      //m_model_space_corners[BOX_CORNERS::MAX_X_MIN_Y_MAX_Z] = glm::vec3(max_x, min_y, max_z);
      //m_model_space_corners[BOX_CORNERS::MAX_X_MIN_Y_MIN_Z] = glm::vec3(max_x, min_y, min_z);
      //m_model_space_corners[BOX_CORNERS::MIN_X_MAX_Y_MAX_Z] = glm::vec3(min_x, max_y, max_z);
      //m_model_space_corners[BOX_CORNERS::MIN_X_MAX_Y_MIN_Z] = glm::vec3(min_x, max_y, min_z);
      //m_model_space_corners[BOX_CORNERS::MIN_X_MIN_Y_MAX_Z] = glm::vec3(min_x, min_y, max_z);
      //m_model_space_corners[BOX_CORNERS::MIN_X_MIN_Y_MIN_Z] = glm::vec3(min_x, min_y, min_z);
   }

   void AABB_Component::calculate_initial_bounds(const Shapes::Geometry *box_this)
   {
      MY_ASSERT(box_this != 0);

      glm::vec3 initial_vert = (box_this->m_shape_data).m_verts->position;
      float min_x = initial_vert.x;
      float max_x = initial_vert.x;
      float min_y = initial_vert.y;
      float max_y = initial_vert.y;
      float min_z = initial_vert.z;
      float max_z = initial_vert.z;

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

      m_center = glm::vec3((min_x + max_x) / 2.0f, (min_y + max_y) / 2.0f, (min_z + max_z) / 2.0f);
      m_half_width = (max_x - min_x) / 2.0f;
      m_half_length = (max_z - min_z) / 2.0f;
      m_half_height = (max_y - min_y) / 2.0f;
   }

   glm::vec3 AABB_Component::is_colliding_with_AABB(const AABB_Component &other_box)
   {
      float this_top = m_center.y + m_half_height;
      float this_bottom = m_center.y - m_half_height;
      float this_left = m_center.x - m_half_width;
      float this_right = m_center.x + m_half_width;
      float this_front = m_center.z - m_half_length;
      float this_back = m_center.z + m_half_length;

      float other_top = other_box.m_center.y + other_box.m_half_height;
      float other_bottom = other_box.m_center.y - other_box.m_half_height;
      float other_left = other_box.m_center.x - other_box.m_half_width;
      float other_right = other_box.m_center.x + other_box.m_half_width;
      float other_front = other_box.m_center.z - other_box.m_half_length;
      float other_back = other_box.m_center.z + other_box.m_half_length;

      bool potential_right_collision = (this_right > other_left) && (this_right < other_right);
      bool potential_left_collision = (this_left < other_right) && (this_left > other_left);
      bool potential_top_collision = (this_top > other_bottom) && (this_top < other_top);
      bool potential_bottom_collision = (this_bottom < other_top) && (this_bottom > other_bottom);

      if (potential_right_collision && potential_left_collision)
      {

      }

      return glm::vec3();
   }
}