#include "engine.h"
#include "rotation.h" // TODO: remove
#include <cmath>

using namespace GE;

Engine::Engine()
{
    m_camera = Camera(640, 480, 320, 240);
    m_camera.t() = Translation({0,-5,1});
    setCameraRot(0, -M_PI/2, 0);
}

void Engine::addObject(EngineObject obj)
{
    m_objects.push_back(obj);
}

ImgPointList& Engine::projectObjectPoints(EngineObject& obj, ImgPointList& imgPoints)
{
    auto cameraRotInv = m_camera.rot().inv(); // precalculate inverse camera rotation
    // std::cout << "Camera:\n" << m_camera.rot().str() << "\n" << m_camera.t().str() << std::endl;
    // std::cout << "Object:\n" << obj.rot().str() << "\n" << obj.t().str() << std::endl;
    for (auto p : obj.points())
    {
        // transform object points to world frame
        Vector3 world = obj.t() + obj.rot()*p;
        // transform world points to camera frame
        Vector3 cameraToPoint = world - m_camera.t();
        Vector3 camera = cameraRotInv*cameraToPoint;
        // project to image (only if z>0)
        if (camera.data()[2] > 0)
        {
            Point3 cameraP(camera.data()[0], camera.data()[1], camera.data()[2]);
            ImgPoint imgP = m_camera.project(cameraP);
            imgPoints.push_back(imgP);
        }
    }

    return imgPoints;
}

void Engine::draw(Img& img)
{
    //const unsigned char blue[] = { 0,0,255 }, red[] = { 128,0,0 };

    for (auto obj : m_objects)
    {
        ImgPointList imgPoints;
        projectObjectPoints(obj, imgPoints);
        for (size_t i = 0; i < imgPoints.size(); ++i) 
        {
            ImgPoint imgP = imgPoints[i];
            //img.draw_circle(imgP.first, imgP.second, 3, obj.color(), 1);
            for (size_t j = i + 1; j < imgPoints.size(); ++j) 
            {
                
                ImgPoint imgPNext = imgPoints[j];
                img.draw_line(imgP.first, imgP.second, imgPNext.first, imgPNext.second, obj.color(), 1);
            }
        }
    }
}

Camera& Engine::camera()
{
    return m_camera;
}

void Engine::moveCamera(double right, double down, double forward)
{
    m_camera.move(Translation({right, down, forward}));
}

void Engine::rotateCamera(double yaw, double pitch, double roll)
{
    m_camera.rotate(yaw, pitch, roll);
}

void Engine::setCameraRot(double yaw, double pitch, double roll)
{
    m_camera.rot() = matx::Rotation::eulerToRotMat(pitch, yaw, roll);
}