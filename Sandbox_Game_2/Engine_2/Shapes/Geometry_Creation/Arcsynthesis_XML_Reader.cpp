#include "Arcsynthesis_XML_Reader.h"

#include <middleware\arcsynthesis\framework\rapidxml.hpp>
#include <middleware\arcsynthesis\framework\rapidxml_helpers.h>
#include <fstream>
#include <Shapes\Shape_Data.h>

#include <glm\vec2.hpp>
#include <glm\vec3.hpp>

#include <iostream>
using std::cout;
using std::endl;


namespace Shapes
{
   namespace Geometry_Creation
   {
      void Arcsynthesis_XML_Reader::load_from_xml_file(Shape_Data *put_shape_data_here, const std::string &file_path)
      {
         // open the file
         rapidxml::xml_document<> doc;
         std::vector<char> file_data;     // apparently this needs to be in the same scope as the xml_document<> object
         open_xml_file(&doc, file_data, file_path);

         // make sure that there is a "mesh" root node
         rapidxml::xml_node<> *root_node_ptr = doc.first_node("mesh");
         if (0 == root_node_ptr)
         {
            throw std::runtime_error("'mesh' node not found in mesh file: " + file_path);
         }

         // make sure that there is at least one "attribute" node
         // Note: There is at least two attribute nodes (attribute layout indices 0 and 2, 
         // which are used for vertex position and normal) in all the Arcsynthesis XML files.
         const rapidxml::xml_node<> *vertex_attrib_node_ptr = root_node_ptr->first_node("attribute");
         if (0 == vertex_attrib_node_ptr)
         {
            throw std::runtime_error("'mesh' node must have at least one 'attribute' child.  File: " + file_path);
         }

         // make sure that there is at least one "indices" node
         const rapidxml::xml_node<> *indices_node_ptr = root_node_ptr->first_node("indices");
         if (0 == indices_node_ptr)
         {
            throw std::runtime_error("'mesh' node must have at least one 'indices' child.  File: " + file_path);
         }

         // collect the pointers that all point to vertex attribute data
         std::vector<const rapidxml::xml_node<> *> attribute_node_ptrs;
         for (;
            vertex_attrib_node_ptr && ("attribute" == rapidxml::make_string_name(*vertex_attrib_node_ptr));
            vertex_attrib_node_ptr = rapidxml::next_element(vertex_attrib_node_ptr))
         {
            attribute_node_ptrs.push_back(vertex_attrib_node_ptr);
         }

         // load the data
         load_all_attributes(put_shape_data_here, attribute_node_ptrs);

         // collect the pointers that all point to index data
         std::vector<const rapidxml::xml_node<> *> index_node_ptrs;
         for (;
            indices_node_ptr && ("indices" == rapidxml::make_string_name(*indices_node_ptr));
            indices_node_ptr = rapidxml::next_element(indices_node_ptr))
         {
            index_node_ptrs.push_back(indices_node_ptr);
         }

         // load the data
         load_all_index_data(put_shape_data_here, index_node_ptrs);
      }

      void Arcsynthesis_XML_Reader::open_xml_file(rapidxml::xml_document<> *put_parsed_xml_document_here, std::vector<char> &file_data, const std::string &file_path)
      {
         // this function is heavily influenced by the arcsynthesis framework's Mesh 
         // class constructor, and some lines are copied verbatim except for the 
         // camel-case-to-underscore notaion change
         std::ifstream file_stream(file_path);
         if (!file_stream.is_open())
         {
            throw std::runtime_error("Could not find the mesh file: " + file_path);
         }

         //std::vector<char> file_data;

         // reserve memory
         // Note: Reserving memory ensures contiguency of memory, which allows the 
         // rpaidxml parser to run through the data with just a pointer.
         file_data.reserve(2 * 1024 * 1024);
         file_data.insert(file_data.end(), std::istreambuf_iterator<char>(file_stream),
            std::istreambuf_iterator<char>());
         file_data.push_back('\0');

         try
         {
            (*put_parsed_xml_document_here).parse<0>(&(file_data[0]));
         }
         catch (rapidxml::parse_error &e)
         {
            // this is a special rapidxml exception, so handle it here
            std::cout << file_path << ": Parse error in the mesh file." << std::endl;
            std::cout << e.what() << std::endl << e.where<char>() << std::endl;
            throw;
         }
      }

      void Arcsynthesis_XML_Reader::load_all_attributes(Shape_Data *put_shape_data_here, const std::vector<const rapidxml::xml_node<> *> attrib_node_ptrs)
      {
         // parse each attribute and store the raw float data
         std::vector<attrib_data_helper> all_attribute_data;
         for (uint attrib_data_index = 0; attrib_data_index < attrib_node_ptrs.size(); attrib_data_index++)
         {
            attrib_data_helper this_attribute_data;
            parse_attribute_node(&this_attribute_data, attrib_node_ptrs[attrib_data_index]);
            all_attribute_data.push_back(this_attribute_data);
         }

         // make sure that the number of vectors in each attribute are equal
         // Note: If they are not (suppose there are 100 position vectors and 99 normals),
         // then somthing is wrong.  It is possible for there to be different numbers of
         // floats, such as if there are vec3 positions and normals but vec2 texture
         // coordinates, but there should always be an equal number of vectors (one 
         // attribute vector for each vertex).
         uint first_attrib_vector_count = all_attribute_data[0].num_vectors;
         for (uint attrib_data_index = 1; attrib_data_index < all_attribute_data.size(); attrib_data_index++)
         {
            if (first_attrib_vector_count != all_attribute_data[attrib_data_index].num_vectors)
            {
               // bad
               throw std::exception("Arcsynthesis XML Reader, load_all_attributes(...): two of the attributes have a different number of vectors");
            }
         }

         // build a contiguous array of My_Vertex structures
         // Note: I need a contiguous array because that is how I have defined the order 
         // of positions, colors, and normals for my own geometry's vertices.  I have an 
         // order that I like to use for all my vertex attributes, and I'd like to keep 
         // it.
         // Note: The memory calculation is this:
         //  number of attributes * number of vectors per attribute * 3 floats per vector * number of bytes per float
         // Note: I assume 3 floats per vector to keep things easy.  I can truncate this 
         // down to 2 for texture coordinates, and I can chop a vec4 into a vec3.
         uint total_num_vertices = all_attribute_data[0].num_vectors;
         put_shape_data_here->m_verts = (My_Vertex *)malloc(all_attribute_data.size() * total_num_vertices * 3 * sizeof(float));
         put_shape_data_here->m_num_verts = total_num_vertices;

         // fill out the contiguous array of My_Vertex structures
         // Note: Each My_Vertex structure stores a vec3 for position, normal, and color.
         // The Arcsynthesis XML files always seem to have a vec3 for position and normal, 
         // but sometimes has a vec4 for color instead of vec3.  If it has a vec4, just 
         // chop off the last value, which is the alpha value that I am not making any
         // use of right now.
         // Note: All the attributes have the same number of vectors, so one of their 
         // vector counts is identical to the number of vertices.
         for (uint vertex_count = 0; vertex_count < total_num_vertices; vertex_count++)
         {
            // get a reference to the current vertex for easier data entry (it's easier
            // than tracking down the exact vector position, which is dependent upon the
            // structure of the My_Vertex structure anyway)
            My_Vertex &this_vert_ref = put_shape_data_here->m_verts[vertex_count];

            // go through each attribute and add its vector to the appropriate My_Vertex member
            for (uint attrib_data_index = 0; attrib_data_index < all_attribute_data.size(); attrib_data_index++)
            {
               attrib_data_helper &this_attrib_ref = all_attribute_data[attrib_data_index];

               // get a pointer to this vector's first float
               // Note: The attribute data has a pointer to float, and pointer arithmetic says
               // that adding, for example, 17 to the pointer will increase the pointer by
               // (17 * sizeof(float)) bytes.  It is therefore possible to get to the starting 
               // float of the desired vector for this attribute by adding the number of floats 
               // that have been processed so far to the original float pointer.
               float *curr_vector_f_ptr = this_attrib_ref.float_data_ptr + (vertex_count * this_attrib_ref.floats_per_vector);

               // check how many floats this vector should have and make an appropriate vector
               if (2 == this_attrib_ref.floats_per_vector)
               {
                  // a vec2
                  // Note: As far as I've seen, only texture coordinates (attribute layout index = 5) 
                  // have this.
                  glm::vec2 V(curr_vector_f_ptr[0], curr_vector_f_ptr[1]);

                  // ignore it for now because my My_Vertex structure can't handle texture coordinates yet
               }
               else if (3 == this_attrib_ref.floats_per_vector)
               {
                  // a vec3 - possibly position, normal, or color (not important yet)
                  glm::vec3 V(curr_vector_f_ptr[0], curr_vector_f_ptr[1], curr_vector_f_ptr[2]);

                  if (0 == this_attrib_ref.attribute_layout_index)
                  {
                     // position
                     this_vert_ref.position = V;
                  }
                  else if (1 == this_attrib_ref.attribute_layout_index)
                  {
                     // color
                     this_vert_ref.color = V;
                  }
                  else if (2 == this_attrib_ref.attribute_layout_index)
                  {
                     // normal
                     this_vert_ref.normal = V;
                  }
                  else
                  {
                     throw std::exception("A vec3 attribute with an attribute layout index other than 0, 1, or 2?  It's more likely than you think.");
                  }
               }
               else if (4 == this_attrib_ref.floats_per_vector)
               {
                  // a vec4
                  // Note: As far as I've seen, a vec4 is only used for color, but all the values
                  // are 1, and I'm not making use of the alpha channel anyway, so I'll just
                  // ignore the 4th value
                  glm::vec3 V(curr_vector_f_ptr[0], curr_vector_f_ptr[1], curr_vector_f_ptr[2]);

                  this_vert_ref.color = V;
               }
               else
               {
                  throw std::exception("Attribute has an unexpected number of floats per vector: ", this_attrib_ref.floats_per_vector);
               }
            }
         }

         // free the memory that was allocated for the attribute data by the parse_attribute_node(...) method
         for (uint attrib_data_index = 0; attrib_data_index < all_attribute_data.size(); attrib_data_index++)
         {
            attrib_data_helper &this_attrib_ref = all_attribute_data[attrib_data_index];
            free(this_attrib_ref.float_data_ptr);
         }

         cout << "end of load_all_attributes" << endl;
      }

      void Arcsynthesis_XML_Reader::parse_attribute_node(attrib_data_helper *put_attrib_data_here, const rapidxml::xml_node<> *node_ptr)
      {
         char *c_ptr = node_ptr->value();
         uint max_chars = node_ptr->value_size();
         std::string number_string = "";
         uint total_chars_scanned = 0;

         // collect all the vertex attribute data together
         // Note: This memory does not need to be contiguous.  The vector is very handy for building 
         // a list of unknown size though, so I'll start with it.
         std::vector<float> local_float_storage;

         while (total_chars_scanned < max_chars)
         {
            uint chars_scanned_this_number = get_next_number_string(c_ptr, &number_string);
            
            // Note: The function atof(...) returns a double, but I am using floats, so I am just
            // jamming the double into the float.
            float f = atof(number_string.c_str());

            c_ptr += chars_scanned_this_number;
            total_chars_scanned += chars_scanned_this_number;

            local_float_storage.push_back(f);
         }

         // this attribute is only one of two or three, so put this data aside 
         // IN CONTIGUOUS MEMORY for now until all the attributes can be organized
         put_attrib_data_here->float_data_ptr = (float *)malloc(local_float_storage.size() * sizeof(float));
         put_attrib_data_here->num_floats = local_float_storage.size();

         // copy the data into the allocated memory
         for (uint float_counter = 0; float_counter < local_float_storage.size(); float_counter++)
         {
            put_attrib_data_here->float_data_ptr[float_counter] = local_float_storage[float_counter];
         }

         // figure out how many floats exist per vector, and therefore how many vectors total
         std::string size_str = rapidxml::get_attrib_string(*node_ptr, "size");
         put_attrib_data_here->floats_per_vector = atoi(size_str.c_str());
         put_attrib_data_here->num_vectors = put_attrib_data_here->num_floats / put_attrib_data_here->floats_per_vector;

         // figure out what this vertex attribute is going to be used for
         // Note: 0 == position, 1 == color, 2 == normal (I think)
         std::string type_str = rapidxml::get_attrib_string(*node_ptr, "index");
         put_attrib_data_here->attribute_layout_index = atoi(type_str.c_str());
      }

      void Arcsynthesis_XML_Reader::load_all_index_data(Shape_Data * put_shape_data_here, const std::vector<const rapidxml::xml_node<> *> index_node_ptrs)
      {
         // parse each render mode command and the indices corresponding to that render mode, 
         // and store the raw data in a temporary structure
         std::vector<index_data_helper> all_index_data;
         uint total_indices = 0;
         for (uint index_data_set = 0; index_data_set < index_node_ptrs.size(); index_data_set++)
         {
            // make a tempoary local data structure, fill it out, copy it into the list, 
            // then do it again until the loop finishes
            index_data_helper this_render_mode_data;
            parse_index_node(&this_render_mode_data, index_node_ptrs[index_data_set]);
            all_index_data.push_back(this_render_mode_data);

            total_indices += this_render_mode_data.num_indices;

            Index_Meta_Data i_meta_data(this_render_mode_data.render_mode, this_render_mode_data.num_indices);
            put_shape_data_here->m_index_meta_data.push_back(i_meta_data);
         }

         // build a contiguous array of GLushorts to store all the index data
         put_shape_data_here->m_indices = (GLushort *)malloc(total_indices * sizeof(GLushort));
         put_shape_data_here->m_num_total_indices = total_indices;

         // go through all the lists of render modes and their index data and copy the indices into one giant array
         GLushort *dest_ptr = put_shape_data_here->m_indices;
         for (uint index_data_set = 0; index_data_set < all_index_data.size(); index_data_set++)
         {
            index_data_helper &this_data_set = all_index_data[index_data_set];

            // the index data in each data structure is contiguous, so a memcpy will suffice
            uint num_bytes_to_copy = this_data_set.num_indices * sizeof(GLushort);
            memcpy(dest_ptr, this_data_set.index_data_ptr, num_bytes_to_copy);
            dest_ptr += num_bytes_to_copy;

            // done with this set of index data, so free the malloc'd memory
            free(this_data_set.index_data_ptr);
         }
      }

      void Arcsynthesis_XML_Reader::parse_index_node(index_data_helper *put_index_data_here, const rapidxml::xml_node<> *node_ptr)
      {
         char *c_ptr = node_ptr->value();
         uint max_chars = node_ptr->value_size();
         std::string number_string = "";
         uint total_chars_scanned = 0;

         // collect all the index data together
         // Note: This memory does not need to be contiguous.  The vector is very handy for building 
         // a list of unknown size though, so I'll start with it.
         std::vector<GLushort> local_ushort_storage;

         while (total_chars_scanned < max_chars)
         {
            uint chars_scanned_this_number = get_next_number_string(c_ptr, &number_string);

            // the standard function atoi(...) returns an integer, but I am jamming it into an 
            // unsigned short, which is fine because I don't expect index numbers to exceed 
            // ~65000 (2^16)
            GLushort i = atoi(number_string.c_str());

            c_ptr += chars_scanned_this_number;
            total_chars_scanned += chars_scanned_this_number;

            local_ushort_storage.push_back(i);
         }

         // allocate some heap memory for this group of indices
         put_index_data_here->index_data_ptr = (GLushort *)malloc(local_ushort_storage.size() * sizeof(GLushort));
         put_index_data_here->num_indices = local_ushort_storage.size();

         // load the index data into the allocated memory
         for (uint ushort_counter = 0; ushort_counter < local_ushort_storage.size(); ushort_counter++)
         {
            put_index_data_here->index_data_ptr[ushort_counter] = local_ushort_storage[ushort_counter];
         }

         // figure out the type of rendering that is intended for this group of indices
         std::string render_mode_str = rapidxml::get_attrib_string(*node_ptr, "cmd");
         if ("tri-fan" == render_mode_str)
         {
            put_index_data_here->render_mode = GL_TRIANGLE_FAN;
         }
         else if ("tri-strip" == render_mode_str)
         {
            put_index_data_here->render_mode = GL_TRIANGLE_STRIP;
         }
         else if ("triangles" == render_mode_str)
         {
            put_index_data_here->render_mode = GL_TRIANGLES;
         }
         else
         {
            // bad
            throw std::exception("Arcsynthesis XML Reader, parse_index_node(...): two of the attributes have a different number of vectors");
         }
      }

      uint Arcsynthesis_XML_Reader::get_next_number_string(const char *start_ptr, std::string *put_number_string_here)
      {
         uint characters_scanned = 0;

         put_number_string_here->clear();
         if (0 == *start_ptr)
         {
            // already at end of data set; don't read anything
            return 0;
         }

         // scan through white space 
         const char *c_ptr = start_ptr;
         char c = *c_ptr;
         while ('\n' == c || '\r' == c || '\t' == c || ' ' == c)
         {
            c_ptr += 1;
            c = *c_ptr;
            characters_scanned++;
         }

         // scan through non-white space characters and put them into the number string
         while (1)
         {
            if (0 == c || '\n' == c || '\r' == c || '\t' == c || ' ' == c)
            {
               // 0 means end of data set, and white space means end of number but the data set still
               // has values in it, but both cases indicate the end of this number
               return characters_scanned;
            }

            // not a white-space character, so record it
            (*put_number_string_here) += c;

            c_ptr += 1;
            c = *c_ptr;
            characters_scanned++;
         }
      }
   }
}