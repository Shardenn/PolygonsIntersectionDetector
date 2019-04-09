#ifndef OBJECT3DTESTS_H
#define OBJECT3DTESTS_H

#include <QTest>

namespace OBJLoader {

class Object3DTests : public QObject
{
    Q_OBJECT

private slots:
    void objLoaderShuffledVertices();

    // TODO
    //void objLoaderWithTextures();

    void objLoaderEmptyFile();
    void objLoaderIncorrectFile();
    void objLoaderTextStream();
};

}

#endif // OBJECT3DTESTS_H
