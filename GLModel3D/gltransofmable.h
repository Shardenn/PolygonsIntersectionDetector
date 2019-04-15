#ifndef GLTRANSOFMABLE_H
#define GLTRANSOFMABLE_H

#include <QOpenGLFunctions>

class QQuaternion;
class QVector3D;
class QMatrix4x4;
class QOpenGLShaderProgram;

namespace GLModel3D {

class GLTransformable
{
public:
    virtual void rotate(const QQuaternion &quat) = 0;
    virtual void translate(const QVector3D &tran) = 0;
    virtual void scale(const float &factor) = 0;
    virtual void draw(QOpenGLShaderProgram *program,
                      QOpenGLFunctions *functions) = 0;

    GLenum drawMode{ GL_TRIANGLES };
};

}

#endif // GLTRANSOFMABLE_H
