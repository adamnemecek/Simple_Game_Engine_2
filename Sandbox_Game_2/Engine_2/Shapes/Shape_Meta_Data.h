#ifndef SHAPE_META_DATA_H
#define SHAPE_META_DATA_H

#include <glm\vec3.hpp>

namespace Shapes
{
   // it's all public, so be nice to the data
   // Note: The geometry loader is responsible for analyzing and setting this info.
   struct __declspec(dllexport) Shape_Meta_Data
   {
      float m_min_X;
      float m_max_X;
      float m_min_Y;
      float m_max_Y;
      float m_min_Z;
      float m_max_Z;

      glm::vec3 m_center_of_vertices;
   };
}

#endif