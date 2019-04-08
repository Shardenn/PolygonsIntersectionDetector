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

// TODO delete vertexData
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
    /*!
     * \brief Vector of vertices positions
     */
    QVector<QVector3D> positions;

    QVector<QVector3D> positionsTriangulated;

    /*!
     * \brief Vector of vertex data of the model stored flat
     */
    QVector<QVector2D> textureCoords;
    /*!
     * \brief Vector of normals for each vertex
     */
    QVector<QVector3D> normals;
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
    QVector<int> polygonVertices;

    QVector<int> polygonVerticesTriangulated;
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
    QVector<int> polygonTextures;
    MeshData();

    MeshData(QVector<QVector3D>& vertices,
             QVector<QVector2D>& textureCoords,
             QVector<QVector3D>& normals,
             QVector<int>& indices);

    // duplicate. Can we get rig of them?
    QVector<QVector3D> getPolygonVertices(const int polygonID);

    QVector<QVector2D> getPolygonTextureCoords(const int polygonID);

    QVector<QVector3D> getPolygonNormals(const int polygonID);

    inline bool operator==(const MeshData& other) const
    {
        // doing a trivial member-by-memeber comparison here
        return this->positions == other.positions &&
                this->normals == other.normals &&
                this->textureCoords == other.textureCoords &&
                this->polygonTextures == other.polygonTextures &&
                this->polygonVertices == other.polygonVertices;
    }

    inline bool operator!=(const MeshData& other) const
    {
        return !(*this == other);
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
    void getPolygonVerticesInterval(const int polygonID,
                                    QVector<int>& intervalSource,
                                    int& firstVertexNumber,
                                    int& numVertices);
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
