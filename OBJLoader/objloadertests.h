#ifndef OBJLOADERTESTS_H
#define OBJLOADERTESTS_H

#include <QTest>

namespace OBJLoader {

class OBJLoaderTests : public QObject
{
    Q_OBJECT

private slots:
    void objLoaderShuffledVertices();
    void objLoaderEmptyFile();
    void objLoaderIncorrectFile();
    void objLoaderTextStream();
    void objLoaderWithoutData();
};

}

#endif // OBJECT3DTESTS_H
