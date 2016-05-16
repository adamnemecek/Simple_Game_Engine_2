#pragma once

#include "Particles/AirParticle.h"
//#include "Shapes/Shape_Data.h"
#include <vector>
#include <deque>

#include <glm/vec3.hpp>

namespace Particles
{
    class ParticleManager
    {
    public:
        ParticleManager();

        // numParticles: maximum number of particles that can exist inside the radius at once
        // numToLaunchPerFrame: must be <= numParticles; used to allow particles to be release gradually
        // inital min/max velocities: allow some variance to initial particle behavior
        // radius: outside this spherical region the particles will be relaunched from center point
        // emitterPosition: center point of particle emission
        bool Init(const int numParticles, const int numToLaunchPerFrame,
            const float initialMinVelocity, const float initialMaxVelocity,
            const float radius, const glm::vec3 emitterPosition);

        void Update(const float deltaTimeSec);

    private:
        // can be used to allow particles to release over time instead 
        int _numToLaunchPerFrame;

        // allows for some variance to initial particle behavior
        float _initialMinVelocity;
        float _initialMaxVelocity;

        // particles outside this radius from emitter position are relaunched
        float _radius;
        glm::vec3 _emitterPosition;

        std::vector<AirParticle> _allParticles;
        std::deque<AirParticle *> _activeParticles;
        std::deque<AirParticle *> _relaunchThese;
    };
}
