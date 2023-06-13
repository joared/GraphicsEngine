#include <gtest/gtest.h>
#include "rotmat.h"

TEST(RotMatTest, SimpleTest)
{
    matx::RotMat rot = matx::Matrix33d({1,2,3,4,5,6,7,8,9});
    std::cout << rot.str() << std::endl;
    //matx::RotMat rot2 = rot;
};