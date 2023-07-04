#ifndef ROTVEC
#define ROTVEC

#include "vector.h"
#include <cmath>

namespace matx
{
    template <typename DATA_TYPE>
    constexpr Matrix<DATA_TYPE, 3, 3> skew(const Vector<DATA_TYPE, 3>& v)
    {
        return Matrix<DATA_TYPE, 3, 3>
        (
            {   0,  -v[2],   v[1],
             v[2],      0,  -v[0],
             -v[1],   v[0],     0
            }
        );
    }

    template <typename DATA_TYPE>
    class RotVec : public Vector<DATA_TYPE, 3>
    {
        public:
            using Vector<DATA_TYPE, 3> :: Vector;
            constexpr Matrix<DATA_TYPE, 3, 3> asRotMat()
            {
                DATA_TYPE a = Vector<DATA_TYPE, 3>::len();
                auto s = skew(Vector<DATA_TYPE, 3>::norm());
                return Matrix<DATA_TYPE, 3, 3>::Identity() + std::sin(a)*s + (1.0-std::cos(a))*(s*s);
            }
    };

    typedef RotVec<double> RotVecd;
}

#endif /* ROTVEC */
