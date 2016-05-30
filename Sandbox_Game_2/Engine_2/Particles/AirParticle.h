#pragma once

#include <glm/vec3.hpp>
#include <vector>

// useful for resetting a vec3 without having to dereference each item or make a new one
static const glm::vec3 NULL_VEC;

namespace Particles
{
    // TODO: this may be of use in time as an argument when creating a particle
    enum PARTICLE_TYPE
    {
        AIR = 0,
        O2,     // NOT IN USE
        CO2     // NOT IN USE
        // etc.
    };

    struct AirParticle
    {
        AirParticle(const PARTICLE_TYPE type = AIR) 
            : _type(type)   // once this particle type, always this particle type
        {
            this->Reset(NULL_VEC, NULL_VEC);
        }

        // make inline because, when handling many many particles, I'll take whatever speed advantage I can get
        inline void Reset(const glm::vec3 emitterPosition, const glm::vec3 velocity)
        {
            switch (_type)
            {
            case AIR:
                _mass = 0.001f;
                _collisionRadius = 0.02f;
                _repulsiveForce = 1.0f;
                break;
            default:
                _mass = 0.0f;
                _collisionRadius = 0.0f;
                _repulsiveForce = 0.0f;
                break;
            }

            _posCurr = emitterPosition;
            _posPrev = emitterPosition;
            _velocity = velocity;

            // upon creation there is no "net force" acting upon this particle
            _netForceActingOnMe = NULL_VEC;  
        }


        // determines momentum and intertia
        // Note: Units in grams.
        float _mass;

        // the particle's position in the previous and current frames will be used to calculate 
        // collisions, such as determining if two particles crossed paths or if a particle cross 
        // and plane that was the boundry of a solid
        glm::vec3 _posCurr;
        glm::vec3 _posPrev;

        // Note: In meters/second
        glm::vec3 _velocity;

        // at the beginning of each frame, a bunch of particles may be in proximety to each 
        // other, and each of them may have crossed paths with one or more of the others between 
        // the previous frame to the current one, each imparting a replusive force on the others,
        // all of which add up to create a net force on this particle
        glm::vec3 _netForceActingOnMe;

        // TODO: make different particle types interact differently with other particles, such as
        // the point of attraction instead of replusion
        PARTICLE_TYPE _type;

        // determined by particle type, air density, and possibly by the total number of 
        // particles, the collision radius gives this particle, which is infinitesimally small 
        // and therefore almost impossible to collide with directly, a region of influence so 
        // that particles can bounce off each other
        float _collisionRadius;

        // if something is within the collision radius, then it experienced a linearly regressed 
        // repulsive force from 0 at the edge of the collision radius to the full force at the
        // center
        // Note: In Newtons.
        // Also Note: This repulsive force is a property of the particle type.  
        float _repulsiveForce;
    };
}

