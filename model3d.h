#ifndef MODEL3D_H
#define MODEL3D_H

#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include "model_data.h"

class QOpenGLTexture;
class QOpenGLFunctions;
class QOpenGLShaderProgram;

class Model3D
{
public:
    Model3D();
    Model3D(const QVector<VertexData> &vertexData,
             const QVector<GLuint> &indeces,
             const QImage &texture);

    ~Model3D();

    void init(const QVector<VertexData> &vertexData,
         const QVector<GLuint> &indeces,
         const QImage &texture);


    void draw(QOpenGLShaderProgram *shaderProgram,
              QOpenGLFunctions *functions);
private:
    QOpenGLBuffer   m_vertexBuffer;
    QOpenGLBuffer   m_indexBuffer;
    QMatrix4x4      m_modelMatrix;
    QOpenGLTexture *m_texture;
    QOpenGLFunctions* f;
};

#endif // MODEL3D_H
