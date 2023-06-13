#ifndef EULER_ANGLES
#define EULER_ANGLES

namespace matx
{

enum EulerOrder 
{
    XYZ,
    ZYX,
    ZXY,
    YXZ // Useful for yaw/pitch/roll of camera
};

template <typename DATA_TYPE>
class EulerAngles
{
    public:
        constexpr EulerAngles() : m_x(0), m_y(0), m_z(0) {}
        DATA_TYPE& x() { return m_x; }
        DATA_TYPE& y() { return m_y; }
        DATA_TYPE& z() { return m_z; }
        const DATA_TYPE& x() const { return m_x; }
        const DATA_TYPE& y() const { return m_y; }
        const DATA_TYPE& z() const { return m_z; }
    private:
        DATA_TYPE m_x;
        DATA_TYPE m_y;
        DATA_TYPE m_z;
};

}

#endif /* EULER_ANGLES */
