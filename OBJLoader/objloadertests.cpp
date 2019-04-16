#include "objloadertests.h"
#include "objloader.h"
#include "Object3D/model3d.h"
#include <memory>

void OBJLoader::OBJLoaderTests::objLoaderShuffledVertices()
{
    OBJLoader loader;

    std::unique_ptr<MeshData> loadedMesh(loader.load(":/Objects/UnitTests/test_shuffled.obj"));
    std::unique_ptr<MeshData> expectedMesh(new MeshData);

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

void OBJLoader::OBJLoaderTests::objLoaderEmptyFile()
{
    OBJLoader loader;
    std::unique_ptr<MeshData> loadedMesh(loader.load(":/Objects/UnitTests/test_empty.obj"));
    std::unique_ptr<MeshData> expectedMesh(new MeshData);

    QCOMPARE(*expectedMesh, *loadedMesh);
}

void OBJLoader::OBJLoaderTests::objLoaderIncorrectFile()
{
    OBJLoader loader;
    std::unique_ptr<MeshData> loadedMesh(loader.load(":/Objects/UnitTests/test_incorrect_file.obj"));

    QCOMPARE(nullptr, loadedMesh);
}

void OBJLoader::OBJLoaderTests::objLoaderTextStream()
{
    OBJLoader loader;

    QTextStream stream("v 1 2 3\n"    \
                       "v 2.0 -1 4\n" \
                      "vn 1 0.0 0.0\n"  \
                       "f 1//1 2//1\n");
    std::unique_ptr<MeshData> loadedMesh(loader.load(stream));
    std::unique_ptr<MeshData> expectedMesh(new Model3D::MeshData);

    expectedMesh->positions.append(QVector3D(1, 2, 3));
    expectedMesh->positions.append(QVector3D(2, -1, 4));

    expectedMesh->normals.append(QVector3D(1, 0, 0));

    expectedMesh->verticesIndices = QVector<int>{1, 2};
    expectedMesh->normalsIndices = QVector<int>{1, 1};
    expectedMesh->polygonElementsIndices = QVector<int>{0, 2};

    QCOMPARE(*loadedMesh, *expectedMesh);
}

void OBJLoader::OBJLoaderTests::objLoaderWithoutData()
{
    OBJLoader loader;

    QTextStream stream("vn 1.0 0.0 -0.0\n" \
                       "f 2/3/2 3/2/1 1/1/1");

    std::unique_ptr<MeshData> loadedMesh(loader.load(stream));

    QCOMPARE(nullptr, loadedMesh);
}
