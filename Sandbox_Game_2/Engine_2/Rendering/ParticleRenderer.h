#pragma once

#include "Particles/ParticleManager.h"
#include <Utilities/Include_Helpers/GL_Version.h>

namespace Rendering
{
    class __declspec(dllexport) ParticleRenderer
    {
    public:
        ParticleRenderer();

        bool Init(const GLuint programId);
        void Render();
    private:
        GLuint _programId;  // bind this before rendering
    };
}