#ifndef ENGINE
#define ENGINE

#include "definitions.h"
#include "object.h"
#include "camera.h"

namespace GE
{
    class Engine
    {
        public:
            Engine();
            virtual void addObject(EngineObject obj);
            virtual ImgPointList& projectObjectPoints(EngineObject& obj, ImgPointList& imgPoints);
            virtual void draw(Img& img);
            virtual Camera& camera();
            virtual void moveCamera(double right, double down, double forward);
            virtual void rotateCamera(double yaw, double pitch, double roll);
            virtual void setCameraRot(double yaw, double pitch, double roll);
        private:
            Camera m_camera;
            ObjectList m_objects;
    };

}

#endif /* ENGINE */
