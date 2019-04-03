#include "object3dtests.h"
#include "objloader.h"

void OBJLoader::Object3DTests::objLoaderTrivial()
{
    auto loadedMesh = OBJLoader::OBJLoader::load(":/Objects/UnitTests/test_trivial.obj");
    MeshData expectedMesh;
    expectedMesh.m_positions.append(QVector3D(2, -1, 0));
    expectedMesh.m_positions.append(QVector3D(1, -1, 2));
    expectedMesh.m_positions.append(QVector3D(0, -1, 0));

    expectedMesh.m_normals.append(QVector3D(0, -1, 0));
    expectedMesh.m_normals.append(QVector3D(0, -1, 0));
    expectedMesh.m_normals.append(QVector3D(0, -1, 0));

    expectedMesh.m_polygonVertices.append(0);
    expectedMesh.m_polygonVertices.append(3);

    QCOMPARE(loadedMesh, expectedMesh);
}

void OBJLoader::Object3DTests::objLoaderUsual()
{
    auto loadedMesh = OBJLoader::OBJLoader::load(":/Objects/UnitTests/test_usual.obj");

    MeshData expectedMesh;
    expectedMesh.m_positions.append(QVector3D(2, -1, 0));
    expectedMesh.m_positions.append(QVector3D(1, -1, 2));
    expectedMesh.m_positions.append(QVector3D(0, -1, 0));
    expectedMesh.m_positions.append(QVector3D(1, 2, 0));
    expectedMesh.m_positions.append(QVector3D(1, 1, 2));
    expectedMesh.m_positions.append(QVector3D(1, 0, 0));

    expectedMesh.m_normals.append(QVector3D(0, -1, 0));
    expectedMesh.m_normals.append(QVector3D(0, -1, 0));
    expectedMesh.m_normals.append(QVector3D(0, -1, 0));
    expectedMesh.m_normals.append(QVector3D(1, 0, 0));
    expectedMesh.m_normals.append(QVector3D(1, 0, 0));
    expectedMesh.m_normals.append(QVector3D(1, 0, 0));

    expectedMesh.m_polygonVertices.append(0);
    expectedMesh.m_polygonVertices.append(3);
    expectedMesh.m_polygonVertices.append(6);

    QCOMPARE(expectedMesh, loadedMesh);
}

void OBJLoader::Object3DTests::objLoaderShuffledVertices()
{
    auto loadedMesh = OBJLoader::load(":/Objects/UnitTests/test_shuffled.obj");

    MeshData expectedMesh;
    expectedMesh.m_positions.append(QVector3D(1, -1, 2));
    expectedMesh.m_positions.append(QVector3D(2, -1, 0));
    expectedMesh.m_positions.append(QVector3D(0, -1, 0));
    expectedMesh.m_positions.append(QVector3D(1, 0, 0));
    expectedMesh.m_positions.append(QVector3D(1, 2, 0));
    expectedMesh.m_positions.append(QVector3D(1, 1, 2));

    expectedMesh.m_normals.append(QVector3D(0, -1, 0));
    expectedMesh.m_normals.append(QVector3D(1, 0, 0));
    expectedMesh.m_normals.append(QVector3D(0, -1, 0));
    expectedMesh.m_normals.append(QVector3D(1, 0, 0));
    expectedMesh.m_normals.append(QVector3D(0, -1, 0));
    expectedMesh.m_normals.append(QVector3D(1, 0, 0));

    expectedMesh.m_polygonVertices.append(0);
    expectedMesh.m_polygonVertices.append(3);
    expectedMesh.m_polygonVertices.append(6);

    QCOMPARE(expectedMesh, loadedMesh);
}

void OBJLoader::Object3DTests::objLoaderEmptyFile()
{
    auto loadedMesh = OBJLoader::load(":/Objects/UnitTests/test_empty.obj");
    MeshData expectedMesh;

    QCOMPARE(expectedMesh, loadedMesh);
}

void OBJLoader::Object3DTests::objLoaderTest01()
{
    auto loadedMesh = OBJLoader::load("v 1 2 3");
    QCOMPARE(loadedMesh.isNull());
    auto loadedMesh = OBJLoader::load("v 1 2 3\n f 1 1 1");
    QCOMPARE(loadedMesh.isNull());
    auto loadedMesh = OBJLoader::load("v 1 2 3\n f -1 1 1");
    QCOMPARE(loadedMesh.isNull());
    auto loadedMesh = OBJLoader::load("v 1 2 3\n f 1 1 1 \n g groupName\n f 1 1 1");
    QCOMPARE(loadedMesh.isNull());
}
/*
void OBJLoader::Object3DTests::objLoaderIncorrectFile()
{
    auto loadedMesh = OBJLoader::load(":/Objects/UnitTests/test_incorrect_file.obj");
    MeshData expectedMesh;
    expectedMesh.m_positions.append(QVector3D(2,1,2)); // TODO what to expect from wrong file?
    QCOMPARE(expectedMesh, loadedMesh);
}
*/
