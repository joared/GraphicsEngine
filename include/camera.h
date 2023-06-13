#ifndef CAMERA
#define CAMERA

#include <cmath>
#include "definitions.h"
#include "rotation.h"

using namespace GE;

class Camera
{
    public:
        constexpr Camera() : m_r(), m_t(), m_fx(-1), m_fy(-1), m_cx(-1), m_cy(-1) {}
        constexpr Camera(double fx, double fy, int cx, int cy) 
        : m_r(), m_t(), m_fx(fx), m_fy(fy), m_cx(cx), m_cy(cy) {}

        constexpr ImgPoint project(Point3 p)
        {
            double scaleInv = 1/p.z;
            double x = m_cx + m_fx*p.x*scaleInv;
            double y = m_cy + m_fy*p.y*scaleInv;
            return ImgPoint((int)x, (int)y);
        }

        constexpr void move(Translation displacement)
        {
            m_t += m_r*displacement;
        }

        constexpr void rotate(double yaw, double pitch, double roll)
        {
            RotMat delta = matx::Rotation::eulerToRotMat(pitch, yaw, roll);
            m_r = m_r*delta;
        }

        const RotMat& rot() const { return m_r; };
        RotMat& rot() { return m_r; };
        const Translation& t() const { return m_t; }
        Translation& t() { return m_t; }
        
    private:
        RotMat m_r;
        Translation m_t;
        double m_fx;
        double m_fy;
        double m_cx;
        double m_cy;
};

#endif /* CAMERA */
