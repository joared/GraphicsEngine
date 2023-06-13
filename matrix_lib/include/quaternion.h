#ifndef QUATERNION
#define QUATERNION

#include <cmath>

// Hamilton convention
template <typename DATA_TYPE>
class Quaternion
{
    public:
        // Unit identity quaternion
        constexpr Quaternion(): m_x(0), m_y(0), m_z(0), m_w(1) {}

        // Set it yourself
        constexpr Quaternion(DATA_TYPE x, DATA_TYPE y, DATA_TYPE z, DATA_TYPE w)
        : m_x(x), m_y(y), m_z(z), m_w(w) {}

        constexpr unsigned int len()
        {
            return std::sqrt(m_x*m_x + m_y*m_y + m_z*m_z + m_w*m_w);
        }

        constexpr void normalize()
        {
            auto lInv = 1/len();
            m_x *= lInv; m_y *= lInv; m_z *= lInv; m_w *= lInv;
        }

        // Quaternion product
        constexpr Quaternion product(Quaternion q, Quaternion p)
        {
            // Equation (1) in https://github.com/joared/quaternion_eskf/blob/main/quaternion_eskf.pdf
            DATA_TYPE x =  q.w()*p.x() - q.z()*p.y() + q.y()*p.z() + q.x()*p.w();
            DATA_TYPE y =  q.z()*p.x() + q.w()*p.y() - q.x()*p.z() + q.y()*p.w();
            DATA_TYPE z = -q.y()*p.x() + q.x()*p.y() + q.w()*p.z() + q.z()*p.w();
            DATA_TYPE w = -q.x()*p.x() - q.y()*p.y() - q.z()*p.z() + q.w()*p.w();
            
            return Quaternion(x, y, z, w);
        }

        // Getters
        constexpr const DATA_TYPE& x() const { return m_x; }
        constexpr const DATA_TYPE& y() const { return m_y; }
        constexpr const DATA_TYPE& z() const { return m_z; }
        constexpr const DATA_TYPE& w() const { return m_w; }

        // Setters
        constexpr DATA_TYPE& x() { return m_x; }
        constexpr DATA_TYPE& y() { return m_y; }
        constexpr DATA_TYPE& z() { return m_z; }
        constexpr DATA_TYPE& w() { return m_w; }

    private:
        DATA_TYPE m_x;
        DATA_TYPE m_y;
        DATA_TYPE m_z;
        DATA_TYPE m_w;
};

template<typename DATA_TYPE>
constexpr Quaternion<DATA_TYPE> operator*(const DATA_TYPE& d, const Quaternion<DATA_TYPE>& q)
{
    DATA_TYPE x = q.x()*d; DATA_TYPE y = q.y()*d; DATA_TYPE z = q.z()*d; DATA_TYPE w = q.w()*d;
    return Quaternion<DATA_TYPE>(x, y, z, w);
}

template<typename DATA_TYPE>
constexpr Quaternion<DATA_TYPE> operator*(const Quaternion<DATA_TYPE>& q, const DATA_TYPE& d)
{
    DATA_TYPE x = q.x()*d; DATA_TYPE y = q.y()*d; DATA_TYPE z = q.z()*d; DATA_TYPE w = q.w()*d;
    return Quaternion<DATA_TYPE>(x, y, z, w);
}

typedef Quaternion<double> Quaterniond;
typedef Quaternion<float> Quaternionf;

#endif /* QUATERNION */
