#ifndef GLOBJECT3D_H
#define GLOBJECT3D_H

#include "gltransformable.h"
#include <Object3D/model3d.h>
#include <QMatrix4x4>

namespace GLObject {

class GLObject3D : public GLTransformable
{
public:
    GLObject3D();

    GLObject3D(const QVector<QVector3D> &vertices,
               const QVector<QVector2D> &textures,
               const QVector<QVector3D> &normals,
               const QVector<GLuint> &indices);

    GLObject3D(const Model3D::MeshData &mesh);

    virtual ~GLObject3D();

    virtual void reinit(const QVector<QVector3D> &vertices,
                        const QVector<QVector2D> &textures,
                        const QVector<QVector3D> &normals,
                        const QVector<GLuint> &indices);

    /*!
     * \brief Only triangulated data will be taken
     * from mesh data
     * \param mesh - mesh data.
     */
    virtual void reinit(const Model3D::MeshData &mesh);

    virtual void draw(QOpenGLShaderProgram *shaderProgram,
                      QOpenGLFunctions *functions);

    void translate(const QVector3D &t) override;
    void rotate(const QQuaternion &q) override;
    void scale(const float &factor) override;
protected:
    QOpenGLBuffer m_vertexBuffer;
    QOpenGLBuffer m_indexBuffer;
    QMatrix4x4 m_modelMatrix;
    QOpenGLFunctions *m_openglFunctions{ nullptr };

    // needed to know how much to draw later
    int m_verticesCount{ 0 };
    int m_textureCoordsCount{ 0 };
    int m_normalsCount{ 0 };
};

}

#endif // GLOBJECT3D_H
