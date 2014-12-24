#ifndef GEOMETRY_META_DATA_H
#define GEOMETRY_META_DATA_H

#include <glm\vec3.hpp>

// forward declarations
namespace Entities
{
   class AABB_Component;
   class Physics_Component;
}

namespace Shapes
{
   namespace Geometry_Creation
   {
      class Geometry_Loader;
   }
}

namespace Shapes
{

   class __declspec(dllexport) Geometry_Meta_Data
   {
   public:
      // default constructors and destructor

   private:
      // let a select few classes have access to these
      friend class Geometry_Creation::Geometry_Loader;
      friend class Entities::AABB_Component;
      friend class Entities::Physics_Component;

      float m_min_X;
      float m_max_X;
      float m_min_Y;
      float m_max_Y;
      float m_min_Z;
      float m_max_Z;

      glm::vec3 m_center_of_geometry;
   };
}

#endif