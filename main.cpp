#include "glwidget.h"
#include <QApplication>

#include "GeomStack/geomstacktests.h"

void runTests()
{
    GeomStack::GeomStackTests gsTests;
    QTest::qExec(&gsTests);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //runTests();

    GLWidget w;
    w.show();

    return a.exec();
}
