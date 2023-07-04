#include <iostream>
#include "CImg.h"
#include "engine.h"
#include "object.h"
#include "particle_sim.h"


using namespace cimg_library;

int main()
{
    std::cout << "hello!" << std::endl;

    Engine engine;
    engine.addObject(EngineObject::Grid());
    auto xWall = EngineObject::Grid();
    xWall.rot() = matx::Rotation::eulerToRotMat(0, 1.57, 0);
    xWall.t()[0] = 5;
    engine.addObject(xWall);
    engine.addObject(EngineObject());

    ParticleSim particleSim;

    CImg<unsigned char> img(640,480,1,3,255);
    CImgDisplay disp(img,"[#21] - Fireworks (Click to add/explode rockets)");
    const unsigned char white[] = { 255,255,255 }, red[] = { 128,0,0 };

    double inc = 0.06;
    double incRot = 0.03;
    int delay = 25;
    double dt = (double)delay/1000.0;
    while (!disp.is_closed() && !disp.is_keyESC() ) //!disp.is_keyQ() 
    {
        img = 290;
        
        img.draw_text(5,5," %u frames/s ",white,red,0.5f,13,(unsigned int)disp.frames_per_second());

        engine.draw(img);
        particleSim.update(img, dt, engine.camera());
        
        disp.display(img).wait(delay);
        if (disp.is_keyCTRLLEFT() && disp.is_keyF()) disp.resize(640,480,false).toggle_fullscreen(false);
        if (disp.is_resized()) disp.resize(disp,false);

        if (disp.is_keyW()) engine.moveCamera(0, 0, inc);
        if (disp.is_keyS()) engine.moveCamera(0, 0, -inc);
        if (disp.is_keyD()) engine.moveCamera(inc, 0, 0);
        if (disp.is_keyA()) engine.moveCamera(-inc, 0, 0);
        
        // if (disp.is_keyARROWUP()) pitch -= incRot;
        // if (disp.is_keyARROWDOWN()) pitch += incRot;
        // if (disp.is_keyARROWLEFT()) yaw -= incRot;
        // if (disp.is_keyARROWRIGHT()) yaw += incRot;
        // engine.setCameraRot(yaw, pitch, roll);
        if (disp.is_keyARROWUP()) engine.rotateCamera(0, -incRot, 0);
        if (disp.is_keyARROWDOWN()) engine.rotateCamera(0, incRot, 0);
        if (disp.is_keyQ()) engine.rotateCamera(RotVec({0,0,incRot}));
        if (disp.is_keyE()) engine.rotateCamera(-1.0*RotVec({0,0,incRot})); // Weird bug if using constexpr camera.rotate while also pressing space
        if (disp.button()&2) particleSim.addFireWork();
        if (disp.button()&1 || disp.is_keySPACE()) 
        {
            ParticleSim::Particle p 
            {
                engine.camera().t()+Vector3({0,0,-0.8}),
                Vector3({engine.camera().rot()(0,2),engine.camera().rot()(1,2),engine.camera().rot()(2,2)})*20.0,
                0,
                {255,255,55},
                50,
                0.01,
                true,
                2.0//*(double)std::rand()/RAND_MAX + 1.5
            };
            particleSim.addFireWork(p);
        }
        if (disp.is_keyF())
        {
            for (auto& p : particleSim.particles())
            {
                if (p.isFireWork)
                {
                    engine.lookAt(p.position);
                }
            }
        }
        
        //engine.camera().t().data()[2] = 1.8;
    }
  return 0;
  
}