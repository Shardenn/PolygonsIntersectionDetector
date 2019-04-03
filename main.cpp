#include "glwidget.h"
#include <QApplication>

#include <Object3D/object3dtests.h>

void runTests()
{
    OBJLoader::Object3DTests objTests;
    QTest::qExec(&objTests);
}

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QApplication a(argc, argv);

    runTests();

    GLWidget w;
    w.show();

    return a.exec();
}
