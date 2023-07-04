#include <gtest/gtest.h>
#include "rotvec.h"

TEST(RotVecTest, SimpleTest)
{
    matx::RotVecd r = matx::Vector3d({1,2,3});
    std::cout << r.str() << std::endl;
    //matx::RotMat rot2 = rot;
};