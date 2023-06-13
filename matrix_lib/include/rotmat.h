#ifndef ROTMAT
#define ROTMAT

#include "matrix.h"

namespace matx
{

class RotMat : public Matrix33d
{
    using Matrix33d::Matrix;

    public:
        // Rule of ?
        constexpr RotMat() : Matrix33d() { *this=Matrix33d::Identity(); } // Constructor
        constexpr RotMat(const Matrix33d & mat) : Matrix33d(mat) {}       // Copy constructor
         //RotMat& operator=(const RotMat& rot);
        RotMat inv() { return transpose(); }
};

} /*matx*/

#endif /* ROTMAT */
