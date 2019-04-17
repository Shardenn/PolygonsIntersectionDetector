#include "triangulatortests.h"
#include "triangulator.h"

using namespace Triangulation;

void TriangulatorTests::positiveTest0()
{
    QVector<int> indices { 3, 5, 1, 4,
                           8, 2, 7,
                           10, 9, 6, 14, 11, 13, 12 };
    QVector<int> offsets { 0, 4, 7, 14 };

    Triangulation::NaiveTriangulator tr;
    QVector<int> triangulated = tr.triangulate(indices, offsets);

    QVector<int> expected { 3,  5,  1,  3,  1,  4,  8,  2,  7, 10, 9,  6,
                            10, 6, 14, 10, 14, 11, 10, 11, 13, 10, 13, 12 };

    QCOMPARE(expected, triangulated);
}

void TriangulatorTests::positiveTest1()
{
    QVector<int> indices { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    QVector<int> offsets { 0, 9 };

    NaiveTriangulator tr;
    QVector<int> triangulated = tr.triangulate(indices, offsets);

    QVector<int> expected { 1, 2, 3, 1, 3, 4, 1, 4, 5, 1, 5, 6,
                            1, 6, 7, 1, 7, 8, 1, 8, 9 };

    QCOMPARE(expected, triangulated);
}

void TriangulatorTests::negativeTest0()
{
    QVector<int> indices { -1 , 0, -1, 0,
                           1, 1, 0,
                           -10 };
    QVector<int> offsets { 0, 0, 0, 4, 7, 8 };

    NaiveTriangulator tr;
    QVector<int> triangulated = tr.triangulate(indices, offsets);

    QVector<int> expected { -1, 0, -1, -1, -1, 0,
                          1, 1, 0 };

    QCOMPARE(expected, triangulated);
}

void TriangulatorTests::negativeTest1()
{
    QVector<int> indices { 0, 1, 2,
                           3, 4, 5,
                           6, 7, 8, 9, 10, 11 };
    QVector<int> offsets { 0, 3, 6, 2, 5, 8, 12 };

    NaiveTriangulator tr;
    QVector<int> triangulated = tr.triangulate(indices, offsets);

    QVector<int> expected { 0, 1, 2, 3, 4, 5, 2, 3, 4, 5, 6, 7,
                          8, 9, 10, 8, 10, 11 };

    QCOMPARE(expected, triangulated);
}

void TriangulatorTests::negativeTest2()
{
    QVector<int> indices { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
    QVector<int> offsets { -1, 0, 3, -3, 6, 9 };

    NaiveTriangulator tr;
    QVector<int> triangulated = tr.triangulate(indices, offsets);

    QVector<int> expected { 0, 1, 2, 6, 7, 8 };

    QCOMPARE(expected, triangulated);
}

void TriangulatorTests::negativeTest3()
{
    QVector<int> indices { 0, 1, 2, 3, 4, 5, 6, 7, 8 };
    QVector<int> offsets { 0, 6, 12 };

    NaiveTriangulator tr;
    QVector<int> triangulated = tr.triangulate(indices, offsets);

    QVector<int> expected { 0, 1, 2, 0, 2, 3, 0, 3, 4, 0, 4, 5 };

    QCOMPARE(expected, triangulated);
}


void TriangulatorTests::emptyTest()
{
    QVector<int> indices {};
    QVector<int> offsets {};

    NaiveTriangulator tr;
    QVector<int> triangulated = tr.triangulate(indices, offsets);

    QVector<int> expected {};

    QCOMPARE(expected, triangulated);
}

void TriangulatorTests::wrongPolygonTest()
{
    QVector<int> indices { 1, 1, 1, 1,
                           2, 3, 3,
                           4, 5, 6, 7, 7, 7, 7 };
    QVector<int> offsets { 0, 4, 7, 14 };

    NaiveTriangulator tr;
    QVector<int> triangulated = tr.triangulate(indices, offsets);

    QVector<int> expected { 1, 1, 1, 1, 1, 1, 2, 3, 3, 4, 5, 6, 4, 6, 7, 4, 7, 7,
                          4, 7, 7, 4, 7, 7 };

    QCOMPARE(expected, triangulated);
}
