#ifndef ENGINE_INCLUDE_HELPER_WORLD_UP_VECTOR_H
#define ENGINE_INCLUDE_HELPER_WORLD_UP_VECTOR_H

#include <glm\vec3.hpp>
namespace Utilities
{
   // Note: Cross product order should be maintained for consistency.
   // I have chosen the following order: cross(orientation, WORLD_UP_VECTORtor).

   const glm::vec3 WORLD_UP_VECTOR(0.0f, +1.0f, 0.0f);
}

#endif
