#include "engine.h"
#include "rotation.h" // TODO: remove
#include <cmath>

using namespace GE;

Engine::Engine()
{
    float scale = 1.0;
    m_camera = Camera(640*scale, 480*scale, 320, 240);
    m_camera.t() = Translation({0,-5,1});
    setCameraRot(0, -M_PI/2, 0);
}

void Engine::addObject(EngineObjectPtr obj)
{
    m_objects.push_back(obj);
}

PointList& Engine::transformObjectPoints(EngineObjectPtr obj, PointList& camPoints)
{
    auto cameraRotInv = m_camera.rot().inv(); // precalculate inverse camera rotation
    // std::cout << "Camera:\n" << m_camera.rot().str() << "\n" << m_camera.t().str() << std::endl;
    // std::cout << "Object:\n" << obj.rot().str() << "\n" << obj.t().str() << std::endl;
    for (auto& p : obj->points())
    {
        // transform object points to world frame
        Vector3 world = obj->t() + obj->rot()*p;
        // transform world points to camera frame
        Vector3 cameraToPoint = world - m_camera.t();
        Vector3 camera = cameraRotInv*cameraToPoint;
        // project to image (only if z>0)
        camPoints.push_back(camera);
    }

    return camPoints;
}

void Engine::draw(Img& img)
{
    //const unsigned char blue[] = { 0,0,255 }, red[] = { 128,0,0 };

    for (auto& obj : m_objects)
    {
        PointList camPoints;
        transformObjectPoints(obj, camPoints);

        if (!obj->draw(img, camPoints, m_camera))
        {
            for (size_t i = 0; i < camPoints.size(); ++i) 
            {
                Vector3 camP = camPoints[i];
                if (camP.data()[2] > 0)
                {
                    Point3 cameraP(camP.data()[0], camP.data()[1], camP.data()[2]);
                    ImgPoint imgP = m_camera.project(cameraP);
                    img.draw_circle(imgP.first, imgP.second, 5.0/camP.len(), obj->color(), 1);
                }
                
                // for (size_t j = i + 1; j < imgPoints.size(); ++j) 
                // {
                    
                //     ImgPoint imgPNext = imgPoints[j];
                //     img.draw_line(imgP.first, imgP.second, imgPNext.first, imgPNext.second, obj.color(), 1);
                // }
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
    m_camera.t()[2] = 1.8;
}

void Engine::lookAt(const Vector3& point)
{
    // z-axis towards point
    Vector3 zAxis = point - m_camera.t();
    zAxis = zAxis.norm();
    // x-axis parallell to xy-plane, to the right.
    // Project z-axis to the xy-plane and rotate to the right.
    Vector3 proj({zAxis[0], zAxis[1], 0.0});
    proj = proj.norm();
    Vector3 xAxis({-proj[1], proj[0], 0.0});
   // auto rotMat = matx::Rotation::eulerToRotMat(0, 0, -1.57);
    //Vector3 xAxis = rotMat*proj;
    xAxis = xAxis.norm();
    // y-axis is simply the cross-product
    Vector3 yAxis = matx::skew(zAxis) * xAxis;
    yAxis = yAxis.norm();

    RotMat camRot({xAxis[0], xAxis[1], xAxis[2], yAxis[0], yAxis[1], yAxis[2], zAxis[0], zAxis[1], zAxis[2]});
    m_camera.rot() = camRot;

}

void Engine::rotateCamera(double yaw, double pitch, double roll)
{
    m_camera.rotate(yaw, pitch, roll);
}

void Engine::rotateCamera(RotVec r)
{
    m_camera.rotate(r);
}

void Engine::setCameraRot(double yaw, double pitch, double roll)
{
    m_camera.rot() = matx::Rotation::eulerToRotMat(pitch, yaw, roll);
}