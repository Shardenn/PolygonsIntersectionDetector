#include "triangulator.h"
#include <memory>

Triangulator::Triangulator()
{

}

NaiveTriangulator::NaiveTriangulator() :
    Triangulator ()
{}

void NaiveTriangulator::triangulate(MeshData &mesh)
{
    auto triangulated = mesh;

    triangulated.normalsIndicesTriangulated.clear();
    triangulated.verticesIndicesTriangulated.clear();
    triangulated.texturesIndicesTriangulated.clear();
    triangulated.polygonElementsIndicesTriangulated.clear();

    for (int i = 0; i < triangulated.polygonElementsIndices.size() - 1; i++) {
        int startingInd = triangulated.polygonElementsIndices[i];
        int numIndices = triangulated.polygonElementsIndices[i+1] - startingInd;

        if (numIndices <= 3) {
            // copy these indices from
            // normals, textures and positions to triangulated
            auto vectorChunk = triangulated.verticesIndices.mid(startingInd, numIndices);
            triangulated.verticesIndicesTriangulated.append(vectorChunk);

            vectorChunk = triangulated.texturesIndices.mid(startingInd, numIndices);
            triangulated.texturesIndicesTriangulated.append(vectorChunk);

            vectorChunk = triangulated.normalsIndices.mid(startingInd, numIndices);
            triangulated.normalsIndicesTriangulated.append(vectorChunk);
        } else {
            // from the long polygon make several short (triangulated) ones
            for (int j = 0; j < numIndices - 3 + 1; j++) {
                /*
                 append the line
                 e.g. vertices indices for the polygon:
                 1 4 3 6 5 7 9
                 should become
                 1 2 3 1 3 6 1 6 5 1 5 7 1 7 9
                 and polygonElements should become from
                 5 12
                 to
                 5 8 11 14 17 20
                */
            }
        }
    }
}

EarClippingTriangulator::EarClippingTriangulator() :
    Triangulator()
{}

void EarClippingTriangulator::triangulate(MeshData &mesh)
{

}

QVector<QVector3D> EarClippingTriangulator::earClip(const QVector<QVector3D> &polygon)
{

}

bool EarClippingTriangulator::isVertexConvex(const QVector3D previousVertex, const QVector3D vertex, const QVector3D nextVertex)
{

}

bool EarClippingTriangulator::isPointInsideTriangle(const QVector3D point,
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
