#include "ParticleManager.h"

//#include <ctime>    // for time(...) for srand()
//#include <cstdlib>  // for rand()
#include <algorithm>    // for std:remove_if(...)
#include <glm/detail/func_geometric.hpp> // for normalizing certain normal vector calculations
#include "Utilities/My_Constants.h"     // for INVERSE_RAND_MAX
#include "Utilities/FastRandom.h"

// helps to clean up particle initialization
// TODO: make a member of "particle manager" base class (when it exists)
inline glm::vec3 CalculateRandomVelocity(const float minVelocity, const float maxVelocity)
{
    using namespace Utilities;

    // create a velocity magnitude between min and max
    float vDelta = maxVelocity - minVelocity;
    float velocityVariation = ((float)rand() * INVERSE_RAND_MAX) * vDelta;
    float velocityMagnitude = minVelocity + velocityVariation;

    // create a normalized random vector, then multiple all items by the magnitude
    // Note: The hard-coded mod100 is just to prevent the random axis magnitudes from 
    // getting too crazy different from each other;
    float newX = (float)(FastRandPosAndNeg() % 100);
    float newY = (float)(FastRandPosAndNeg() % 100);
    float newZ = (float)(FastRandPosAndNeg() % 100);
    glm::vec3 randomVelocity = glm::normalize(glm::vec3(newX, newY, newZ));

    return randomVelocity * velocityMagnitude;
}

namespace Particles
{
    ParticleManager::ParticleManager()
    {
        //// for initializing particles with a sense of randomness
        //srand((unsigned int)time(0));
    }

    bool ParticleManager::Init(const int numParticles, const int numToLaunchPerFrame,
        const float initialMinVelocity, const float initialMaxVelocity,
        const float radius, const glm::vec3 emitterPosition)
    {
        // initialize these with starting sizes to avoid unnecessary reallocation 
        _allParticles.resize(numParticles);
        
        _numToLaunchPerFrame = numToLaunchPerFrame;
        _initialMinVelocity = initialMinVelocity;
        _initialMaxVelocity = initialMaxVelocity;
        _radius = radius;
        _emitterPosition = emitterPosition;

        // all particles start as "inactive"
        for (size_t particleIndex = 0; particleIndex < _allParticles.size(); particleIndex++)
        {
            _inactiveParticles.push_back(_allParticles.data() + particleIndex);
        }

        return true;
    }

    // intended to be used with std::remove_if(...) for fast particle updating and removal, this 
    // function updates a particle's position and flags it for removal if necessary
    // Note: 
    bool ParticleManager::UpdateParticleRemoveIfTrue(AirParticle *p)
    {
        glm::vec3 pathLastFrame = p->_posCurr - p->_posPrev;

        p->_posPrev = p->_posCurr;

        // detect collisions and sum forces
        // calculate new velocity from current velocity and net force vector and delta time

        // new position
        p->_posCurr = p->_posCurr + (p->_velocity * _deltaTimeSec);

        // check if the particle went out of bounds
        glm::vec3 vecToCenter = p->_posCurr - _emitterPosition;
        float distSqr = glm::dot(vecToCenter, vecToCenter);
        float radiusSqr = _radius * _radius;
        if (distSqr > radiusSqr)
        {
            // out of bounds, but need to put the pointer in a responsible place because  
            // std:remove_if(...) has a tendency to trample all over the values that are flagged 
            // for removal
            _inactiveParticles.push_back(p);
            return true;
        }

        // not out of bounds, so still good for rendering
        return false;
    }

    void ParticleManager::Update(const float deltaTimeSec)
    {
        _deltaTimeSec = deltaTimeSec;

        // update existing particles
        // Note: Updating existing particles and removing out-of-bounds particles has been 
        // condensed via std::remove_if(...).  It rearranges items so that all that fail the 
        // predicate function are moved to the end of the container, and when it finished an
        // iterator is returned to first item in a contiguous set of items to be removed.  I 
        // used this predicate function to update as well as flag for removal, and then since all
        // the items to be removed are contiguous, cache misses are down.
        // Also Note: std::remove_if(...) does not like addresses to member functions, but it is
        // perfectly happy with a lambda function that calls a class method.  Roll with it.
        PARTICLE_CONTAINER::iterator sortOfEnd =
            std::remove_if(_activeParticles.begin(), _activeParticles.end(), 
                [&](AirParticle *p) { return UpdateParticleRemoveIfTrue(p); });

        // the predicate functions take care of putting any pointers to out-of-bound particles 
        // into the "inactive particles" collection, so now just handle the erasing
        // Note: The erasing reduces the array size, but not the capacity.
        _activeParticles.erase(sortOfEnd, _activeParticles.end());

        // launch new particles
        for (int particlesRelaunched = 0;
            particlesRelaunched < _numToLaunchPerFrame;
            particlesRelaunched++)
        {
            if (_inactiveParticles.empty())
            {
                // all particles active
                break;
            }

            // have at least one
            AirParticle *p = _inactiveParticles.back();

            // reset the particle, add it to the "active particles" collection, and remove it 
            // from the "inactive particles" collection
            p->Reset(_emitterPosition,
                CalculateRandomVelocity(_initialMinVelocity, _initialMaxVelocity));
            _activeParticles.push_back(p);
            _inactiveParticles.pop_back();
        }
    }

    const PARTICLE_CONTAINER &ParticleManager::GetActiveParticles() const
    {
        return _activeParticles;
    }

    const unsigned int ParticleManager::GetNumParticles() const
    {
        return _allParticles.size();
    }
}