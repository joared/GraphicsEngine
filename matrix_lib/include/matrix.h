#ifndef MATRIX
#define MATRIX

#include <cstring>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>

namespace matx
{

template <typename DATA_TYPE, unsigned int ROWS, unsigned int COLS>
class Matrix
{
    public:
        static constexpr Matrix Identity() 
        {
            Matrix m;
            for (unsigned int x = 0; x < std::min(COLS, ROWS); ++x)
            {
                m(x, x) = static_cast<DATA_TYPE>(1.0);
            }
            return m;
        }

        constexpr Matrix() { std::memset(m_data, 0, sizeof(m_data)); } // Constructor
        
        constexpr Matrix(const Matrix & mat) 
        { 
            for (unsigned int x=0; x<ROWS*COLS; ++x)
                m_data[x] = mat.m_data[x];
        }

        constexpr Matrix(const std::initializer_list<DATA_TYPE>& l) 
        {
            int i=0;
            for (auto it=l.begin(); it<l.end(); ++it, ++i)
                m_data[i] = *it;
        }

        constexpr Matrix(DATA_TYPE* data) 
        { 
            for (unsigned int x = 0; x < ROWS * COLS; ++x)
                m_data[x] = data[x];
        }

        constexpr const DATA_TYPE* data() const { return m_data; }
        constexpr DATA_TYPE* data() { return m_data; }

        // Element get and set
        constexpr const DATA_TYPE& operator()(unsigned int row, unsigned int col) const { return m_data[row*COLS + col]; }
        constexpr DATA_TYPE& operator()(unsigned int row, unsigned int col) { return m_data[row*COLS + col]; }
        constexpr const DATA_TYPE& operator[](unsigned int i) const { return m_data[i]; }
        constexpr DATA_TYPE& operator[](unsigned int i) { return m_data[i]; }

        // Transpose
        constexpr Matrix<DATA_TYPE, COLS, ROWS> transpose() const
        {
            Matrix<DATA_TYPE, COLS, ROWS> res;
            for (unsigned int r = 0; r < ROWS; ++r)
            for (unsigned int c = 0; c < COLS; ++c)
                 res(r, c) = m_data[(c * ROWS) + r];
            return res;
        }

        constexpr Matrix& operator+=(const Matrix& m)
        {
            for (unsigned int i = 0; i<ROWS*COLS; ++i)
                m_data[i] += m.m_data[i];

            return *this;
        }

        constexpr Matrix& operator*=(const DATA_TYPE& d)
        {
            for (unsigned int i = 0; i<ROWS*COLS; ++i)
                m_data[i] *= d;

            return *this;
        }

        std::string str() 
        {
            std::stringstream ss;
            for (size_t r=0; r<ROWS; r++)
            {
                for (size_t c=0; c<COLS; c++)
                {
                    ss << std::setprecision(3) << this->operator()(r, c);
                    ss << " ";
                }
                ss << "\n";
            }
            return ss.str();
        }
    // // Different names for rows and cols to prevent "shadowing"
    // template <int r, int c>
    // friend inline Matrix operator*(Matrix m1, Matrix m2);

    protected:
        DATA_TYPE m_data[ROWS*COLS];

};

// Matrix multiplication for same size matricies
template <typename DATA_TYPE, unsigned int ROWS, unsigned int COLS>
constexpr Matrix<DATA_TYPE, ROWS, COLS> operator*(const Matrix<DATA_TYPE, ROWS, COLS>& m1, const Matrix<DATA_TYPE, ROWS, COLS>& m2)
{
    Matrix<DATA_TYPE, ROWS, COLS> res;
    for (unsigned int r=0; r<ROWS; r++)
        for (unsigned int c=0; c<COLS; c++)
            for (unsigned int i=0; i<COLS; i++)
                res(r, c) += m1(r, i) * m2(i, c);
    return res;
}

// Matrix addition for same size matricies
template <typename DATA_TYPE, unsigned int ROWS, unsigned int COLS>
constexpr Matrix<DATA_TYPE, ROWS, COLS> operator+(const Matrix<DATA_TYPE, ROWS, COLS>& m1, const Matrix<DATA_TYPE, ROWS, COLS>& m2)
{
    Matrix<DATA_TYPE, ROWS, COLS> res;
    for (unsigned int i = 0; i<ROWS*COLS; ++i)
        res.data()[i] = m1.data()[i] + m2.data()[i];
    return res;
}

// Matrix subtraction for same size matricies
template <typename DATA_TYPE, unsigned int ROWS, unsigned int COLS>
constexpr Matrix<DATA_TYPE, ROWS, COLS> operator-(const Matrix<DATA_TYPE, ROWS, COLS>& m1, const Matrix<DATA_TYPE, ROWS, COLS>& m2)
{
    Matrix<DATA_TYPE, ROWS, COLS> res;
    for (unsigned int i = 0; i<ROWS*COLS; ++i)
        res.data()[i] = m1.data()[i] - m2.data()[i];
    return res;
}

// Value multiplication for matrices
template <typename DATA_TYPE, unsigned int ROWS, unsigned int COLS>
constexpr Matrix<DATA_TYPE, ROWS, COLS> operator*(const DATA_TYPE& v, const Matrix<DATA_TYPE, ROWS, COLS>& m)
{
    Matrix<DATA_TYPE, ROWS, COLS> res = m;
    for (unsigned int r=0; r<ROWS; r++)
        for (unsigned int c=0; c<COLS; c++)
            res(r, c) *= v;
    return res;
}

// Value multiplication for matrices
template <typename DATA_TYPE, unsigned int ROWS, unsigned int COLS>
constexpr Matrix<DATA_TYPE, ROWS, COLS> operator*(const Matrix<DATA_TYPE, ROWS, COLS>& m, const DATA_TYPE& v)
{
    Matrix<DATA_TYPE, ROWS, COLS> res = m;
    for (unsigned int r=0; r<ROWS; r++)
        for (unsigned int c=0; c<COLS; c++)
            res(r, c) *= v;
    return res;
}

typedef Matrix<double, 3, 3> Matrix33d;

} /*matx*/

#endif /* MATRIX */
