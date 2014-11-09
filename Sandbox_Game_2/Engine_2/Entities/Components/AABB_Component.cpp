#include "AABB_Component.h"

#include <Entities\Entity.h>
#include <Shapes\Geometry.h>
#include <Shapes\My_Vertex.h>
#include <Utilities\Quaternion_Helper.h>
#include <memory.h>
#include <Utilities\My_Assert.h>

#include <Utilities\Printer_Helper.h>

#include <iostream>
using std::cout;
using std::endl;


// a helper namespace for non-class helper things
namespace
{
   inline float avg(float min, float max)
   {
      return (max + min) / 2.0f;
   }

   inline float half_distance(float min, float max)
   {
      return (max - min) / 2.0f;
   }

}

namespace Entities
{
   AABB_Component::AABB_Component()
      : m_geometry_data_ptr(0)
   {
   }

   void AABB_Component::set_geometry(const Shapes::Geometry *box_this)
   {
      m_geometry_data_ptr = box_this;
   }

   bool AABB_Component::initialize()
   {
      // can't initialize without any geometry
      MY_ASSERT(m_geometry_data_ptr != 0);

      recalculate_all_min_max_values(
         &(m_geometry_data_ptr->m_shape_data.m_verts->position), 
         m_geometry_data_ptr->m_shape_data.m_num_verts,
         sizeof(Shapes::My_Vertex) / sizeof(glm::vec3));

      // set the default vectors so that they can be transformed on the next update
      m_default_face_centers[BOX_FACES::LEFT_CENTER] = glm::vec3(m_curr_min_x, 0.0f, 0.0f);
      m_default_face_centers[BOX_FACES::RIGHT_CENTER] = glm::vec3(m_curr_max_x, 0.0f, 0.0f);
      m_default_face_centers[BOX_FACES::TOP_CENTER] = glm::vec3(0.0f, m_curr_max_y, 0.0f);
      m_default_face_centers[BOX_FACES::BOTTOM_CENTER] = glm::vec3(0.0f, m_curr_min_y, 0.0f);
      m_default_face_centers[BOX_FACES::FRONT_CENTER] = glm::vec3(0.0f, 0.0f, m_curr_min_z);
      m_default_face_centers[BOX_FACES::BACK_CENTER] = glm::vec3(0.0f, 0.0f, m_curr_max_z);

      //// give the min/max values initial data that is within the boundaries of the geometry
      //glm::vec3 initial_vert = m_geometry_data_ptr->m_shape_data.m_verts->position;
      //m_curr_min_x = initial_vert.x;
      //m_curr_max_x = initial_vert.x;
      //m_curr_min_y = initial_vert.y;
      //m_curr_max_y = initial_vert.y;
      //m_curr_min_z = initial_vert.z;
      //m_curr_max_z = initial_vert.z;

      //// go through all the vertices and find the min and max in all axes
      //Shapes::My_Vertex *vertex_data_ptr = m_geometry_data_ptr->m_shape_data.m_verts;
      //uint num_verts = m_geometry_data_ptr->m_shape_data.m_num_verts;

      //cout << "vertex addr [0] = " << (uint)(&vertex_data_ptr[0]) << "; position addr [0] = " << (uint)(&vertex_data_ptr[0].position) << endl;
      //cout << "vertex addr [1] = " << (uint)(&vertex_data_ptr[1]) << "; position addr [1] = " << (uint)(&vertex_data_ptr[1].position) << endl;

      //glm::vec3 *position_ptr = &vertex_data_ptr[0].position;
      //cout << "size of my_vertex = " << sizeof(Shapes::My_Vertex) / sizeof(glm::vec3) << endl;
      //cout << "position ptr [0] = " << (uint)(&position_ptr[0]) << "position ptr [1] = " << (uint)(&position_ptr[3]) << endl;

      //for (uint vert_counter = 0; vert_counter < num_verts; vert_counter++)
      //{
      //   glm::vec3 &this_vert = vertex_data_ptr[vert_counter].position;


      //   if (this_vert.x < m_curr_min_x) { m_curr_min_x = this_vert.x; }
      //   else if (this_vert.x > m_curr_max_x) { m_curr_max_x = this_vert.x; }

      //   if (this_vert.y < m_curr_min_y) { m_curr_min_y = this_vert.y; }
      //   else if (this_vert.y > m_curr_max_y) { m_curr_max_y = this_vert.y; }

      //   if (this_vert.z < m_curr_min_z) { m_curr_min_z = this_vert.z; }
      //   else if (this_vert.z > m_curr_max_z) { m_curr_max_z = this_vert.z; }
      //}


      return true;
   }

   void AABB_Component::update()
   {
      //static uint update_count = 0;

      //if (update_count > 10)
      //{
      //   glm::normalize(m_parent_entity_ptr->m_where_and_which_way);
      //}
      //update_count++;

      glm::vec3 curr_face_centers[BOX_FACES::NUM_FACES];

      // transform the face center points
      glm::fdualquat &transform = m_parent_entity_ptr->m_where_and_which_way;
      glm::fdualquat transform_conjugate = Utilities::Quaternion_Helper::dual_quat_conjugate(transform);
      for (uint face_index = 0; face_index < BOX_FACES::NUM_FACES; face_index++)
      {
         curr_face_centers[face_index] = Utilities::Quaternion_Helper::dual_quat_translate_point(
            m_default_face_centers[face_index],
            transform,
            transform_conjugate);
      }

      // data is closely packed, so index_stride is 1
      recalculate_all_min_max_values(curr_face_centers, BOX_FACES::NUM_FACES, 1);



      //// give the new min/max values some initial values
      //float min_x = m_face_centers[0].x;
      //float max_x = m_face_centers[0].x;
      //float min_y = m_face_centers[0].y;
      //float max_y = m_face_centers[0].y;
      //float min_z = m_face_centers[0].z;
      //float max_z = m_face_centers[0].z;


      //// recalculate the extremeties agains
      //for (uint face_index = 0; face_index < BOX_FACES::NUM_FACES; face_index++)
      //{
      //   glm::vec3 &this_vert = m_face_centers[face_index];

      //   if (this_vert.x < min_x) { min_x = this_vert.x; }
      //   else if (this_vert.x > max_x) { max_x = this_vert.x; }

      //   if (this_vert.y < min_y) { min_y = this_vert.y; }
      //   else if (this_vert.y > max_y) { max_y = this_vert.y; }

      //   if (this_vert.z < min_z) { min_z = this_vert.z; }
      //   else if (this_vert.z > max_z) { max_z = this_vert.z; }
      //}

      //if (min_z < +0.3f && min_z > -0.3f)
      //{
      //   cout << " - something_happened";
      //}


      //// re-calculate the face points
      //set_faces(min_x, max_x, min_y, max_y, min_z, max_z);



      // procedure:
      // - Need to generate 

      // make vectors out of the center planes of each side of the box, transform them by
      // the entity's latest transform, transform the center, then recalculate the length, 
      // width, and height of the box
      //glm::fdualquat face_centers[BOX_FACES::NUM_FACES];

      //// vectors out of the previous center points of the 6 box faces
      //face_centers[BOX_FACES::TOP_CENTER] = Utilities::Quaternion_Helper::dual_quat_from_point(m_center + glm::vec3(0.0f, +m_half_height, 0.0f));
      //face_centers[BOX_FACES::BOTTOM_CENTER] = Utilities::Quaternion_Helper::dual_quat_from_point(m_center + glm::vec3(0.0f, -m_half_height, 0.0f));

      //face_centers[BOX_FACES::LEFT_CENTER] = Utilities::Quaternion_Helper::dual_quat_from_point(m_center + glm::vec3(-m_half_width, 0.0f, 0.0f));
      //face_centers[BOX_FACES::RGIHT_CENTER] = Utilities::Quaternion_Helper::dual_quat_from_point(m_center + glm::vec3(+m_half_width, 0.0f, 0.0f));

      //face_centers[BOX_FACES::FRONT_CENTER] = Utilities::Quaternion_Helper::dual_quat_from_point(m_center + glm::vec3(0.0f, 0.0f, -m_half_length));
      //face_centers[BOX_FACES::BACK_CENTER] = Utilities::Quaternion_Helper::dual_quat_from_point(m_center + glm::vec3(0.0f, 0.0f, +m_half_length));

      //// finally, make a vector out of the center
      //glm::fdualquat center = Utilities::Quaternion_Helper::dual_quat_from_point(m_center);





      //// transform all the previous center points of the box's faces and recalculate the etremities
      //// Note: The transforms are declared const to emphasize that they are not to be changed once created.
      //const glm::fdualquat &entity_transform = m_parent_entity_ptr->m_where_and_which_way;
      //const glm::fdualquat entity_transform_conjugate = Utilities::Quaternion_Helper::dual_quat_conjugate(entity_transform);

      //for (uint face_center_index = 0; face_center_index < BOX_FACES::NUM_FACES; face_center_index++)
      //{
      //   glm::fdualquat &dq_ref = face_centers[face_center_index];
      //   dq_ref = entity_transform * dq_ref * entity_transform_conjugate;

      //   // check if the point is part of any extremities
      //   glm::fquat &position = dq_ref.dual;
      //   if (position.x < min_x) { min_x = position.x; }
      //   else if (position.x > max_x) { max_x = position.x; }

      //   if (position.y < min_y) { min_y = position.y; }
      //   else if (position.y > max_x) { max_y = position.y; }

      //   if (position.z < min_z) { min_z = position.z; }
      //   else if (position.z > max_z) { max_z = position.z; }
      //}

      //// transform the center, then recalculate the half length, width, and height
      //center = entity_transform * center * entity_transform_conjugate;
      //m_center.x = center.dual.x;
      //m_center.y = center.dual.y;
      //m_center.z = center.dual.z;

      //m_half_width = (max_x - min_x) / 2.0f;
      //m_half_length = (max_z - min_z) / 2.0f;
      //m_half_height = (max_y - min_y) / 2.0f;


      // recalculate the box's extremities




      //// transform the box corners and recalculate the box's extremities
      //for (uint corner_index = 0; corner_index < 8; corner_index++)
      //{
      //   glm::fdualquat point_dq = Utilities::Quaternion_Helper::dual_quat_from_point(m_model_space_corners[corner_index]);

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


   glm::vec3 AABB_Component::is_colliding_with_AABB(const AABB_Component &other_box)
   {
      //float this_top = m_center.y + m_half_height;
      //float this_bottom = m_center.y - m_half_height;
      //float this_left = m_center.x - m_half_width;
      //float this_right = m_center.x + m_half_width;
      //float this_front = m_center.z - m_half_length;
      //float this_back = m_center.z + m_half_length;

      //float other_top = other_box.m_center.y + other_box.m_half_height;
      //float other_bottom = other_box.m_center.y - other_box.m_half_height;
      //float other_left = other_box.m_center.x - other_box.m_half_width;
      //float other_right = other_box.m_center.x + other_box.m_half_width;
      //float other_front = other_box.m_center.z - other_box.m_half_length;
      //float other_back = other_box.m_center.z + other_box.m_half_length;


      //float this_left = m_face_centers[BOX_FACES::LEFT_CENTER].x;
      //float this_right = m_face_centers[BOX_FACES::RIGHT_CENTER].x;
      //float this_top = m_face_centers[BOX_FACES::TOP_CENTER].y;
      //float this_bottom = m_face_centers[BOX_FACES::BOTTOM_CENTER].y;
      //float this_front = m_face_centers[BOX_FACES::FRONT_CENTER].z;
      //float this_back = m_face_centers[BOX_FACES::BACK_CENTER].z;

      //float other_left = other_box.m_face_centers[BOX_FACES::LEFT_CENTER].x;
      //float other_right = other_box.m_face_centers[BOX_FACES::RIGHT_CENTER].x;
      //float other_top = other_box.m_face_centers[BOX_FACES::TOP_CENTER].y;
      //float other_bottom = other_box.m_face_centers[BOX_FACES::BOTTOM_CENTER].y;
      //float other_front = other_box.m_face_centers[BOX_FACES::FRONT_CENTER].z;
      //float other_back = other_box.m_face_centers[BOX_FACES::BACK_CENTER].z;

      //bool X_overlap = (this_left < other_right) && (this_right > other_left);   // "left" is negative X
      //bool Y_overlap = (this_top > other_bottom) && (this_bottom < other_top);   // "up" is positive Y
      //bool Z_overlap = (this_front < other_back) && (this_back > other_front);   // "forward" is negative Z

      // this overlap section does not make sense without imagining the boundaries of two cubes 
      // infringing on each other on a given axis, but it works
      bool X_overlap = (m_curr_min_x < other_box.m_curr_max_x) && (m_curr_max_x > other_box.m_curr_min_x);   // "left" is negative X
      bool Y_overlap = (m_curr_max_y > other_box.m_curr_min_y) && (m_curr_min_y < other_box.m_curr_max_y);   // "up" is positive Y
      bool Z_overlap = (m_curr_min_z < other_box.m_curr_max_z) && (m_curr_max_z > other_box.m_curr_min_z);   // "forward" is negative Z

      //if (this_front < +0.3f && this_front > -0.3f)
      //{
      //   cout << " - something_happened";
      //}

      if (X_overlap)
      {
         cout << " - X overlap";
      }
      
      if (Y_overlap)
      {
         cout << " - Y overlap";
      }
      else
      {
         //cout << " - no Y overlap";
      }

      if (Z_overlap)
      {
         cout << " - Z overlap";
      }

      if (X_overlap || Y_overlap || Z_overlap)
      {
         cout << endl;
      }


      return glm::vec3();
   }


   void AABB_Component::recalculate_all_min_max_values(const glm::vec3 *vec_data_source, const uint max_vectors, const uint index_stride)
   {
      // give the min/max values initial data that is within the boundaries of the data
      const glm::vec3 &initial_vert = *vec_data_source;
      m_curr_min_x = initial_vert.x;
      m_curr_max_x = initial_vert.x;
      m_curr_min_y = initial_vert.y;
      m_curr_max_y = initial_vert.y;
      m_curr_min_z = initial_vert.z;
      m_curr_max_z = initial_vert.z;

      // go through all the vectors and find the min and max in all axes
      for (uint vec_counter = 0; vec_counter < max_vectors; vec_counter++)
      {
         // calculate the index location of the vector data
         // Ex: In My_Vertex, there are position, normal, and color vec3s, so the
         // first position index (in vec3 land, not in My_Vertex land) is [0], the
         // next is [3], the next is [6], etc.
         // Ex: In m_default_face_centers, the vectors are closely packed, so the
         // first position is at index [0], the next at [1], the next at [2], etc.
         uint vec_index = vec_counter * index_stride;

         const glm::vec3 &vec_ref = vec_data_source[vec_index];

         if (vec_ref.x < m_curr_min_x) { m_curr_min_x = vec_ref.x; }
         else if (vec_ref.x > m_curr_max_x) { m_curr_max_x = vec_ref.x; }

         if (vec_ref.y < m_curr_min_y) { m_curr_min_y = vec_ref.y; }
         else if (vec_ref.y > m_curr_max_y) { m_curr_max_y = vec_ref.y; }

         if (vec_ref.z < m_curr_min_z) { m_curr_min_z = vec_ref.z; }
         else if (vec_ref.z > m_curr_max_z) { m_curr_max_z = vec_ref.z; }
      }
   }

   //void AABB_Component::set_faces(
   //   float min_x, float max_x,
   //   float min_y, float max_y,
   //   float min_z, float max_z)
   //{
   //   glm::vec3 center_point = glm::vec3(avg(min_x, max_x), avg(min_y, max_y), avg(min_z, max_z));
   //   glm::vec3 half_width = glm::vec3(half_distance(min_x, max_x), 0.0f, 0.0f);
   //   glm::vec3 half_height = glm::vec3(0.0f, half_distance(min_y, max_y), 0.0f);
   //   glm::vec3 half_length = glm::vec3(0.0f, 0.0f, half_distance(min_z, max_z));

   //   m_face_centers[BOX_FACES::LEFT_CENTER] = center_point - half_width;
   //   m_face_centers[BOX_FACES::RIGHT_CENTER] = center_point + half_width;
   //   m_face_centers[BOX_FACES::TOP_CENTER] = center_point + half_height;
   //   m_face_centers[BOX_FACES::BOTTOM_CENTER] = center_point - half_height;
   //   m_face_centers[BOX_FACES::FRONT_CENTER] = center_point - half_length;
   //   m_face_centers[BOX_FACES::BACK_CENTER] = center_point + half_length;
   //}
}