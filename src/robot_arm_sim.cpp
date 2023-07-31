#include <iostream>
#include "CImg.h"
#include "engine.h"
#include "object.h"
#include "particle_sim.h"
#include <math.h>

using namespace cimg_library;

class Arm : public EngineObject
{
    public:
        using EngineObject::EngineObject;
        virtual bool draw(Img& img, PointList& camPoints, Camera& camera) override
        {
            //assert(camPoints.size() == 2);
            ImgPoint imgP1 = camera.project(Point3(camPoints[0][0], camPoints[0][1], camPoints[0][2]));
            ImgPoint imgP2 = camera.project(Point3(camPoints[1][0], camPoints[1][1], camPoints[1][2]));
            ImgPoint imgP3 = camera.project(Point3(camPoints[2][0], camPoints[2][1], camPoints[2][2]));
            ImgPoint imgP4 = camera.project(Point3(camPoints[3][0], camPoints[3][1], camPoints[3][2]));
            img.draw_line(imgP1.first, imgP1.second, imgP2.first, imgP2.second, color());
            img.draw_line(imgP2.first, imgP2.second, imgP3.first, imgP3.second, color());
            img.draw_line(imgP3.first, imgP3.second, imgP4.first, imgP4.second, color());
            img.draw_line(imgP4.first, imgP4.second, imgP1.first, imgP1.second, color());
            return true;
        }
};

class RobotArm
{
    public:
        RobotArm(double l1, double l2) : m_l1(l1), m_l2(l2) 
        {
            double s = 10;
            m_arm1 = std::make_shared<Arm>(PointList({
                {0, l1/s, 0}, 
                {0, -l1/s, 0}, 
                {l1, -l1/s, 0},
                {l1, l1/s, 0}}));
            m_arm2 = std::make_shared<Arm>(PointList({
                {0, l2/s, 0}, 
                {0, -l2/s, 0}, 
                {l2, -l2/s, 0},
                {l2, l2/s, 0}}));
            m_theta1 = 0;
            m_theta2 = 0;
            forwardKinematics(m_theta1, m_theta2);
        }
        std::pair<double, double> forwardKinematics(double theta1, double theta2)
        {
            m_theta1 = theta1;
            m_theta2 = theta2;
            m_arm1->rot() = matx::Rotation::eulerToRotMat(0, 0, theta1);
            m_arm2->rot() = m_arm1->rot() * matx::Rotation::eulerToRotMat(0, 0, theta2);
            m_arm2->t() = m_arm1->rot() * Vector3({m_l1, 0, 0});
            auto xy = m_arm2->t() + m_arm2->rot() * Vector3({m_l2, 0, 0});
            return std::make_pair(xy[0], xy[1]);
        }
        void inverseKinematics(double x, double y)
        {
            std::cout << "x: " << x << std::endl;
            std::cout << "y: " << y << std::endl;
            auto rSquared = x*x + y*y;

            double theta2 = std::acos((rSquared - m_l1*m_l1-m_l2*m_l2)/(2*m_l1*m_l2));            
            double theta1 = std::atan2(y,x) - std::atan2(m_l2*std::sin(theta2), m_l1+m_l2*std::cos(theta2));
            double theta2Sol2 = -theta2;
            double theta1Sol2 = std::atan2(y,x) + std::atan2(m_l2*std::sin(theta2), m_l1+m_l2*std::cos(theta2));
            
            if (1)
            {
                theta2 = theta2Sol2;
                theta1 = theta1Sol2;
            }

            forwardKinematics(theta1, theta2);
        }
        EngineObjectPtr arm1() { return m_arm1; }
        EngineObjectPtr arm2() { return m_arm2; }
    private:
        double m_l1;
        double m_l2;
        double m_theta1;
        double m_theta2;

        EngineObjectPtr m_arm1;
        EngineObjectPtr m_arm2;
};

int main()
{
    std::cout << "hello!" << std::endl;
    
    Engine engine;

    double theta1 = 0;
    double theta2 = 0;
    RobotArm robotArm1(1, 1);
    engine.addObject(robotArm1.arm1());
    engine.addObject(robotArm1.arm2());
    
    RobotArm robotArm2(1, 1);
    engine.addObject(robotArm2.arm1());
    engine.addObject(robotArm2.arm2());
    robotArm2.arm1()->color()[2] = 255;
    robotArm2.arm2()->color()[2] = 255;
    auto grid = std::make_shared<EngineObject>(EngineObject::Grid());
    engine.addObject(grid);
    // auto xWall = EngineObject::Grid();
    // xWall.rot() = matx::Rotation::eulerToRotMat(0, 1.57, 0);
    // xWall.t()[0] = 5;
    // engine.addObject(xWall);

    CImg<unsigned char> img(640,480,1,3,255);
    CImgDisplay disp(img,"Robot arm");
    const unsigned char white[] = { 255,255,255 }, red[] = { 128,0,0 };

    double inc = 0.06;
    double incRot = 0.03;
    int delay = 25;
    double dt = (double)delay/1000.0;
    while (!disp.is_closed() && !disp.is_keyESC() ) //!disp.is_keyQ() 
    {
        std::cout << "Theta1: " << theta1 << std::endl;
        img = 290;
        
        img.draw_text(5,5," %u frames/s ",white,red,0.5f,13,(unsigned int)disp.frames_per_second());

        auto x = robotArm1.forwardKinematics(theta1, theta2);
        robotArm2.inverseKinematics(x.first, x.second);
        engine.draw(img);
        
        disp.display(img).wait(delay);
        if (disp.is_keyCTRLLEFT() && disp.is_keyF()) disp.resize(640,480,false).toggle_fullscreen(false);
        if (disp.is_resized()) disp.resize(disp,false);

        if (disp.is_keyW()) engine.moveCamera(0, 0, inc);
        if (disp.is_keyS()) engine.moveCamera(0, 0, -inc);
        if (disp.is_keyD()) engine.moveCamera(inc, 0, 0);
        if (disp.is_keyA()) engine.moveCamera(-inc, 0, 0);
        
        if (disp.is_keyARROWUP()) engine.rotateCamera(0, -incRot, 0);
        if (disp.is_keyARROWDOWN()) engine.rotateCamera(0, incRot, 0);
        // Weird bug if using constexpr camera.rotate while also pressing space
        if (disp.is_keyQ()) engine.rotateCamera(RotVec({0,0,incRot}));
        if (disp.is_keyE()) engine.rotateCamera(-1.0*RotVec({0,0,incRot}));
        //if (disp.is_keyE()) engine.rotateCamera(-incRot, 0, 0);
        //if (disp.is_keyQ()) engine.rotateCamera(incRot, 0, 0);
        if (disp.is_key1()) theta1 += 0.1;
        if (disp.is_key2()) theta2 += 0.1;
    }
  return 0;
  
}