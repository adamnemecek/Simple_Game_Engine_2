#ifndef ENGINE_INCLUDE_HELPER_DEFAULT_VECTORS_H
#define ENGINE_INCLUDE_HELPER_DEFAULT_VECTORS_H

#include <glm\vec3.hpp>
namespace Utilities
{
   namespace Default_Vectors
   {
      // Note: Cross product order should be maintained for consistency.
      // I have chosen the following order: cross(WORLD_UP_VECTOR, another_vector).
      const glm::vec3 WORLD_UP_VECTOR(0.0f, +1.0f, 0.0f);

      const glm::vec3 WORLD_FORWARD(0.0f, 0.0f, -1.0f);
   }
}

#endif
