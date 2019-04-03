#ifndef OBJECT3DTESTS_H
#define OBJECT3DTESTS_H

#include <QTest>

namespace OBJLoader {

class Object3DTests : public QObject
{
    Q_OBJECT

private slots:
    void objLoaderTrivial();
    void objLoaderUsual();
    void objLoaderShuffledVertices();

    // TODO
    //void objLoaderWithTextures();

    void objLoaderEmptyFile();
    void objLoaderTest01();
    // TODO
    // no error checking in objLoader now.
    // Add error checks, this test crashes
    //void objLoaderIncorrectFile();
};

}

#endif // OBJECT3DTESTS_H
