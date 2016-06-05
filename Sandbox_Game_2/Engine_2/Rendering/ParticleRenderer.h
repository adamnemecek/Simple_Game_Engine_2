#pragma once

#include <vector>
#include "glm/mat4x4.hpp"
#include "Particles/ParticleManager.h"
#include "Utilities/Include_Helpers/GL_Version.h"


namespace Rendering
{
    class Camera;
}

namespace Rendering
{
    class __declspec(dllexport) ParticleRenderer
    {
    public:
        ParticleRenderer();

        bool Init(const GLuint programId);
        void SetParticleManager(const Particles::ParticleManager *pParticleManager);
        void SetViewport(GLsizei width, GLsizei height);
        void SetCameraToRender(Camera *camera_ptr);
        void Render();
    private:
        GLuint _programId;  // bind this before rendering
        GLuint _vaoId;  // stores attrib pointers for a particular array buffer
        GLuint _arrayBufferId;  // necessary for glBufferSubData(...)
        GLint _unifLocWorldToClipMatrix;
        glm::mat4 _perspectiveMat;
        Camera *_pCamera;

        const Particles::ParticleManager *_pParticleManager;
        std::vector<float> _valuesForRender;
    };
}