#ifndef ENGINE_RENDERER_H
#define ENGINE_RENDERER_H

#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"
#include "Utilities/Include_Helpers/GL_Version.h"
#include "Utilities/Typedefs.h""
#include <string>

// the renderer stores arrays of both of these, so we can't forward declare them
#include <Rendering\Renderable.h>

// forward declarations
namespace Shapes
{
    class Geometry;
}
namespace Entities
{
    class Entity;
}
namespace Rendering
{
    class Camera;
}

namespace Rendering
{
    class __declspec(dllexport) Renderer
    {
    public:
        Renderer();
        bool Init(const GLuint programId);
        bool Shutdown();

        // fill out an unassigned renderable in the Renderer's collection of renderables
        // with the geometry that it is referencing, which includes information on the 
        // vertex array object binding, and the entity that it is for, which is how the
        // renderable gets the entity-specific model-to-world matrix
        void ConfigureNewRenderable(const Entities::Entity *pE, const Shapes::Geometry *pG);

        void SetViewport(GLsizei width, GLsizei height);
        void SetCameraToRender(Camera *camera_ptr);

        void Render();

    private:
        Camera *_pCamera;

        GLuint _renderProgramId;
        GLint _unifLocModelToCameraMatrix;
        GLint _unifLocCameraToClipMatrix;
        GLint _unifLocLight1PosCs;
        GLint _unifLocLight1Intensity;
        GLint _unifLocLight2PosCs;
        GLint _unifLocLight2Intensity;
        glm::mat4 _perspectiveMat;

        // declare a pool of renderables
        static const uint _MAX_RENDERABLES = 10;
        Renderable _renderables[_MAX_RENDERABLES];
        uint _numCurrentRenderables;
    };
}


#endif
