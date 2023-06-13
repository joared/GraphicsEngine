#ifndef OBJECT
#define OBJECT

#include "definitions.h"
#include "vector"

namespace GE
{
    class EngineObject
    {
        public:
            EngineObject();
            EngineObject(PointList points);
            static EngineObject Grid();
            const PointList& points() const { return m_points; }
            PointList& points() { return m_points; }
            unsigned char* color();

            const RotMat& rot() const { return m_r; };
            RotMat& rot() { return m_r; };
            const Translation& t() const { return m_t; }
            Translation& t() { return m_t; }
        protected:
            Color m_color;
            PointList m_points;
            RotMat m_r;
            Translation m_t;
    };

    typedef std::vector<EngineObject> ObjectList;
}

#endif /* OBJECT */
