#include "particle_sim.h"
#include <time.h>

using namespace GE;

ParticleSim::ParticleSim()
{
    m_elapsed = 0;
    m_max = 5000;
    m_nFireWorks = 0;
    m_particles.reserve(m_max);
}

void ParticleSim::addFireWork()
{
    Vector3 dir({(double)std::rand()/RAND_MAX-0.5, 
                             (double)std::rand()/RAND_MAX-0.5, 
                             (double)std::rand()/RAND_MAX-0.5});
    dir *= 3*(double)std::rand()/RAND_MAX + 2;

    ParticleSim::Particle p 
    {
        Vector3({0,0,0}),
        Vector3({0,1,15}) + dir,
        0,
        {255,255,255},
        20,
        0.01,
        true,
        1.2*(double)std::rand()/RAND_MAX + 0.2,
        false
    };
    m_nFireWorks += 1;
    addParticle(p);
}

void ParticleSim::addFireWork(Particle p)
{
    m_nFireWorks += 1;
    m_particles.push_back(p);
}

void ParticleSim::addParticle(Particle p)
{
    if (!p.isFireWork && m_particles.size() >= m_max) return;
    m_particles.push_back(p);
}

bool ParticleSim::updateParticle(Particle& p)
{

}

void ParticleSim::update(Img& img, double dt, Camera& cam)
{
    Vector3 g({0, 0, -9.82}); // gravity
    auto cameraRotInv = cam.rot().inv();
    ParticleList newParticles;
    newParticles.reserve(3000);
    Vector3 camPlaneNormal({cam.rot()(0,2), cam.rot()(1,2), cam.rot()(2,2)});
    
    for (auto it =m_particles.begin(); it!=m_particles.end(); )
    {
        auto& p = *it;
        if (p.lifeTime < 0)
        {
            m_particles.erase(it);
        }
        else if (p.isFireWork && p.explodeTime <= 0)
        {
            unsigned char red, green, blue;
            bool completeRandom = true;
            if ((double)std::rand()/RAND_MAX < 0.8)
            {
                red = (double)std::rand()/RAND_MAX > 0.5 ? 255 : 100;
                green = (double)std::rand()/RAND_MAX > 0.5 ? 255 : 100;
                blue = (double)std::rand()/RAND_MAX > 0.5 ? 255 : 100;
                completeRandom = false;
            }

            for (int i=0; i<std::min(100, (int)(m_max/(m_nFireWorks+1))); i++)
            {
                Vector3 dir({(double)std::rand()/RAND_MAX-0.5, 
                             (double)std::rand()/RAND_MAX-0.5, 
                             (double)std::rand()/RAND_MAX-0.5});
                
                dir *= 50*(double)std::rand()/RAND_MAX + 20; // big explosion
                //dir *= 30*(double)std::rand()/RAND_MAX + 10; // small explosion
                if (completeRandom)
                {
                    red = (double)std::rand()/RAND_MAX > 0.5 ? 255 : 0;
                    green = (double)std::rand()/RAND_MAX > 0.5 ? 255 : 0;
                    blue = (double)std::rand()/RAND_MAX > 0.5 ? 255 : 0;
                }
                ParticleSim::Particle pNew 
                {
                    p.position,
                    dir,
                    0.3*(double)std::rand()/RAND_MAX+.1,
                    {red,green,blue},
                    250*(double)std::rand()/RAND_MAX, // big explosion
                    //100*(double)std::rand()/RAND_MAX, // small explostion
                    .1,
                    false,
                    0
                };
                newParticles.push_back(pNew);
            }
            m_particles.erase(it);
            m_nFireWorks -= 1;
        }
        else 
        {
            Particle newParticle = p;
            
            if (newParticle.isFireWork)
                newParticle.explodeTime = p.explodeTime-dt;
            
            // update speed and position
            newParticle.position += p.speed*dt;
            
            if (!newParticle.isFireWork)
            {
                newParticle.lifeTime = 0.01;
                newParticle.speed += p.isCamCoord ? Vector3({0,0.01,0}) : (g*dt*(1-p.damping));
                newParticle.speed *= (1-p.damping);
                newParticle.size *= 0.9;
                newParticle.isCamCoord = p.isCamCoord;
                // Some random speed direction
                if (newParticle.size > 0.5)
                {
                    // RotVec dir({(double)std::rand()/RAND_MAX-0.5, 
                    //             (double)std::rand()/RAND_MAX-0.5, 
                    //             (double)std::rand()/RAND_MAX-0.5});                    
                    // dir *= 0.5;
                    // newParticle.speed = dir.asRotMat()*newParticle.speed;
                }
                if (newParticle.position.data()[2] <= 0)
                {
                    
                    newParticle.position.data()[2] = 0;
                    // bounce
                    // reflected_vector = normalized_incoming_vector - 2 * dot_product * normal_vector
                    newParticle.speed = newParticle.speed - 2.0*newParticle.speed.data()[2]*Vector3({0,0,1});
                    
                    newParticle.speed *= 0.8;
                }
                else if (newParticle.position.data()[0] >= 5)
                {
                    // bounce
                    newParticle.position.data()[0] = 5;
                    // reflected_vector = normalized_incoming_vector - 2 * dot_product * normal_vector
                    newParticle.speed = newParticle.speed - 2.0*newParticle.speed.data()[0]*Vector3({-1.0,0,0});
                    newParticle.speed *= 0.8;
                }
                else if (!newParticle.isCamCoord)
                {
                    // bounce on camera plane
                    Vector3 cameraToPoint = newParticle.position - cam.t();
                    auto camera = cameraRotInv*cameraToPoint;
                    if (camera[2] < 1.0)
                    {
                        newParticle.position += -1.0*newParticle.speed;
                        cameraToPoint = newParticle.position - cam.t();
                        camera = cameraRotInv*cameraToPoint;
                        if (camera[2] > 0)
                        {
                            newParticle.lifeTime = .01;
                            newParticle.position = camera - p.speed;
                            newParticle.position = newParticle.position.norm()*.2;
                            newParticle.speed *= 0;
                            newParticle.isCamCoord = true;
                        }
                    }
                }
                //newParticle.color[1] = std::min(255.0/newParticle.size+150, (double)newParticle.color[0]);
                if (newParticle.size > 1 && (newParticle.color[0] > 10 || newParticle.color[1] > 10 || newParticle.color[1] > 10))
                {
                    newParticles.push_back(newParticle);
                }
            }
            else
            {
                newParticle.lifeTime = 0.02;
                if (newParticle.position.data()[2] <= 0)
                {
                    newParticle.position.data()[2] = 0;
                    
                    // bounce
                    //newParticle.speed = newParticle.speed - 2.0*newParticle.speed.data()[2]*Vector3({0,0,1.0});

                    // explode directly
                    newParticle.explodeTime = 0;
                }
                else if (newParticle.position.data()[0] >= 5)
                {
                    newParticle.position.data()[0] = 5;
                    // bounce
                    // reflected_vector = normalized_incoming_vector - 2 * dot_product * normal_vector
                    //newParticle.speed = newParticle.speed - 2.0*newParticle.speed.data()[0]*Vector3({1.0,0,0});
                    // explode directly
                    newParticle.explodeTime = 0;
                }
                
                
                newParticle.speed += g*dt;
                newParticles.push_back(newParticle);        
            }
            
            p.lifeTime -= dt;
            p.size *= 0.9;
            //p.color[0] *= 0.9;
            //p.color[1] *= 0.9;
            //p.color[2] *= 0.9;

            Vector3 camera = p.position;
            if (!p.isCamCoord)
            {
                Vector3 cameraToPoint = p.position - cam.t();
                camera = cameraRotInv*cameraToPoint;
            }
            
            if (camera.data()[2] > 0)
            {
                Point3 cameraP(camera.data()[0], camera.data()[1], camera.data()[2]);
                ImgPoint imgP = cam.project(cameraP);
                
                int n = 2;
                for (int k=n; k>0; k--)
                {
                    float op = 1.0/(float)k;
                    auto radius = std::min(20.0, (p.size*k)/camera.len());
                    img.draw_circle(imgP.first, imgP.second, radius, p.color, op);
                }
            }
            
            ++it;
        }
    }

    for (auto& p : newParticles)
        addParticle(p);
    m_elapsed += dt;
    if (m_particles.size() == 0)
    {
        addFireWork();
    }
}

ParticleSim::ParticleList& ParticleSim::particles()
{
    // TODO: insert return statement here
    return m_particles;
}
