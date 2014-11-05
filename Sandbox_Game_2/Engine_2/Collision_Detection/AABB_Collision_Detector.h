#ifndef ENGINE_AABB_COLLISION_DETECTION_H
#define ENGINE_AABB_COLLISION_DETECTION_H

#include <Entities\Components\AABB_Component.h>
#include <vector>

namespace Collision_Detection
{
   class AABB_Collision_Detector
   {
   public:


   private:
      std::vector<Entities::AABB_Component *> m_bounding_boxes;
   };
}

#endif
