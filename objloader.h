#ifndef QOBJLOADER_H
#define QOBJLOADER_H

#include <GL/gl.h>
#include <QString>
#include <QVector>
#include <QVector3D>
#include <QVector2D>
#include "vertexdata.h"
// TODO loadModel should return new object
class QObjLoader
{
public:
    QObjLoader(const QString &filePath = nullptr);

    void LoadModel(const QString &filePath);
    void LoadModel();
private:
    QString m_filePath;

    QVector<QVector3D> m_verticesCoord;
    QVector<QVector3D> m_normals;
    QVector<QVector2D> m_textureCoordinates;

    QVector<GLuint> m_indices;
    QVector<QVertexData> m_vertices;
};

#endif // QOBJLOADER_H
