#include "triangulator.h"
#include <memory>

Triangulation::Triangulator::Triangulator()
{

}

Triangulation::Triangulator::~Triangulator()
{}

Triangulation::NaiveTriangulator::NaiveTriangulator() :
    Triangulator ()
{}

Triangulation::NaiveTriangulator::~NaiveTriangulator()
{}

QVector<int> Triangulation::NaiveTriangulator::triangulate(QVector<int> indices,
                                                           QVector<int> offsets)
{
    QVector<int> res{};

    for (int i = 0; i < offsets.size() - 1; ++i) {
        const int firstVertInd = offsets[i];
        const int numVerts     = offsets[i + 1] - firstVertInd;

        if (numVerts < 1 || firstVertInd < 0  ||
                firstVertInd > indices.size() ||
                firstVertInd + numVerts > indices.size())
            continue;

        int commonVert = indices[firstVertInd];
        for (int j = 1; j < numVerts - 1; ++j) {
            const int currVert = indices[firstVertInd + j];
            const int nextVert = indices[firstVertInd + j + 1];
            res.append({commonVert, currVert, nextVert});
        }
    }

    return res;
}

Triangulation::EarClippingTriangulator::EarClippingTriangulator() :
    Triangulator()
{}

Triangulation::EarClippingTriangulator::~EarClippingTriangulator()
{}

QVector<int> Triangulation::EarClippingTriangulator::triangulate(QVector<int> indices,
                                                                 QVector<int> offsets)
{

}

QVector<QVector3D> Triangulation::EarClippingTriangulator::earClip(const QVector<QVector3D> &polygon)
{

}

bool Triangulation::EarClippingTriangulator::isVertexConvex(const QVector3D previousVertex, const QVector3D vertex, const QVector3D nextVertex)
{

}

bool Triangulation::EarClippingTriangulator::isPointInsideTriangle(const QVector3D point,
                                                    const QVector3D a,
                                                    const QVector3D b,
                                                    const QVector3D c)
{
    /*
     * The approach used in the function is described here:
     * https://www.youtube.com/watch?v=HYAgJN3x4GA
     * */

    float w1, w2;
    w1 = (a.x() * (c.y() - a.y()) +
          (point.y() - a.y()) * (c.x() - a.x()) - point.x() * (c.y() - a.y())) /
            ((b.y() - a.y()) * (c.x() - a.x()) - (b.x() - a.x()) * (c.y() - a.y()) );

    w2 = (point.y() - a.y() - w1 * (b.y() - a.y()) ) /
            (c.y() - a.y());

    if (w1 >= 0 && w2 >= 0 && w1 + w2 <= 1)
        return true;

    return false;
}
