#ifndef ROTATION
#define ROTATION

#include "quaternion.h"
#include "rotmat.h"
#include "euler_angles.h"

namespace matx
{
    
// Represents rotation where the internal representation is a Quaternions

class Rotation
{
    public:
        constexpr static EulerAngles<double> rotMatToEuler(RotMat& r)
        {
            return EulerAngles<double>();
        }

        constexpr static RotMat eulerToRotMat(double x, double y, double z)
        {
            double cx = std::cos(x);
            double sx = std::sin(x);
            double cy = std::cos(y);
            double sy = std::sin(y);
            double cz = std::cos(z);
            double sz = std::sin(z);

            RotMat rx
            (
                {1, 0, 0,
                 0, cx, -sx,
                 0, sx, cx}
            );
            RotMat ry
            (
                {cy, 0, sy,
                 0,  1,  0,
                 -sy, 0, cy}
            );
            RotMat rz
            (
                {cz, -sz, 0,
                 sz, cz, 0,
                 0, 0, 1}
            );

            //return ry*rx*rz;
            return rz*rx*ry;
        }

        constexpr Rotation() : m_q() {}


    private:
        Quaterniond m_q;
};

}

#endif /* ROTATION */
