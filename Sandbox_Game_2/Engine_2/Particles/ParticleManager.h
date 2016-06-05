#pragma once

#include "Particles/AirParticle.h"
//#include "Shapes/Shape_Data.h"
#include <vector>

#include <glm/vec3.hpp>

namespace Particles
{
    typedef std::vector<AirParticle *> PARTICLE_CONTAINER;

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

        const PARTICLE_CONTAINER &GetActiveParticles() const;
        const unsigned int GetNumParticles() const;

    private:
        // Note: I will be using std::remove_if(...) over the container of active particles, but 
        // since the predicate argument can only take a reference to an item in that container 
        // and updating and determining if a particle is out of bounds requires delta time and 
        // a distance from the emitter position, use a method for the predicate so that members
        // can be tapped into.
        bool UpdateParticleRemoveIfTrue(AirParticle *p);
        float _deltaTimeSec;

        // can be used to allow particles to release over time instead 
        int _numToLaunchPerFrame;

        // allows for some variance to initial particle behavior
        float _initialMinVelocity;
        float _initialMaxVelocity;

        // particles outside this radius from emitter position are relaunched
        float _radius;
        glm::vec3 _emitterPosition;

        // make these all vectors to guarantee contiguous memory, which keeps cache misses (the 
        // biggest villain when dealing with many many items) down
        std::vector<AirParticle> _allParticles;
        PARTICLE_CONTAINER _activeParticles;
        PARTICLE_CONTAINER _inactiveParticles;

    };
}
