#include "glwidget.h"
#include <QApplication>

#include "OBJLoader/objloadertests.h"
#include "Object3D/triangulatortests.h"

void runTests()
{
    OBJLoader::OBJLoaderTests objTests;
    QTest::qExec(&objTests);

    Triangulation::TriangulatorTests triangTests;
    QTest::qExec(&triangTests);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    runTests();

    GLWidget w;
    w.show();

    return a.exec();
}
