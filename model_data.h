#ifndef MESH_DATA_H
#define MESH_DATA_H

#include <QVector3D>
#include <QVector2D>
#include <QVector>

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

class MeshData
{
public:
    /*!
     * \brief Vector of vertex data of the model stored flat
     */
    QVector<VertexData> m_vertexesData;
    /*!
     * \brief Vector of polygon vertexes indexes.
     * i-th position contains (m_polygonVertIndexes[i+1] - m_polygonVertIndexes[i])
     * vertexes data for i-th polygon
     */
    QVector<unsigned int> m_polygonVertIndexes;

    MeshData()
    {
        m_vertexesData = QVector<VertexData>();
        m_polygonVertIndexes = QVector<unsigned int>();
    }

    MeshData(QVector<VertexData> vData, QVector<unsigned int> indexes) :
        m_vertexesData(vData),
        m_polygonVertIndexes(indexes)
    {}
    // both members are trivial and public, do we need the method?
    void append(VertexData newVertex, bool newPolygon = false)
    {
        if(newPolygon) {
            m_polygonVertIndexes.append(m_polygonVertIndexes.size());
        }
        m_vertexesData.append(newVertex);
    }

    QVector<VertexData> getVertexesOfPolygonID(const unsigned long polygonID)
    {
        // size - 1 because we are 0-based. size - 2 because vertIndexes contains 1 more number in the end
        if(polygonID > m_polygonVertIndexes.size() - 2) {
            return QVector<VertexData>();
        }
        auto firstVertex = m_polygonVertIndexes[polygonID];
        auto numVertexes = m_polygonVertIndexes[polygonID + 1] - firstVertex;

        return m_vertexesData.mid(firstVertex, numVertexes);
    }
};

#endif // MODEL_DATA_H
