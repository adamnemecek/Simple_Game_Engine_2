#ifndef ENGINE_SHAPE_DATA_H
#define ENGINE_SHAPE_DATA_H

#include <glm\glm.hpp>
#include <Utilities\include_GL_version.h>
#include <Utilities\Typedefs.h>
#include <Shapes\My_Vertex.h>

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
         m_num_indices(0)
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
            return (m_num_indices * sizeof(*m_indices));
         }

         return 0;
      }
      
      My_Vertex *m_verts;
      uint m_num_verts;
      GLushort *m_indices;
      uint m_num_indices;
   };
}

#endif
