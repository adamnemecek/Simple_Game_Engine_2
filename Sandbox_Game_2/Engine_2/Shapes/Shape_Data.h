#ifndef ENGINE_SHAPE_DATA_H
#define ENGINE_SHAPE_DATA_H

#include <Utilities\Include_Helpers\GL_Version.h>
#include <Utilities\Typedefs.h>
#include <Shapes\My_Vertex.h>
#include <vector>
#include <Shapes\Index_Meta_Data.h>

namespace Shapes
{
   // this is a simple support class for the geometry, so just make everything public to access easy
   // Note: Export this to dll because it is a member of the geometry class.
   struct __declspec(dllexport) Shape_Data
   {
      // ensure that values start as null
      Shape_Data()
         : m_verts(0),
         m_num_verts(0),
         m_indices(0),
         m_num_total_indices(0)
      {
      }

      // ensure that memory is cleared on object destruction
      ~Shape_Data()
      {
         delete[] m_verts;
         delete[] m_indices;
      }

      inline GLsizeiptr vertex_buffer_size() const
      {
         return (m_num_verts * My_Vertex::BYTES_PER_VERTEX);
      }

      inline GLsizeiptr index_buffer_size() const
      {
         // a pointer will be dereferenced here, so check that it isn't null first
         if (0 != m_indices)
         {
            return (m_num_total_indices * sizeof(*m_indices));
         }
         
         return 0;
      }
      
      // this is a collection of strings that will store the parameters that were 
      // used to construct this shape
      // Note: This is very useful during scene loading and saving so that two 
      // shapes that use the same generator but different parameters (ex: spheres
      // with different radii) can be saved and reconstructed later.
      std::vector<std::string> m_parameters;

      // store a pointer to the vertex data 
      My_Vertex *m_verts;
      uint m_num_verts;

      // store an array index data
      // Note: Rather than store an array of indices (GLushort), I am adjusting to 
      // ArcSynthesis' XML mesh files, some of which have multiple sets of indices
      // that each use their own drawing commands.  I will still store all the indices
      // in a single array, but I will also store additional information that says 
      // which indices use which draw command. 
      GLushort *m_indices;
      uint m_num_total_indices;

      // stores the drawing command and the number of consecutive indices draw
      //std::vector<std::pair<GLenum, uint>> m_index_meta_data;
      std::vector<Index_Meta_Data> m_index_meta_data;
   };
}

#endif
