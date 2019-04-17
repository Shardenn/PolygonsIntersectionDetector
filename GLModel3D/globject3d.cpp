#include "globject3d.h"
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <memory>
#include "Object3D/triangulator.h"

GLObject::GLObject3D::GLObject3D() :
    m_vertexBuffer(QOpenGLBuffer::VertexBuffer),
    m_indexBuffer(QOpenGLBuffer::IndexBuffer)
{

}

GLObject::GLObject3D::GLObject3D(const QVector<QVector3D> &vertices,
                                 const QVector<QVector2D> &textures,
                                 const QVector<QVector3D> &normals,
                                 const QVector<GLuint> &indices) :
    GLObject3D()
{
    reinit(vertices, textures, normals, indices);
}

GLObject::GLObject3D::GLObject3D(const Model3D::MeshData &mesh) :
    GLObject3D()
{
    reinit(mesh);
}

GLObject::GLObject3D::~GLObject3D()
{
    if (m_vertexBuffer.isCreated())
        m_vertexBuffer.destroy();
    if (m_indexBuffer.isCreated())
        m_indexBuffer.destroy();
}

void GLObject::GLObject3D::reinit(const QVector<QVector3D> &vertices,
                                  const QVector<QVector2D> &textures,
                                  const QVector<QVector3D> &normals,
                                  const QVector<GLuint> &indices)
{
    if(m_vertexBuffer.isCreated())
        m_vertexBuffer.destroy();
    if(m_indexBuffer.isCreated())
        m_indexBuffer.destroy();

    m_verticesCount      = vertices.size();
    m_textureCoordsCount = textures.size();
    m_normalsCount       = normals.size();

    int elementsCount = m_verticesCount +
            m_textureCoordsCount +
            m_normalsCount;
    // create and allocate vertex and index buffer
    m_vertexBuffer.create();
    m_vertexBuffer.bind();
    m_vertexBuffer.allocate(elementsCount * (3 + 2 + 3));

    int offset = 0;
    m_vertexBuffer.write(offset, vertices.constData(), vertices.size() * sizeof(QVector3D));

    offset += vertices.size() * sizeof(QVector3D);
    m_vertexBuffer.write(offset, textures.constData(), textures.size() * sizeof(QVector2D));

    offset += textures.size() * sizeof(QVector2D);
    m_vertexBuffer.write(offset, normals.constData(), normals.size() * sizeof(QVector3D));

    m_vertexBuffer.release();

    m_indexBuffer.create();
    m_indexBuffer.bind();
    m_indexBuffer.allocate(indices.constData(),
                           indices.size() * sizeof(GLuint));
    m_indexBuffer.release();
}

void GLObject::GLObject3D::reinit(const Model3D::MeshData &mesh)
{
    Triangulation::NaiveTriangulator tr;

    auto vertices      = mesh.getTriangulatedVertices();
    auto textureCoords = mesh.getTriangulatedTextureCoords();
    auto normals       = mesh.getTriangulatedNormals();

    QVector<GLuint> indices;
    for (unsigned int i = 0; i < vertices.size(); i++) {
        indices.append(i);
    }

    reinit(vertices, textureCoords, normals, indices);
}

void GLObject::GLObject3D::draw(QOpenGLShaderProgram *shaderProgram,
                                QOpenGLFunctions *functions)
{
    if (!m_vertexBuffer.isCreated()) {
        qDebug() << "Vertex buffer is not created";
        return;
    }
    if (!m_indexBuffer.isCreated()) {
        qDebug() << "Index buffer is not created";
        return;
    }

    shaderProgram->setUniformValue("u_modelMatrix", m_modelMatrix);

    m_vertexBuffer.bind();

    int offset = 0;

    int vertLoc = shaderProgram->attributeLocation("vertexPosition");
    shaderProgram->enableAttributeArray(vertLoc);
    shaderProgram->setAttributeBuffer(vertLoc, GL_FLOAT, offset, 3);

    offset += m_verticesCount * sizeof(QVector3D);

    int texLoc = shaderProgram->attributeLocation("textureCoordinate");
    shaderProgram->enableAttributeArray(texLoc);
    shaderProgram->setAttributeBuffer(texLoc, GL_FLOAT, offset, 2);

    offset += m_textureCoordsCount * sizeof(QVector2D);

    int normLoc = shaderProgram->attributeLocation("normal");
    shaderProgram->enableAttributeArray(normLoc);
    shaderProgram->setAttributeBuffer(normLoc, GL_FLOAT, offset, 3);

    m_indexBuffer.bind();

    functions->glDrawElements(drawMode, m_indexBuffer.size() - 1,
                              GL_UNSIGNED_INT, nullptr);

    m_indexBuffer.release();
    m_vertexBuffer.release();
}

void GLObject::GLObject3D::translate(const QVector3D &t)
{
    m_modelMatrix.translate(t);
}

void GLObject::GLObject3D::rotate(const QQuaternion &q)
{
    m_modelMatrix.rotate(q);
}

void GLObject::GLObject3D::scale(const float &factor)
{
    m_modelMatrix.scale(factor);
}
