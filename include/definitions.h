#ifndef DEFINITIONS
#define DEFINITIONS

#include "vector.h"
#include "rotmat.h"
#include "point.h"

#include <utility>
#include <vector>
#include "CImg.h"

namespace GE
{

typedef matx::Point3d Point3;
typedef matx::Vector3d Vector3;
typedef std::vector<Vector3> PointList; // TODO: shouldnt be Vector3

typedef matx::Vector3d Translation;
typedef matx::RotMat RotMat;

typedef cimg_library::CImg<unsigned char> Img;
typedef std::pair<int, int> ImgPoint; // x, y
typedef std::vector<std::pair<int, int>> ImgPointList;
using Color = unsigned char[3];

}

#endif /* DEFINITIONS */
