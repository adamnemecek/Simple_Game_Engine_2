#pragma once

#include <glm/vec3.hpp>
#include <vector>

namespace Particles
{
    // TODO: this may be of use in time as an argument when creating a particle
    enum PARTICLE_TYPE
    {
        AIR = 0,
    };

    struct AirParticle
    {
        // TODO: are these comments correct?

        // determines momentum and intertia
        float mass;

        // the particle's position in the previous and current frames will be used to calculate 
        // collisions, such as determining if two particles crossed paths or if a particle cross 
        // and plane that was the boundry of a solid
        glm::vec3 posCurr;
        glm::vec3 posPrev;

        // self-explanatory
        glm::vec3 velocity;

        // at the beginning of each frame, a bunch of particles may be in proximety to each 
        // other, and each of them may have crossed paths with one or more of the others between 
        // the previous frame to the current one, each imparting a replusive force on the others,
        // all of which add up to create a net force on this particle
        glm::vec3 _netForceActingOnMe;

        // TODO: make different particle types interact differently with other particles, such as
        // the point of attraction instead of replusion
        PARTICLE_TYPE type;

        // determined by particle type, air density, and possibly by the total number of 
        // particles, the collision radius gives this particle, which is infinitesimally small 
        // and therefore almost impossible to collide with directly, a region of influence so 
        // that particles can bounce off each other
        float collisionRadius;

        // if something is within the collision radius, then it experienced a linearly regressed 
        // repulsive force from 0 at the edge of the collision radius to the full force at the
        // center
        // Note: This repulsive force is a property of the particle type.  
        float repulsiveForce;
    };
}

