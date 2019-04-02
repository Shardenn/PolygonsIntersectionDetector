#ifndef MODEL3D_H
#define MODEL3D_H

#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QVector2D>
#include <QVector3D>

class QOpenGLTexture;
class QOpenGLFunctions;
class QOpenGLShaderProgram;

namespace  Model3D {

class VertexData
{
public:
    QVector3D m_position;
    QVector2D m_textureCoordinate;
    QVector3D m_normal;

    VertexData(){}

    VertexData(QVector3D position, QVector2D textureCoord, QVector3D normal) :
        m_position(position),
        m_textureCoordinate(textureCoord),
        m_normal(normal)
    {
    }
};

/// unsigned long or int? How much vertices do
/// we usually have?
class MeshData
{
public:
    using indexNumber_t = unsigned int;
    /*!
     * \brief Vector of vertices positions
     */
    QVector<QVector3D> m_positions;
    /*!
     * \brief Vector of vertex data of the model stored flat
     */
    QVector<QVector2D> m_textureCoords;
    /*!
     * \brief Vector of normals for each vertex
     */
    QVector<QVector3D> m_normals;
/*
    QVector<indexNumber_t> m_positionIndices;
    QVector<indexNumber_t> m_textureIndices;
    QVector<indexNumber_t> m_normalIndices;
*/
    QVector<indexNumber_t> m_polygonVertices;
    QVector<indexNumber_t> m_polygonTextures;
    MeshData() {}

    MeshData(QVector<QVector3D>& vertices,
             QVector<QVector2D>& textureCoords,
             QVector<QVector3D>& normals,
             QVector<indexNumber_t>& indices) :
        m_positions(vertices),
        m_textureCoords(textureCoords),
        m_normals(normals),
        m_polygonVertices(indices)
    {}

    // duplicate. Can we get rig of them?
    QVector<QVector3D> getPolygonVertices(const indexNumber_t polygonID)
    {
        indexNumber_t firstVertex, numVertices;
        getPolygonVerticesInterval(polygonID, firstVertex, numVertices);
        return m_positions.mid(firstVertex, numVertices);
    }

    QVector<QVector2D> getPolygonTextureCoords(const indexNumber_t polygonID)
    {
        indexNumber_t firstVertex, numVertices;
        getPolygonVerticesInterval(polygonID, firstVertex, numVertices);
        return m_textureCoords.mid(firstVertex, numVertices);
    }

    QVector<QVector3D> getPolygonNormals(const indexNumber_t polygonID)
    {
        indexNumber_t firstVertex, numVertices;
        getPolygonVerticesInterval(polygonID, firstVertex, numVertices);
        return m_normals.mid(firstVertex, numVertices);
    }

private:
    /*!
     * \brief getPolygonVerticesInterval
     *
     * \param polygonID - number of the polygon in
     * m_polygonVertIndices vector. 0-based
     * \param firstVertexNumber - first vertex that the polygon has
     * in m_verticesPositions vector
     * \param numVertices - number of vertices that the polygon includes
     * \return true if the ID is valid
     * false otherwise on in case of an error
     */
    void getPolygonVerticesInterval(const indexNumber_t polygonID,
                                    indexNumber_t& firstVertexNumber,
                                    indexNumber_t& numVertices)
    {
        // "-1" because vertIndices contains 1 more number in the end
        Q_ASSERT(polygonID > m_polygonVertices.size() - 1);

        firstVertexNumber = m_polygonVertices[polygonID];
        numVertices = m_polygonVertices[polygonID + 1] - firstVertexNumber;
    }
};

class GLModel3D
{
public:
    GLModel3D();
    GLModel3D(const QVector<VertexData>& vertexData,
             const QVector<GLuint>& indeces,
             const QImage& texture);

    GLModel3D(const MeshData& meshData,
              const QImage& texture);

    ~GLModel3D();

    void reinit(const QVector<VertexData> &vertexData,
         const QVector<GLuint> &indeces,
         const QImage &texture);

    void reinit(const MeshData& meshData,
                const QImage& texture);

    void draw(QOpenGLShaderProgram *shaderProgram,
              QOpenGLFunctions *functions);

    void translate(const QVector3D &translation);
private:
    QOpenGLBuffer   m_vertexBuffer;
    QOpenGLBuffer   m_indexBuffer;
    QMatrix4x4      m_modelMatrix;
    QOpenGLTexture *m_texture;
    QOpenGLFunctions* f;
};

} // namespace Model3D

#endif // MODEL3D_H
