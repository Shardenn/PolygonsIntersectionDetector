#include <Object3D/model3d.h>
#include <QOpenGLTexture>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QDebug>

Model3D::MeshData::MeshData()
{}

QVector<QVector3D> Model3D::MeshData::getCronologicalVerticesCoords() const
{
    QVector<QVector3D> coords(verticesIndices.size());

    for (int index = 0; index < verticesIndices.size(); index++) {
        coords[index] = positions[verticesIndices[index]];
    }

    return coords;
}

QVector<QVector2D> Model3D::MeshData::getCronologicalTexturesCoords() const
{
    QVector<QVector2D> textures(texturesIndices.size());

    for (int i = 0; i < texturesIndices.size(); i++) {
        textures[i] = textureCoords[texturesIndices[i]];
    }

    return textures;
}

QVector<QVector3D> Model3D::MeshData::getCronologicalNormalsCoords() const
{
    QVector<QVector3D> allNormals(normalsIndices.size());

    for (int i = 0; i < normalsIndices.size(); i++) {
        allNormals[i] = normals[normalsIndices[i]];
    }

    return allNormals;
}

QVector<QVector3D> Model3D::MeshData::getPolygonVertices(const int polygonID)
{
    int firstVertex, numVertices;
    getPolygonVerticesInterval(polygonID, polygonElementsIndices, firstVertex, numVertices);
    return positions.mid(firstVertex, numVertices);
}

QVector<QVector2D> Model3D::MeshData::getPolygonTextureCoords(const int polygonID)
{
    int firstVertex, numVertices;
    getPolygonVerticesInterval(polygonID, polygonElementsIndices, firstVertex, numVertices);
    return textureCoords.mid(firstVertex, numVertices);
}

QVector<QVector3D> Model3D::MeshData::getPolygonNormals(const int polygonID)
{
    int firstVertex, numVertices;
    getPolygonVerticesInterval(polygonID, polygonElementsIndices, firstVertex, numVertices);
    return normals.mid(firstVertex, numVertices);
}

void Model3D::MeshData::getPolygonVerticesInterval(const int polygonID,
                                QVector<int>& intervalSource,
                                int& firstVertexNumber,
                                int& numVertices)
{
    // "-1" because vertIndices contains 1 more number in the end
    Q_ASSERT(polygonID > intervalSource.size() - 1);

    firstVertexNumber = intervalSource[polygonID];
    numVertices = intervalSource[polygonID + 1] - firstVertexNumber;
}

Model3D::GLModel3D::GLModel3D() :
    m_vertexBuffer(QOpenGLBuffer::VertexBuffer),
    m_indexBuffer(QOpenGLBuffer::IndexBuffer)
{

}

Model3D::GLModel3D::GLModel3D(const MeshData& mesh,
                              const QImage& texture) :
    GLModel3D()
{
    m_meshData = mesh;
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
        delete m_texture;
        //const GLuint textureID = m_texture->textureId();
        //f->glDeleteTextures(1, &textureID);
    }
}

void Model3D::GLModel3D::reinit(const MeshData& mesh,
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
    QVector<QVector3D> allPositions = mesh.getCronologicalVerticesCoords();
    m_vertexBuffer.write(offset, allPositions.constData(),
                                 allPositions.size() * sizeof(QVector3D));

    offset += allPositions.size() * sizeof(QVector3D);
    QVector<QVector2D> allTextures = mesh.getCronologicalTexturesCoords();
    m_vertexBuffer.write(offset, allTextures.constData(),
                                 allTextures.size() * sizeof(QVector2D));

    offset += allTextures.size()  * sizeof(QVector2D);
    QVector<QVector3D> allNormals = mesh.getCronologicalNormalsCoords();
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
    m_texture = new QOpenGLTexture(texture.mirrored()); // TODO why it should be mirrored?

    // TODO what are the first two things?
    // set nearest filtering mode for texture minification
    m_texture->setMinificationFilter(QOpenGLTexture::Nearest);
    // set bilinear filtering mode for texture magnification
    m_texture->setMagnificationFilter(QOpenGLTexture::Linear);
    // (1.1, 1.2) is the same tex coord as (0.1, 0.2)
    m_texture->setWrapMode(QOpenGLTexture::Repeat);
}

void Model3D::GLModel3D::draw(QOpenGLShaderProgram *shaderProgram,
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

    functions->glDrawElements(GL_TRIANGLES, m_indexBuffer.size()-1, GL_UNSIGNED_INT, nullptr);

    m_indexBuffer.release();
    m_vertexBuffer.release();

    m_texture->release();
}

void Model3D::GLModel3D::translate(const QVector3D &translation)
{
    m_modelMatrix.translate(translation);
}
