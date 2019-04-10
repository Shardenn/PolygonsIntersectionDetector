#ifndef GLMODEL3D
#define GLMODEL3D

#include <QOpenGLBuffer>
#include <QMatrix4x4>
// needed for MeshData. Maybe it should be pointer?
#include <Object3D/model3d.h>

class QOpenGLTexture;
class QOpenGLFunctions;
class QOpenGLShaderProgram;

namespace GLModel3D {

class GLModel3D
{
public:
    GLModel3D();
    GLModel3D(const Model3D::MeshData& meshData,
              const QImage& texture);

    ~GLModel3D();

    void reinit(const Model3D::MeshData& meshData,
                const QImage& texture);

    void draw(QOpenGLShaderProgram *shaderProgram,
              QOpenGLFunctions *functions);

    void translate(const QVector3D &translation);
private:
    QOpenGLBuffer     m_vertexBuffer;
    QOpenGLBuffer     m_indexBuffer;
    QMatrix4x4        m_modelMatrix;
    QOpenGLTexture   *m_texture = nullptr;
    QOpenGLFunctions *m_openglFunctions = nullptr;
    Model3D::MeshData m_meshData;
};

} // namespace GLModel3D

#endif // GLMODEL3D
