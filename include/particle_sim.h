#ifndef PARTICLE_SIM
#define PARTICLE_SIM

#include "definitions.h"
#include "camera.h"
#include <vector>

namespace GE
{
    
// 3D fireworks
class ParticleSim
{
    public:
        struct Particle
        {
            Vector3 position;
            Vector3 speed;
            double damping;
            Color color;
            double size;
            double lifeTime;
            bool isFireWork;
            double explodeTime;
            bool isCamCoord = false;
        };

        typedef std::vector<Particle> ParticleList;
    
        ParticleSim();
        void addFireWork();
        void addFireWork(Particle p);
        void addParticle(Particle p);
        bool updateParticle(Particle& p);
        void update(Img& img, double dt, Camera& cam);
        ParticleList& particles();
    private:
        double m_elapsed;
        int m_max;
        int m_nFireWorks;
        ParticleList m_particles;
};

}

#endif /* PARTICLE_SIM */
