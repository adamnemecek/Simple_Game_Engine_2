#ifndef ENGINE_MY_VERTEX_H
#define ENGINE_MY_VERTEX_H

#include <glm\glm.hpp>

namespace Shapes
{
   struct My_Vertex
   {
      static const unsigned int FLOATS_PER_POSITION = 3;
      static const unsigned int FLOATS_PER_COLOR = 3;
      static const unsigned int FLOATS_PER_NORMAL = 3;

      static const unsigned int BYTES_PER_POSITION = 12;
      static const unsigned int BYTES_PER_COLOR = 12;
      static const unsigned int BYTES_PER_NORMAL = 12;
      static const unsigned int BYTES_PER_VERTEX = BYTES_PER_POSITION + BYTES_PER_COLOR + BYTES_PER_NORMAL;

      glm::vec3 position;
      glm::vec3 color;
      glm::vec3 normal;
   };
}

#endif
