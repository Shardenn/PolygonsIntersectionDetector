#pragma once

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
    /*!
     * \brief stores starting vertices\normals position
     * indices for each polygon
     * e.g. to get i'th polygon vertices\normals:
     * the polygon has exactly m_polygonVertices[i+1]
     * vertices\normals;
     * the first vertex\normal from m_positions
     * (or m_normals)
     * vector, that belongs to the polygon, is
     * m_polygonVertices[i]
     */
    QVector<indexNumber_t> m_polygonVertices;
    /*!
     * \brief stores starting texture coordinates
     * indices for each polygon
     * e.g. to get i'th polygon texture coordinates:
     * the polygon has exactly m_polygonTextures[i+1]
     * texture coordinates;
     * the first texture coordinate from m_textureCoords
     * vector, that belongs to the polygon, is
     * m_polygonTextures[i]
     */
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
        getPolygonVerticesInterval(polygonID, m_polygonVertices, firstVertex, numVertices);
        return m_positions.mid(firstVertex, numVertices);
    }

    QVector<QVector2D> getPolygonTextureCoords(const indexNumber_t polygonID)
    {
        indexNumber_t firstVertex, numVertices;
        getPolygonVerticesInterval(polygonID, m_polygonTextures, firstVertex, numVertices);
        return m_textureCoords.mid(firstVertex, numVertices);
    }

    QVector<QVector3D> getPolygonNormals(const indexNumber_t polygonID)
    {
        indexNumber_t firstVertex, numVertices;
        getPolygonVerticesInterval(polygonID, m_polygonVertices, firstVertex, numVertices);
        return m_normals.mid(firstVertex, numVertices);
    }
/*
    const inline bool operator==(const MeshData& other)
    {
        // doing a trivial member-by-memeber comparison here
        return this->m_positions == other.m_positions &&
                this->m_normals == other.m_normals &&
                this->m_textureCoords == other.m_textureCoords &&
                this->m_polygonTextures == other.m_polygonTextures &&
                this->m_polygonVertices == other.m_polygonVertices;
    }

    const inline bool operator!=(const MeshData& other)
    {
        return !(*this == other);
    }
*/
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
                                    QVector<indexNumber_t>& intervalSource,
                                    indexNumber_t& firstVertexNumber,
                                    indexNumber_t& numVertices)
    {
        // "-1" because vertIndices contains 1 more number in the end
        Q_ASSERT(polygonID > intervalSource.size() - 1);

        firstVertexNumber = intervalSource[polygonID];
        numVertices = intervalSource[polygonID + 1] - firstVertexNumber;
    }
};

inline bool operator==(const MeshData& a, const MeshData& b)
{
    return a.m_positions        == b.m_positions &&
            a.m_normals         == b.m_normals &&
            a.m_textureCoords   == b.m_textureCoords &&
            a.m_polygonTextures == b.m_polygonTextures &&
            a.m_polygonVertices == b.m_polygonVertices;
}

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
