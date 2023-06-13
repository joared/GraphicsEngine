#ifndef POINT
#define POINT

#include "vector.h"

namespace matx
{

// TODO: use some kind of VecBase?
class Point2d
{
    public:
        constexpr Point2d(double x, double y) : x(x), y(y) {}
        double x;
        double y;
};

class Point3d
{
    public:
        constexpr Point3d(double x, double y, double z) : x(x), y(y), z(z) {}
        double x;
        double y;
        double z;
};

}

#endif /* POINT */
