#include "geomstacktests.h"

int square(const int x)
{
    return x*x;
}


void GeomStack::GeomStackTests::testSquare01()
{
    const int res = square(2);
    QCOMPARE(res, 3);
}
