#include "ParticleManager.h"

#include <ctime>    // for time(...) for srand()
#include <cstdlib>  // for rand()
#include <glm/detail/func_geometric.hpp> // for normalizing certain normal vector calculations
#include "Utilities/My_Constants.h"     // for INVERSE_RAND_MAX

// helps to clean up particle initialization
// TODO: make a member of "particle manager" base class (when it exists)
inline glm::vec3 CalculateRandomVelocity(const float minVelocity, const float maxVelocity)
{
    using namespace Utilities;

    float vDelta = maxVelocity - minVelocity;
    float velocityVariation = ((float)rand() * INVERSE_RAND_MAX) * vDelta;
    float velocityMagnitude = minVelocity + velocityVariation;

    // create a normalized random vector, then multiple all items by the magnitude
    glm::vec3 randomVelocity = glm::normalize(glm::vec3(
        rand() * INVERSE_RAND_MAX,    // X
        rand() * INVERSE_RAND_MAX,    // Y
        rand() * INVERSE_RAND_MAX));  // Z

    return randomVelocity * velocityMagnitude;
}

namespace Particles
{
    ParticleManager::ParticleManager()
    {
    }

    bool ParticleManager::Init(const int numParticles, const int numToLaunchPerFrame,
        const float initialMinVelocity, const float initialMaxVelocity,
        const float radius, const glm::vec3 emitterPosition)
    {
        // initialize these with starting sizes to avoid unnecessary reallocation 
        _allParticles.resize(numParticles);
        
        // do NOT attempt to resize the std::deque structures or else, since they store 
        // pointers, they will stick a bunch of null pointers into itself, and then the next push

        _emitterPosition = emitterPosition;
        _initialMinVelocity = initialMinVelocity;
        _initialMaxVelocity = initialMaxVelocity;

        // because dividing by RAND_MAX for every particle is expensive
        float inverseRandMax = 1.0f / (float)RAND_MAX;

        for (size_t particleIndex = 0; particleIndex < _allParticles.size(); particleIndex++)
        {
            _relaunchThese.push_back(_allParticles.data() + particleIndex);
        }

        return true;
    }


    void ParticleManager::Update(const float deltaTimeSec)
    {
        srand((unsigned int)time(0));

        for (size_t activeIndex = 0; activeIndex < _activeParticles.size(); activeIndex++)
        {
            AirParticle *p = _activeParticles[activeIndex];

            glm::vec3 pathLastFrame = p->_posCurr - p->_posPrev;

            p->_posPrev = p->_posCurr;

            // detect collisions and sum forces
            // calculate new velocity from current velocity and net force vector and delta time

            // new position
            p->_posCurr = p->_posCurr + (p->_velocity * deltaTimeSec);

            // check if the particle went out of bounds
            glm::vec3 vecToCenter = p->_posCurr - _emitterPosition;
            float distSqr = glm::dot(vecToCenter, vecToCenter);
            float radiusSqr = _radius * _radius;
            if (distSqr > radiusSqr)
            {
                // out of bounds
                _relaunchThese.push_back(p);
                _activeParticles.erase(_activeParticles.begin() + activeIndex);
                activeIndex--;
            }
            else
            {
                // not out of bounds, so still good for rendering
            }
        }

        for (int particlesRelaunched = 0;
            particlesRelaunched < _numToLaunchPerFrame;
            particlesRelaunched++)
        {
            if (_relaunchThese.empty())
            {
                // all particles active
                break;
            }

            // have at least one
            AirParticle *p = _relaunchThese[0];

            // reset the particle, add it to the "active particles" collection, and remove it 
            // from the "relaunch these" collection
            p->Reset(_emitterPosition,
                CalculateRandomVelocity(_initialMinVelocity, _initialMaxVelocity), 
                _radius);
            _activeParticles.push_back(p);
            _relaunchThese.pop_front();
        }
    }
}