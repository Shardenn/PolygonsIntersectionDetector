#include "Model3D.h"
#include <QOpenGLTexture>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QDebug>

Model3D::GLModel3D::GLModel3D() :
    m_vertexBuffer(QOpenGLBuffer::VertexBuffer),
    m_indexBuffer(QOpenGLBuffer::IndexBuffer),
    m_texture(nullptr)
{

}

Model3D::GLModel3D::GLModel3D(const QVector<VertexData> &vertexData,
                              const QVector<GLuint> &indeces,
                              const QImage &texture):
    GLModel3D()
{
    reinit(vertexData, indeces, texture);
}

Model3D::GLModel3D::GLModel3D(const MeshData& mesh,
                              const QImage& texture) :
    GLModel3D()
{
    reinit(mesh, texture);
}

Model3D::GLModel3D::~GLModel3D()
{
    if(m_vertexBuffer.isCreated())
        m_vertexBuffer.destroy();
    if(m_indexBuffer.isCreated())
        m_indexBuffer.destroy();

    if(m_texture != nullptr && m_texture->isCreated()) {
        // GLModel3D does not have vaild OGL context.
        // how to destroy a texture?
        m_texture->destroy();
        //const GLuint textureID = m_texture->textureId();
        //f->glDeleteTextures(1, &textureID);
    }
}

void Model3D::GLModel3D::reinit(const QVector<VertexData> &vertexData,
                                const QVector<GLuint> &indices,
                                const QImage &texture)
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

    // it is harder to operate with GL when the data is separated.
    // Seems like using tied vertex/textures/normals is a good decision
    m_vertexBuffer.create();
    m_vertexBuffer.bind();
    m_vertexBuffer.allocate(vertexData.constData(),
                            vertexData.size() * sizeof (VertexData));
    m_vertexBuffer.release();

    m_indexBuffer.create();
    m_indexBuffer.bind();
    m_indexBuffer.allocate(indices.constData(),
                           indices.size() * sizeof (GLuint));
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

void Model3D::GLModel3D::reinit(const MeshData& mesh,
                                const QImage& texture)
{
    // we will create VertexData vector to call another
    // overload of reinit function

    // get how much elements in QVector<VertexData> do we need
    auto elementsCount = qMax(mesh.m_positions.size(),
                            qMax(mesh.m_textureCoords.size(), mesh.m_normals.size()));
    QVector<VertexData> vertexData(elementsCount);

    // reset the iterator and assign an element to
    // our vertexData
    auto it = vertexData.begin();
    for(auto position : mesh.m_positions) {
        it->m_position = position;
        it++;
    }
    it = vertexData.begin();
    for(auto texture : mesh.m_textureCoords) {
        it->m_textureCoordinate = texture;
        it++;
    }
    it = vertexData.begin();
    for(auto normal : mesh.m_normals) {
        it->m_normal = normal;
        it++;
    }
/*  TODO does meshData stores indices this way? What way?
    QVector<GLuint> indices;
    auto indicesCount = mesh.m_polygonVertIndices.last();
    for(unsigned int i = 0; i < indicesCount; i++) {
        indices.append(i);
    }

    reinit(vertexData, indices, texture);
*/
    reinit(vertexData, mesh.m_polygonVertices, texture);
}

void Model3D::GLModel3D::draw(QOpenGLShaderProgram *shaderProgram,
                              QOpenGLFunctions *f)
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

    f->glDrawElements(GL_TRIANGLES, m_indexBuffer.size()-1, GL_UNSIGNED_INT, nullptr);

    m_vertexBuffer.release();
    m_indexBuffer.release();
    m_texture->release();
}

void Model3D::GLModel3D::translate(const QVector3D &translation)
{
    m_modelMatrix.translate(translation);
}
