#ifndef MODEL3D_H
#define MODEL3D_H

#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QVector2D>
#include <QVector3D>

namespace  Model3D {

class MeshData
{
public:
    MeshData();
    MeshData(const MeshData &other);

    /*!
     * \brief Vector of vertices positions
     */
    QVector<QVector3D> positions;

    /*!
     * \brief Vector of all textureCoords
     */
    QVector<QVector2D> textureCoords;
    /*!
     * \brief Vector of normals
     */
    QVector<QVector3D> normals;

    /*!
     * \brief Indices of the vertices in their chonological
     * order that is found in the "f" lines
     */
    QVector<int> verticesIndices;

    QVector<int> verticesIndicesTriangulated;
    /*!
     * \brief Indices of the texture coordinates in
     * their chronological order in "f" lines
     */
    QVector<int> texturesIndices;

    QVector<int> texturesIndicesTriangulated;
    /*!
     * \brief Indices of the normals in their chronological
     * order in "f" lines
     */
    QVector<int> normalsIndices;

    QVector<int> normalsIndicesTriangulated;
    /*!
     * \brief stores starting verts/text/normals position
     * indices for each polygon
     * e.g. to get i'th polygon verts/text/normals:
     * the polygon has exactly polygonElementsIndices[i+1]
     * verts/text/normals;
     * the first verts/text/normals from positions
     * (or normals/textureCoords)
     * vector, that belongs to the polygon, is
     * polygonElementsIndices[i]
     */
    QVector<int> polygonElementsIndices;

    void triangulate();

    QVector<QVector3D> getChronologicalVerticesCoords() const;
    QVector<QVector2D> getChronologicalTexturesCoords() const;
    QVector<QVector3D> getChronologicalNormalsCoords() const;

    QVector<QVector3D> getTriangulatedVertices() const;
    QVector<QVector2D> getTriangulatedTextureCoords() const;
    QVector<QVector3D> getTriangulatedNormals() const;

    QVector<QVector3D> getPolygonVertices(const int polygonID);
    QVector<QVector2D> getPolygonTextureCoords(const int polygonID);
    QVector<QVector3D> getPolygonNormals(const int polygonID);

    /*!
     * \brief isValid
     * checks where the MeshData object is valid.
     * For example, if a face points that it uses
     * vertices with indices that do not exist,
     * then it false
     * \return false is MeshData has some invalid data
     * true otherwise
     */
    bool isValid();

    bool operator==(const MeshData& other) const;

    bool operator!=(const MeshData& other) const;

    MeshData &operator=(const MeshData &other);

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
                                    int& numVertices) const;
};


} // namespace Model3D

#endif // MODEL3D_H
