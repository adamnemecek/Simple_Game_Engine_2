#include "ParticleRenderer.h"

#include "Particles/AirParticle.h"
#include "Rendering/Camera.h"

// for glm::value_ptr(...)
#include <glm/gtc/type_ptr.hpp>

#include "Utilities/Typedefs.h"
#include <glm/detail/func_geometric.hpp> // for normalizing vector 
#include "Utilities/Include_Helpers/GL_Version.h"
#include "Utilities/ShaderManager.h"
#include "Utilities/Include_Helpers/GLM_Mat_Transform.h"
#include "Utilities/My_Assert.h"

#include <iostream>
using std::cout;
using std::endl;

namespace Rendering
{
    ParticleRenderer::ParticleRenderer() : 
        _programId(-1), _vaoId(-1), _arrayBufferId(-1), _pParticleManager(0)
    {
    }

    bool ParticleRenderer::Init(const GLuint programId)
    {
        if (programId < 0)
        {
            return false;
        }
        
        _programId = programId;

        // MUST bind the program beforehand or else the VAO generation and binding will blow up
        glUseProgram(_programId);

        _vaoId = -1;
        glGenVertexArrays(1, &_vaoId);
        glBindVertexArray(_vaoId);

        _arrayBufferId = -1;
        glGenBuffers(1, &_arrayBufferId);
        glBindBuffer(GL_ARRAY_BUFFER, _arrayBufferId);

        // data is loaded at runtime (don't load data now), and it changes on every frame, so 
        // set for dynamic draw
        // Note: This conditional allows Init(...) and SetParticleManager(...) to be called in no particular order
        if (_pParticleManager != 0)
        {
            GLuint bufferSizeBytes = sizeof(float) + sizeof(glm::vec3) * _pParticleManager->GetNumParticles();
            glBufferData(GL_ARRAY_BUFFER, bufferSizeBytes, 0, GL_DYNAMIC_DRAW);
        }
        else
        {
            glBufferData(GL_ARRAY_BUFFER, 0, 0, GL_DYNAMIC_DRAW);
        }

        // position = 0 (first 3 floats)
        // magnitude of net force vector = 1 (4th float)
        uint vertexArrayIndex = 0;
        uint bufferStartOffset = 0;
        uint itemsPerPosition = 3;
        uint itemsPerForce = 1;
        GLenum itemType = GL_FLOAT;
        uint bytesPerStep = sizeof(float) + sizeof(glm::vec3);

        // position
        glEnableVertexAttribArray(vertexArrayIndex);
        glVertexAttribPointer(vertexArrayIndex, itemsPerPosition, itemType, GL_FALSE, bytesPerStep, (void *)bufferStartOffset);

        // magnitude of net force vector
        bufferStartOffset += sizeof(glm::vec3); // hop past the position vector
        vertexArrayIndex++;
        glEnableVertexAttribArray(vertexArrayIndex);
        glVertexAttribPointer(vertexArrayIndex, itemsPerForce, itemType, GL_FALSE, bytesPerStep, (void *)bufferStartOffset);

        // cleanup
        glBindVertexArray(0);   // unbind this BEFORE the array
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glUseProgram(0);    // always last

        // lastly (doesn't require a bound program), find the shader program's uniform(s)
        _unifLocWorldToClipMatrix = Utilities::ShaderManager::GetInstance().
            FindUniform(_programId, "unifWorldToClipMatrix");

        return true;
    }

    void ParticleRenderer::SetParticleManager(const Particles::ParticleManager *pParticleManager)
    {
        _pParticleManager = pParticleManager;

        // the air particle only needs current position (vec3) and net force (magnitude of net 
        // force vector, determines color)
        unsigned int numFloats = (3 + 1) * _pParticleManager->GetNumParticles();
        _valuesForRender.resize(numFloats);

        // this code allows Init(...) and SetParticleManager(...) to be called in no particular order
        if (_programId >= 0)
        {
            glUseProgram(_programId);
            glBindBuffer(GL_ARRAY_BUFFER, _arrayBufferId);

            GLuint bufferSizeBytes = numFloats * 4;
            glBufferData(GL_ARRAY_BUFFER, bufferSizeBytes, 0, GL_DYNAMIC_DRAW);

            // cleanup
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glUseProgram(0);
        }

    }

    void ParticleRenderer::SetViewport(GLsizei width, GLsizei height)
    {
        glViewport(0, 0, width, height);

        float fov_radians = (1.0f / 4.0f) * 3.14159f;   // 1/2 pi
        float aspect_ratio = (float)width / height;
        float near_plane_dist = 0.1f;
        float far_plane_dist = 100.0f;
        _perspectiveMat = glm::perspective(fov_radians, aspect_ratio, near_plane_dist, far_plane_dist);

        cout << "resizing to aspect ratio " << aspect_ratio << endl;
    }

    void ParticleRenderer::SetCameraToRender(Camera *camera_ptr)
    {
        MY_ASSERT(camera_ptr != 0);
        _pCamera = camera_ptr;
    }

    void printMe(const Particles::PARTICLE_CONTAINER &these)
    {
        printf("\ntotal particles: %d\n", these.size());
        for (size_t particleIndex = 0; particleIndex < these.size(); particleIndex++)
        {
            const Particles::AirParticle *particle = these[particleIndex];
            printf("[%d] = <%.2f, %.2f, %.2f>\n", particleIndex, particle->_posCurr.x, particle->_posCurr.y, particle->_posCurr.z);
            break;
        }
    }

    void ParticleRenderer::Render()
    {
        // MUST call the geometry renderer BEFORE this renderer because that one clears these bits
        //glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        //glClearDepth(1.0f);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // we can't render without a camera
        MY_ASSERT(_pCamera != 0);

        // run through all particles and get the latest info, then upload it to the GPU
        // Note: Upon initialization, the "values for render" vector was sized to allow for the 
        // maximum number of particles that this particle manager is handling, plus 1, so there 
        // should be no index overruns.
        const Particles::PARTICLE_CONTAINER &renderThese =
            _pParticleManager->GetActiveParticles();
        //printMe(renderThese);
        int floatIndex = 0;
        for (size_t particleIndex = 0; particleIndex < renderThese.size(); particleIndex++)
        {
            const Particles::AirParticle *particle = renderThese[particleIndex];
            _valuesForRender[floatIndex++] = particle->_posCurr.x;
            _valuesForRender[floatIndex++] = particle->_posCurr.y;
            _valuesForRender[floatIndex++] = particle->_posCurr.z;
            _valuesForRender[floatIndex++] = 
                glm::length(particle->_netForceActingOnMe);
        }

        // -1 so that it is effectively a "count" value now
        floatIndex--;   

        glUseProgram(_programId);

        // particles are single points, so they don't have a "model to world" matrix, and 
        // therefore the only transformation matrix needed is the "world to clip"
        glm::mat4 worldToClip = _perspectiveMat * _pCamera->get_world_to_view_matrix();

        for (size_t particleIndex = 0; particleIndex < renderThese.size(); particleIndex++)
        {
            const Particles::AirParticle *particle = renderThese[particleIndex];
            glm::vec4 v4 = glm::vec4(particle->_posCurr, 1.0f);
            v4 = worldToClip * v4;
            //printf("transformed v = <%.2f, %.2f, %.2f, %.2f>\n", v4.x, v4.y, v4.z, v4.w);
            break;
        }


        glUniformMatrix4fv(_unifLocWorldToClipMatrix, 1, GL_FALSE, glm::value_ptr(worldToClip));

        glBindVertexArray(_vaoId);
        glBindBuffer(GL_ARRAY_BUFFER, _arrayBufferId);

        glBufferSubData(GL_ARRAY_BUFFER, 0, floatIndex * 4, _valuesForRender.data());
        glDrawArrays(GL_POINTS, 0, renderThese.size());
        //glDrawArrays(GL_TRIANGLES, 0, renderThese.size());

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glUseProgram(0);
    }

}
