#ifndef GLTRANSOFMABLE_H
#define GLTRANSOFMABLE_H

#include <QOpenGLFunctions>

class QQuaternion;
class QVector3D;
class QMatrix4x4;
class QOpenGLShaderProgram;

namespace GLObject {

class GLDrawable
{
public:
    virtual ~GLDrawable() {}

    virtual void draw(QOpenGLShaderProgram *program,
                      QOpenGLFunctions *functions) = 0;

    GLenum drawMode{ GL_TRIANGLES };
};

class GLTransformable : public GLDrawable
{
public:
    virtual ~GLTransformable() {}

    virtual void rotate(const QQuaternion &quat) = 0;
    virtual void translate(const QVector3D &tran) = 0;
    virtual void scale(const float &factor) = 0;
};

}

#endif // GLTRANSOFMABLE_H
