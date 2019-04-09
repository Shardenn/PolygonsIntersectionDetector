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

class MeshData
{
public:
    /*!
     * \brief Vector of vertices positions
     */
    QVector<QVector3D> positions;

    /*!
     * \brief Vector of vertex data of the model stored flat
     */
    QVector<QVector2D> textureCoords;
    /*!
     * \brief Vector of normals for each vertex
     */
    QVector<QVector3D> normals;

    /*!
     * \brief Indices of the vertices in their chonological
     * order that is found in the "f" lines
     */
    QVector<int> verticesIndices;

    /*!
     * \brief Indices of the texture coordinates in
     * their chronological order in "f" lines
     */
    QVector<int> texturesIndices;
    /*!
     * \brief Indices of the normals in their chronological
     * order in "f" lines
     */
    QVector<int> normalsIndices;
    /*!
     * \brief stores starting vertices position
     * indices for each polygon
     * e.g. to get i'th polygon vertices:
     * the polygon has exactly m_polygonVertices[i+1]
     * vertices;
     * the first vertex from m_positions
     * (or m_normals)
     * vector, that belongs to the polygon, is
     * m_polygonVertices[i]
     */
    QVector<int> polygonElementsIndices;

    MeshData();

    QVector<QVector3D> getCronologicalVerticesCoords() const;
    QVector<QVector2D> getCronologicalTexturesCoords() const;
    QVector<QVector3D> getCronologicalNormalsCoords() const;
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
                this->polygonElementsIndices == other.polygonElementsIndices;
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
    GLModel3D(const MeshData& meshData,
              const QImage& texture);

    ~GLModel3D();

    void reinit(const MeshData& meshData,
                const QImage& texture);

    void draw(QOpenGLShaderProgram *shaderProgram,
              QOpenGLFunctions *functions);

    void translate(const QVector3D &translation);
private:
    QOpenGLBuffer     m_vertexBuffer;
    QOpenGLBuffer     m_indexBuffer;
    QMatrix4x4        m_modelMatrix;
    QOpenGLTexture   *m_texture = nullptr;
    QOpenGLFunctions *m_openglFunctions = nullptr;
    MeshData          m_meshData;
};

} // namespace Model3D

#endif // MODEL3D_H
