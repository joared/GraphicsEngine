#ifndef VECTOR
#define VECTOR

#include <cmath>

#include "matrix.h"

namespace matx
{

// TODO: currently only column vectors are supported
// The transpose() on a vector will return a Matrix
template<typename DATA_TYPE, unsigned int SIZE>
class Vector : public Matrix<DATA_TYPE, SIZE, 1>
{   
    public:
         constexpr Vector() : Matrix<DATA_TYPE, SIZE, 1>() {}
         constexpr Vector(const Matrix<DATA_TYPE, SIZE, 1>& m) : Matrix<DATA_TYPE, SIZE, 1>(m) {}
         constexpr Vector(DATA_TYPE* data) : Matrix<DATA_TYPE, SIZE, 1>(data) {}
         constexpr Vector(const std::initializer_list<DATA_TYPE>& l) : Matrix<DATA_TYPE, SIZE, 1>(l) {}
    public:
        constexpr DATA_TYPE len()
        {
            DATA_TYPE squaredLength = 0;
            for (unsigned int i=0; i<SIZE; ++i)
            {
                auto v = this->operator()(i, 1);
                squaredLength += v*v;
                // m_d
            }

            return std::sqrt(squaredLength);
        }
};

// Matrix vector multiplication
template <typename DATA_TYPE, unsigned int SIZE, unsigned int ROWS>
constexpr Vector<DATA_TYPE, ROWS> operator*(const Matrix<DATA_TYPE, ROWS, SIZE>& mat, const Vector<DATA_TYPE, SIZE>& vec)
{
    Vector<DATA_TYPE, ROWS> res;
    for (unsigned int r=0; r<ROWS; r++)
        for (unsigned int c=0; c<SIZE; c++)
            res(r, 0) += vec(c, 0) * mat(r, c);
    return res;
}

// Dot product
template <typename DATA_TYPE, unsigned int SIZE>
constexpr DATA_TYPE operator*(const Vector<DATA_TYPE, SIZE>& v1, const Vector<DATA_TYPE, SIZE>& v2)
{
    DATA_TYPE res(0);
    for (unsigned int i=0; i<SIZE; i++)
        res += v1(i, 0) * v2(i, 0);
    return res;
}

typedef Vector<double, 3> Vector3d;

}

#endif /* VECTOR */
