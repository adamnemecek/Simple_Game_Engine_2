#ifndef ENGINE_SHAPE_DATA_H
#define ENGINE_SHAPE_DATA_H

#include <glm\glm.hpp>
#include <Utilities\include_GL_version.h>
#include <Utilities\Typedefs.h>

namespace Shapes
{
   // this is a simple support class for the geometry, so just make everything public to access easy
   // Note: Export this to dll because it is a member of the geometry class.
   struct __declspec(dllexport) Shape_Data
   {
      // ensure that pointers are null
      Shape_Data()
         : m_position(0),
         m_normals(0),
         m_colors(0),
         m_indices(0)
      {
      }

      // ensure that memory is cleared on object destruction
      ~Shape_Data()
      {
         delete[] m_position;
         delete[] m_normals;
         delete[] m_colors;
         delete[] m_indices;
      }

      // it is expected for there to be one position, one normal, and one color per vert, so their count is identical
      glm::vec3 *m_position;
      glm::vec3 *m_normals;
      glm::vec3 *m_colors;
      uint m_num_verts;

      GLushort *m_indices;
      uint m_num_indices;
   };
}

#endif
