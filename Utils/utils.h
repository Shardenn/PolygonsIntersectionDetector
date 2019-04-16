#ifndef OBJECTS_INTERSECTIONS_UTILS_H
#define OBJECTS_INTERSECTIONS_UTILS_H

#include <QVector>
#include <QVector3D>

namespace ObjIntUtils {

class Utils
{
public:
    Utils();

    static bool fuzzyCompare(const QVector<QVector3D> &a,
                             const QVector<QVector3D> &b);

    static bool fuzzyCompare(const QVector<QVector2D> &a,
                             const QVector<QVector2D> &b);

    static bool fuzzyCompare(const float &a,
                             const float &b);
};

}

#endif // UTILS_H
