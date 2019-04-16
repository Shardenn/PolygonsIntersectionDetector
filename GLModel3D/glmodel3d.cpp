#include "glmodel3d.h"

#include <QOpenGLTexture>
#include <QOpenGLFunctions>
#include <QOpenGLShader>

using namespace Model3D;

GLObject::GLModel3D::GLModel3D() :
    m_vertexBuffer(QOpenGLBuffer::VertexBuffer),
    m_indexBuffer(QOpenGLBuffer::IndexBuffer)
{

}

GLObject::GLModel3D::GLModel3D(const MeshData& mesh,
                                const QImage& texture) :
    GLModel3D()
{
    m_meshData = mesh;
    reinit(mesh, texture);
}

GLObject::GLModel3D::~GLModel3D()
{
    if(m_vertexBuffer.isCreated())
        m_vertexBuffer.destroy();
    if(m_indexBuffer.isCreated())
        m_indexBuffer.destroy();

    if(m_texture != nullptr && m_texture->isCreated()) {
        m_texture->destroy();
        delete m_texture;
    }
}

void GLObject::GLModel3D::reinit(const MeshData& mesh,
                                const QImage& texture)
{
    if(m_vertexBuffer.isCreated())
        m_vertexBuffer.destroy();
    if(m_indexBuffer.isCreated())
        m_indexBuffer.destroy();

    if(m_texture != nullptr) {
        if (m_texture->isCreated()) {
            m_texture->destroy();
        }
    }

    int elementsCount = mesh.verticesIndices.size() +
                        mesh.texturesIndices.size() +
                        mesh.normalsIndices.size();

    m_vertexBuffer.create();
    m_vertexBuffer.bind();
    m_vertexBuffer.allocate(elementsCount * (3 + 2 + 3));

    int offset = 0;
    QVector<QVector3D> allPositions = mesh.getChronologicalVerticesCoords();
    m_vertexBuffer.write(offset, allPositions.constData(),
                                 allPositions.size() * sizeof(QVector3D));

    offset += allPositions.size() * sizeof(QVector3D);
    QVector<QVector2D> allTextures = mesh.getChronologicalTexturesCoords();
    m_vertexBuffer.write(offset, allTextures.constData(),
                                 allTextures.size() * sizeof(QVector2D));

    offset += allTextures.size()  * sizeof(QVector2D);
    QVector<QVector3D> allNormals = mesh.getChronologicalNormalsCoords();
    m_vertexBuffer.write(offset, allNormals.constData(),
                                 allNormals.size() * sizeof(QVector3D));

    m_vertexBuffer.release();

    QVector<int> indices(mesh.verticesIndices.size());
    for (int i = 0; i < mesh.verticesIndices.size(); i++) {
        indices[i] = i;
    }

    m_indexBuffer.create();
    m_indexBuffer.bind();
    m_indexBuffer.allocate(indices.constData(),
                           indices.size() * sizeof (GLuint));
    m_indexBuffer.release();

    // get how much elements in QVector<VertexData> do we need
    m_texture = new QOpenGLTexture(texture.mirrored());

    // set nearest filtering mode for texture minification
    m_texture->setMinificationFilter(QOpenGLTexture::Nearest);
    // set bilinear filtering mode for texture magnification
    m_texture->setMagnificationFilter(QOpenGLTexture::Linear);
    // (1.1, 1.2) is the same tex coord as (0.1, 0.2)
    m_texture->setWrapMode(QOpenGLTexture::Repeat);
}

void GLObject::GLModel3D::draw(QOpenGLShaderProgram *shaderProgram,
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

    m_texture->bind(0);

    shaderProgram->setUniformValue("u_texture", 0);
    shaderProgram->setUniformValue("u_modelMatrix", m_modelMatrix);

    m_vertexBuffer.bind();

    int offset = 0;

    int vertLoc = shaderProgram->attributeLocation("vertexPosition");
    shaderProgram->enableAttributeArray(vertLoc);
    shaderProgram->setAttributeBuffer(vertLoc, GL_FLOAT, offset, 3);

    offset += m_meshData.verticesIndices.size() * sizeof(QVector3D);

    int texLoc = shaderProgram->attributeLocation("textureCoordinate");
    shaderProgram->enableAttributeArray(texLoc);
    shaderProgram->setAttributeBuffer(texLoc, GL_FLOAT, offset, 2);

    offset += m_meshData.texturesIndices.size() * sizeof(QVector2D);

    int normLoc = shaderProgram->attributeLocation("normal");
    shaderProgram->enableAttributeArray(normLoc);
    shaderProgram->setAttributeBuffer(normLoc, GL_FLOAT, offset, 3);

    m_indexBuffer.bind();

    functions->glDrawElements(drawMode, m_indexBuffer.size() - 1, GL_UNSIGNED_INT, nullptr);

    m_indexBuffer.release();
    m_vertexBuffer.release();

    m_texture->release();
}

void GLObject::GLModel3D::translate(const QVector3D &translation)
{
    m_modelMatrix.translate(translation);
}

void GLObject::GLModel3D::rotate(const QQuaternion &quat)
{
    m_modelMatrix.rotate(quat);
}

void GLObject::GLModel3D::scale(const float &factor)
{
    m_modelMatrix.scale(factor);
}
