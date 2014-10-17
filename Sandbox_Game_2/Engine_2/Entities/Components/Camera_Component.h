#ifndef ENGINE_CAMERA_COMPONENT_H
#define ENGINE_CAMERA_COMPONENT_H

namespace Rendering
{
   class Camera;
}

namespace Entities
{
   class __declspec(dllexport) Camera_Component
   {
   public:
      Rendering::Camera *m_camera_ptr;
   };
}


#endif
