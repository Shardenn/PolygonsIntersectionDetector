#include "utils.h"

ObjIntUtils::Utils::Utils()
{

}

bool ObjIntUtils::Utils::fuzzyCompare(const QVector<QVector3D> &a,
                                      const QVector<QVector3D> &b)
{
    if (a.size() != b.size())
        return false;

    for (int i = 0; i < a.size(); i++) {
        if (!qFuzzyCompare(a[i], b[i]))
            return false;
    }
    return true;
}

bool ObjIntUtils::Utils::fuzzyCompare(const QVector<QVector2D> &a,
                                      const QVector<QVector2D> &b)
{
    if (a.size() != b.size())
        return false;

    for (int i = 0; i < a.size(); i++) {
        if (!qFuzzyCompare(a[i], b[i]))
            return false;
    }
    return true;
}

bool ObjIntUtils::Utils::fuzzyCompare(const float &a,
                                      const float &b)
{
    return qFuzzyCompare(a, b);
}
