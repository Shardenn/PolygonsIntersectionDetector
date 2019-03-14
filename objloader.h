#ifndef QOBJLOADER_H
#define QOBJLOADER_H

#include <GL/GL.h>
#include <QString>
#include <QVector3D>
#include <QVector2D>

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

    QVector<GLuint> m_indeces;
    QVector<QVertexData> m_vertices;
};

#endif // QOBJLOADER_H
