#include "object3dtests.h"
#include "objloader.h"

void OBJLoader::Object3DTests::objLoaderShuffledVertices()
{
    OBJLoader loader;
    Model3D::MeshData *loadedMesh = loader.load(":/Objects/UnitTests/test_shuffled.obj");

    MeshData *expectedMesh = new MeshData;

    expectedMesh->positions.append(QVector3D(2.0, -1.0, 0.0));
    expectedMesh->positions.append(QVector3D(1.0, -1.0, 2.0));
    expectedMesh->positions.append(QVector3D(0.0, -1.0, 0.0));
    expectedMesh->positions.append(QVector3D(1.0, 2.0, 0.0));
    expectedMesh->positions.append(QVector3D(1.0, 1.0, 2.0));
    expectedMesh->positions.append(QVector3D(1.0, 0.0, 0.0));

    expectedMesh->normals.append(QVector3D(0.0, -1.0, 0.0));
    expectedMesh->normals.append(QVector3D(1.0, 0.0, 0.0));

    expectedMesh->verticesIndices = QVector<int>{1, 0, 2, 5, 3, 4};
    expectedMesh->normalsIndices = QVector<int>{0, 1, 0, 1, 0, 1};

    expectedMesh->polygonElementsIndices = QVector<int>{0, 3, 6};

    QCOMPARE(*expectedMesh, *loadedMesh);
}

void OBJLoader::Object3DTests::objLoaderEmptyFile()
{
    OBJLoader loader;
    auto loadedMesh = loader.load(":/Objects/UnitTests/test_empty.obj");
    MeshData *expectedMesh = new MeshData;

    QCOMPARE(*expectedMesh, *loadedMesh);
}

void OBJLoader::Object3DTests::objLoaderIncorrectFile()
{
    OBJLoader loader;
    Model3D::MeshData *loadedMesh = loader.load(":/Objects/UnitTests/test_incorrect_file.obj");

    QCOMPARE(nullptr, loadedMesh);
}

void OBJLoader::Object3DTests::objLoaderTextStream()
{
    OBJLoader loader;

    QTextStream stream("v 1 2 3\n"    \
                       "v 2.0 -1 4\n" \
                      "vn 1 0.0 0.0\n"  \
                       "f 1//1 2//1\n");
    auto loadedMesh = loader.load(stream);

    Model3D::MeshData *expectedMesh = new Model3D::MeshData;
    expectedMesh->positions.append(QVector3D(1, 2, 3));
    expectedMesh->positions.append(QVector3D(2, -1, 4));

    expectedMesh->normals.append(QVector3D(1, 0, 0));

    expectedMesh->verticesIndices = QVector<int>{1, 2};
    expectedMesh->normalsIndices = QVector<int>{1, 1};
    expectedMesh->polygonElementsIndices = QVector<int>{0, 2};

    QCOMPARE(*loadedMesh, *expectedMesh);
}
