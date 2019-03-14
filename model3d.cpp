#include "model3d.h"
#include <QOpenGLTexture>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>

QModel3D::QModel3D() :
    m_vertexBuffer(QOpenGLBuffer::VertexBuffer),
    m_indexBuffer(QOpenGLBuffer::IndexBuffer),
    m_texture(nullptr)
{

}

QModel3D::QModel3D(const QVector<QVertexData> &vertexData, const QVector<GLuint> &indeces, const QImage &texture)
{
    init(vertexData, indeces, texture);
}

QModel3D::~QModel3D()
{
    if(m_vertexBuffer.isCreated())
        m_vertexBuffer.destroy();
    if(m_indexBuffer.isCreated())
        m_indexBuffer.destroy();

    if(m_texture != nullptr && m_texture->isCreated())
        m_texture->destroy();
}

void QModel3D::init(const QVector<QVertexData> &vertexData, const QVector<GLuint> &indeces, const QImage &texture)
{
    m_vertexBuffer.create();
    m_vertexBuffer.bind();
    m_vertexBuffer.allocate(vertexData.constData(), vertexData.size() * sizeof (QVertexData));

    m_indexBuffer.create();
    m_indexBuffer.bind();
    m_indexBuffer.allocate(indeces.constData(), indeces.size() * sizeof (GLuint));

    m_texture = new QOpenGLTexture(texture.mirrored()); // TODO why it should be mirrored?

    // TODO what are the first two things?
    // set nearest filtering mode for texture minification
    m_texture->setMinificationFilter(QOpenGLTexture::Nearest);
    // set bilinear filtering mode for texture magnification
    m_texture->setMagnificationFilter(QOpenGLTexture::Linear);
    // (1.1, 1.2) is the same tex coord as (0.1, 0.2)
    m_texture->setWrapMode(QOpenGLTexture::Repeat);
}

void QModel3D::draw(QOpenGLShaderProgram *shaderProgram, QOpenGLFunctions *functions)
{
    m_texture->bind(0);
    // TODO initialize the shader program here.
    // TODO what are some values in the shader programs? How they work exactly?
    //shaderProgram->setUniformValue()
}
