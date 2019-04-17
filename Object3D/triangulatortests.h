#ifndef TRIANGULATORTESTS_H
#define TRIANGULATORTESTS_H

#include <QTest>

namespace Triangulation {

class TriangulatorTests : public QObject
{
    Q_OBJECT

private slots:
    void positiveTest0();
    void positiveTest1();

    void negativeTest0();
    void negativeTest1();
    void negativeTest2();
    void negativeTest3();

    void emptyTest();
    void wrongPolygonTest();

};

}

#endif // TRIANGULATORTESTS_H
