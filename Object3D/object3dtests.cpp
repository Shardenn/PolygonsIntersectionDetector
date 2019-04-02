#include "object3dtests.h"
#include "objloader.h"

void OBJLoader::Object3DTests::objLoaderTrivial()
{
    auto loadedMesh = OBJLoader::OBJLoader::load(":/Objects/test_trivial.obj");
    MeshData mesh;
    mesh.m_positions.append(QVector3D(2, -1, 0));
    mesh.m_positions.append(QVector3D(1, -1, 2));
    mesh.m_positions.append(QVector3D(0, -1, 0));

    mesh.m_normals.append(QVector3D(0, -1, 0));
    mesh.m_normals.append(QVector3D(0, -1, 0));
    mesh.m_normals.append(QVector3D(0, -1, 0));

    mesh.m_polygonVertices.append(0);
    mesh.m_polygonVertices.append(3);

    QCOMPARE(loadedMesh, mesh);
}
