#include "Geometry.h"


namespace Shapes
{
   Geometry::Geometry(const Shape_Data& new_shape_data)
      : 
      m_shape_data(new_shape_data)  // copy constructor copies pointers and other data
   {
      initialize_attributes();
      calculate_shape_meta_data();
   }

   void Geometry::initialize_attributes()
   {
      // generate vertex array object and bind it
      glGenVertexArrays(1, &m_VAO_ID);
      glBindVertexArray(m_VAO_ID);

      // vertex buffer
      // - generate one array buffer object and bind it
      // - allocate space for the position, normal, and color data with STATIC drawing (this data doesn't change on every draw), but send no data
      // - set vertex attribute pointers 
      // - send data with glBufferSubData(...)
      glGenBuffers(1, &m_vertex_buffer_ID);
      glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer_ID);
      glBufferData(GL_ARRAY_BUFFER, m_shape_data.vertex_buffer_size(), m_shape_data.m_verts, GL_STATIC_DRAW);


      // position = 0
      // normals = 1
      // colors = 2

      // some helper variables
      uint vertex_array_index = 0;
      uint buffer_start_offset = 0;

      // position
      glEnableVertexAttribArray(vertex_array_index);
      glVertexAttribPointer(vertex_array_index, My_Vertex::FLOATS_PER_POSITION, GL_FLOAT, GL_FALSE, My_Vertex::BYTES_PER_VERTEX, (void *)buffer_start_offset);
      buffer_start_offset += My_Vertex::BYTES_PER_POSITION;
      vertex_array_index++;

      // normal
      glEnableVertexAttribArray(vertex_array_index);
      glVertexAttribPointer(vertex_array_index, My_Vertex::FLOATS_PER_NORMAL, GL_FLOAT, GL_FALSE, My_Vertex::BYTES_PER_VERTEX, (void *)buffer_start_offset);
      buffer_start_offset += My_Vertex::BYTES_PER_NORMAL;
      vertex_array_index++;

      // color
      glEnableVertexAttribArray(vertex_array_index);
      glVertexAttribPointer(vertex_array_index, My_Vertex::FLOATS_PER_COLOR, GL_FLOAT, GL_FALSE, My_Vertex::BYTES_PER_VERTEX, (void *)buffer_start_offset);
      buffer_start_offset += My_Vertex::BYTES_PER_COLOR;
      vertex_array_index++;


      // indices buffer
      // - generate one element buffer object and bind it
      // - allocate space for the indices and send the data
      glGenBuffers(1, &m_element_buffer_ID);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_element_buffer_ID);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_shape_data.index_buffer_size(), m_shape_data.m_indices, GL_STATIC_DRAW);


      // clean up bindings 
      glBindVertexArray(0);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
   }

   void Geometry::calculate_shape_meta_data()
   {
      double sum_x = 0.0;
      double sum_y = 0.0;
      double sum_z = 0.0;

      // go through the geometry data and analyze it
      for (size_t vertex_counter = 0; vertex_counter < m_shape_data.m_num_verts; vertex_counter++)
      {
         glm::vec3 &this_pos = m_shape_data.m_verts[vertex_counter].position;

         sum_x += this_pos.x;
         sum_y += this_pos.y;
         sum_z += this_pos.z;

         if (this_pos.x < m_shape_meta_data.m_min_X) { m_shape_meta_data.m_min_X = this_pos.x; }
         else if (this_pos.x > m_shape_meta_data.m_max_X) { m_shape_meta_data.m_max_X = this_pos.x; }

         if (this_pos.y < m_shape_meta_data.m_min_Y) { m_shape_meta_data.m_min_Y = this_pos.y; }
         else if (this_pos.y > m_shape_meta_data.m_max_Y) { m_shape_meta_data.m_max_Y = this_pos.y; }

         if (this_pos.z < m_shape_meta_data.m_min_Z) { m_shape_meta_data.m_min_Z = this_pos.z; }
         else if (this_pos.z > m_shape_meta_data.m_max_Z) { m_shape_meta_data.m_max_Z = this_pos.z; }
      }

      // do this division so that you keep the precision of the double
      double inverse_num_verts = 1.0 / m_shape_data.m_num_verts;

      // perform the double multiplication version of division in order to keep precision
      // of the double, THEN jam the result into the vec3's float
      // Note: This approach should result in minimal precision loss because the float 
      // cast happens after the high-precision sum and division operations.
      m_shape_meta_data.m_center_of_vertices.x = (float)(sum_x * inverse_num_verts);
      m_shape_meta_data.m_center_of_vertices.y = (float)(sum_y * inverse_num_verts);
      m_shape_meta_data.m_center_of_vertices.z = (float)(sum_z * inverse_num_verts);
   }

   const GLuint Geometry::get_VAO_ID() const
   {
      return m_VAO_ID;
   }

   const Shape_Meta_Data& Geometry::get_shape_meta_data() const
   {
      return m_shape_meta_data;
   }

   const std::vector<Index_Meta_Data>& Geometry::get_index_meta_data_list() const
   {
      return m_shape_data.m_index_meta_data;
   }

   //const std::vector<std::string>& Geometry::get_shape_parameter_list() const
   //{
   //   return Geometry::m_shape_data.m_parameters;
   //}

   const std::string Geometry::get_shape_id_string() const
   {
      const std::vector<std::string>& parameter_list = m_shape_data.m_parameters;

      std::string S = "";

      for (uint index_counter = 0; index_counter < parameter_list.size(); index_counter++)
      {
         S += parameter_list[index_counter];
      }

      return S;
   }
}