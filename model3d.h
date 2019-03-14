#ifndef MODEL3D_H
#define MODEL3D_H

#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include "vertexdata.h"

class QOpenGLTexture;
class QOpenGLFunctions;
class QOpenGLShaderProgram;

class QModel3D
{
public:
    QModel3D();
    QModel3D(const QVector<QVertexData> &vertexData,
             const QVector<GLuint> &indeces,
             const QImage &texture);

    ~QModel3D();

    void init(const QVector<QVertexData> &vertexData,
         const QVector<GLuint> &indeces,
         const QImage &texture);
    void draw(QOpenGLShaderProgram *shaderProgram,
              QOpenGLFunctions *functions);
private:
    QOpenGLBuffer m_vertexBuffer;
    QOpenGLBuffer m_indexBuffer;
    QMatrix4x4 m_modelMatrix;
    QOpenGLTexture *m_texture;
};

#endif // MODEL3D_H
