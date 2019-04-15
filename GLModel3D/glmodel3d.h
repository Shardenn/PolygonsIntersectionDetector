#ifndef GLMODEL3D
#define GLMODEL3D

#include <QOpenGLBuffer>
#include <QMatrix4x4>

#include "Object3D/model3d.h"
//#include "glsimpleobject3d.h"
#include "gltransofmable.h"

class QOpenGLTexture;
class QOpenGLFunctions;
class QOpenGLShaderProgram;

namespace GLModel3D {

// draws from MeshData
class GLModel3D : public GLTransformable// : public GLSimpleObject3D
{
public:
    GLModel3D();
    GLModel3D(const Model3D::MeshData& meshData,
              const QImage& texture);

    ~GLModel3D();

    void reinit(const Model3D::MeshData& meshData,
                const QImage& texture);

    void draw(QOpenGLShaderProgram *shaderProgram,
              QOpenGLFunctions *functions) override;

    void rotate(const QQuaternion &quat) override;
    void scale(const float &factor) override;
    void translate(const QVector3D &translation) override;
protected:
    QOpenGLBuffer     m_vertexBuffer;
    QOpenGLBuffer     m_indexBuffer;
    QMatrix4x4        m_modelMatrix;
    QOpenGLFunctions *m_openglFunctions = nullptr;

private:
    QOpenGLTexture   *m_texture = nullptr;
    Model3D::MeshData m_meshData;
};

} // namespace GLModel3D

#endif // GLMODEL3D
