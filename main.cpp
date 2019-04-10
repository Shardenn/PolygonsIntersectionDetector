#include "glwidget.h"
#include <QApplication>

#include "OBJLoader/objloadertests.h"

void runTests()
{
    OBJLoader::OBJLoaderTests objTests;
    QTest::qExec(&objTests);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    runTests();

    GLWidget w;
    w.show();

    return a.exec();
}
