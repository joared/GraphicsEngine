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
        constexpr RotMat() : Matrix33d() 
        { 
            for (unsigned int x = 0; x < 3; ++x)
            {
                this->operator()(x, x) = 1.0;
            }    
        } // Constructor
        constexpr RotMat(const Matrix33d & mat) : Matrix33d(mat) {}       // Copy constructor
         //RotMat& operator=(const RotMat& rot);
        RotMat inv() { return transpose(); }
};

} /*matx*/

#endif /* ROTMAT */
