#include "object.h"

namespace GE
{
    EngineObject::EngineObject() : m_color{0, 0, 255}
    {
        double scale = 0.1;
        m_points.push_back(Vector3({1, 1, 0}));
        m_points.push_back(Vector3({-1, 1, 0}));
        m_points.push_back(Vector3({1, -1, 0}));
        m_points.push_back(Vector3({-1, -1, 0}));
        
        m_points.push_back(Vector3({1, 1, 2}));
        m_points.push_back(Vector3({-1, 1, 2}));
        m_points.push_back(Vector3({1, -1, 2}));
        m_points.push_back(Vector3({-1, -1, 2}));
        for (auto& p : m_points)
            p *= scale;
    }
    
    EngineObject::EngineObject(PointList points) : m_color{128, 0, 0}
    {
        m_points = points;
    }

    EngineObject EngineObject::Grid()
    {
        PointList points;
        int n = 50;
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                points.push_back(Vector3({(double)(i-n/2), (double)(j-n/2), 0}));
            }
        }
        return EngineObject(points);
    }

    unsigned char* EngineObject::color()
    {
        return m_color;
    }
}