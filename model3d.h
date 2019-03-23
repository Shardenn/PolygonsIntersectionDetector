#ifndef MODEL3D_H
#define MODEL3D_H

#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include "modeldata.h"

class QOpenGLTexture;
class QOpenGLFunctions;
class QOpenGLShaderProgram;

namespace  Model3D {

class GLModel3D
{
public:
    GLModel3D();
    GLModel3D(const QVector<VertexData> &vertexData,
             const QVector<GLuint> &indeces,
             const QImage &texture);

    ~GLModel3D();

    void reinit(const QVector<VertexData> &vertexData,
         const QVector<GLuint> &indeces,
         const QImage &texture);


    void draw(QOpenGLShaderProgram *shaderProgram,
              QOpenGLFunctions *functions);

    void translate(const QVector3D &translation);
private:
    QOpenGLBuffer   m_vertexBuffer;
    QOpenGLBuffer   m_indexBuffer;
    QMatrix4x4      m_modelMatrix;
    QOpenGLTexture *m_texture;
    QOpenGLFunctions* f;
};

} // namespace Model3D

#endif // MODEL3D_H
