#include "model3d.h"
#include <QOpenGLTexture>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

Model3D::Model3D() :
    m_vertexBuffer(QOpenGLBuffer::VertexBuffer),
    m_indexBuffer(QOpenGLBuffer::IndexBuffer),
    m_texture(nullptr)
{

}

Model3D::Model3D(const QVector<VertexData> &vertexData, const QVector<GLuint> &indeces, const QImage &texture)
{
    init(vertexData, indeces, texture);
}

Model3D::~Model3D()
{
    if(m_vertexBuffer.isCreated())
        m_vertexBuffer.destroy();
    if(m_indexBuffer.isCreated())
        m_indexBuffer.destroy();

    if(m_texture != nullptr && m_texture->isCreated())
        m_texture->destroy();
}

void Model3D::init(const QVector<VertexData> &vertexData, const QVector<GLuint> &indeces, const QImage &texture)
{
    if(m_vertexBuffer.isCreated())
        m_vertexBuffer.destroy();
    if(m_indexBuffer.isCreated())
        m_indexBuffer.destroy();

    if(m_texture != nullptr) {
        //if (m_texture->isCreated()) {
            //m_texture->destroy();
        //}
    }

    m_vertexBuffer.create();
    m_vertexBuffer.bind();
    m_vertexBuffer.allocate(vertexData.constData(), vertexData.size() * sizeof (VertexData));
    m_vertexBuffer.release();

    m_indexBuffer.create();
    m_indexBuffer.bind();
    m_indexBuffer.allocate(indeces.constData(), indeces.size() * sizeof (GLuint));
    m_indexBuffer.release();

    m_texture = new QOpenGLTexture(texture.mirrored()); // TODO why it should be mirrored?

    // TODO what are the first two things?
    // set nearest filtering mode for texture minification
    m_texture->setMinificationFilter(QOpenGLTexture::Nearest);
    // set bilinear filtering mode for texture magnification
    m_texture->setMagnificationFilter(QOpenGLTexture::Linear);
    // (1.1, 1.2) is the same tex coord as (0.1, 0.2)
    m_texture->setWrapMode(QOpenGLTexture::Repeat);
}

void Model3D::draw(QOpenGLShaderProgram *shaderProgram, QOpenGLFunctions *functions)
{

    if (!m_vertexBuffer.isCreated() || !m_indexBuffer.isCreated()) {
        return;
    }

    m_texture->bind(0);
    shaderProgram->setUniformValue("u_texture", 0);

    shaderProgram->setUniformValue("modelMatrix", m_modelMatrix);

    m_vertexBuffer.bind();

    int offset = 0;

    int vertLoc = shaderProgram->attributeLocation("vertexPosition");
    shaderProgram->enableAttributeArray(vertLoc);
    shaderProgram->setAttributeBuffer(vertLoc, GL_FLOAT, offset, 3, sizeof(VertexData));

    offset += sizeof(QVector3D);

    int texLoc = shaderProgram->attributeLocation("textureCoordinate");
    shaderProgram->enableAttributeArray(texLoc);
    shaderProgram->setAttributeBuffer(texLoc, GL_FLOAT, offset, 2, sizeof(VertexData));

    offset += sizeof(QVector2D);

    int normLoc = shaderProgram->attributeLocation("normal");
    shaderProgram->enableAttributeArray(normLoc);
    shaderProgram->setAttributeBuffer(normLoc, GL_FLOAT, offset, 3, sizeof(VertexData));

    m_indexBuffer.bind();

    functions->glDrawElements(GL_TRIANGLES, m_indexBuffer.size(), GL_UNSIGNED_INT, nullptr);

    m_vertexBuffer.release();
    m_indexBuffer.release();
    m_texture->release();
}
