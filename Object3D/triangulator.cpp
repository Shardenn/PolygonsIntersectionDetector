#include "triangulator.h"
#include <memory>

Triangulator::Triangulator()
{

}

Triangulator::~Triangulator()
{}

NaiveTriangulator::NaiveTriangulator() :
    Triangulator ()
{}

NaiveTriangulator::~NaiveTriangulator()
{}

void NaiveTriangulator::triangulate(MeshData &mesh)
{
    mesh.normalsIndicesTriangulated.clear();
    mesh.verticesIndicesTriangulated.clear();
    mesh.texturesIndicesTriangulated.clear();
    mesh.polygonElementsIndicesTriangulated.clear();
    mesh.polygonElementsIndicesTriangulated.append(0);

    for (int i = 0; i < mesh.polygonElementsIndices.size() - 1; i++) {
        int startingInd = mesh.polygonElementsIndices[i];
        int numIndices = mesh.polygonElementsIndices[i+1] - startingInd;

        if (numIndices <= 3) {
            // copy these indices from
            // normals, textures and positions to triangulated
            auto vectorChunk = mesh.verticesIndices.mid(startingInd, numIndices);
            mesh.verticesIndicesTriangulated.append(vectorChunk);

            vectorChunk = mesh.texturesIndices.mid(startingInd, numIndices);
            mesh.texturesIndicesTriangulated.append(vectorChunk);

            vectorChunk = mesh.normalsIndices.mid(startingInd, numIndices);
            mesh.normalsIndicesTriangulated.append(vectorChunk);
        } else {
            // from the long polygon make several short (triangulated) ones
            int commonInd = mesh.verticesIndices[startingInd];
            for (int j = 1; j < numIndices - 1; j++) {
                int current, next;

                if (mesh.verticesIndices.size() > 0) {
                    current = mesh.verticesIndices[startingInd + j];
                    next = mesh.verticesIndices[startingInd + j + 1];
                    mesh.verticesIndicesTriangulated.append({commonInd, current, next});
                }

                if (mesh.normalsIndices.size() > 0) {
                    current = mesh.normalsIndices[startingInd + j];
                    next = mesh.normalsIndices[startingInd + j + 1];
                    mesh.normalsIndicesTriangulated.append({commonInd, current, next});
                }

                if (mesh.texturesIndices.size() > 0) {
                    current = mesh.texturesIndices[startingInd + j];
                    next = mesh.texturesIndices[startingInd + j + 1];
                    mesh.texturesIndicesTriangulated.append({commonInd, current, next});
                }
                mesh.polygonElementsIndicesTriangulated.append(mesh.verticesIndicesTriangulated.size());
            }
        }
    }
    mesh.texturesIndices = mesh.texturesIndicesTriangulated;
    mesh.verticesIndices = mesh.verticesIndicesTriangulated;
    mesh.normalsIndices = mesh.normalsIndicesTriangulated;
    mesh.polygonElementsIndices = mesh.polygonElementsIndicesTriangulated;
}

EarClippingTriangulator::EarClippingTriangulator() :
    Triangulator()
{}

EarClippingTriangulator::~EarClippingTriangulator()
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
