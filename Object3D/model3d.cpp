#include <Object3D/model3d.h>
#include <QDebug>

Model3D::MeshData::MeshData()
{}

Model3D::MeshData::MeshData(const MeshData &other) :
    positions(other.positions),
    textureCoords(other.textureCoords),
    normals(other.normals),
    verticesIndices(other.verticesIndices),
    verticesIndicesTriangulated(other.verticesIndicesTriangulated),
    texturesIndices(other.texturesIndices),
    texturesIndicesTriangulated(other.texturesIndicesTriangulated),
    normalsIndices(other.normalsIndices),
    normalsIndicesTriangulated(other.normalsIndicesTriangulated),
    polygonElementsIndices(other.polygonElementsIndices),
    polygonElementsIndicesTriangulated(other.polygonElementsIndicesTriangulated)
{}

QVector<QVector3D> Model3D::MeshData::getChronologicalVerticesCoords() const
{
    QVector<QVector3D> coords(verticesIndices.size());

    for (int index = 0; index < verticesIndices.size(); index++) {
        coords[index] = positions[verticesIndices[index]];
    }

    return coords;
}

QVector<QVector2D> Model3D::MeshData::getChronologicalTexturesCoords() const
{
    QVector<QVector2D> textures(texturesIndices.size());

    for (int i = 0; i < texturesIndices.size(); i++) {
        textures[i] = textureCoords[texturesIndices[i]];
    }

    return textures;
}

QVector<QVector3D> Model3D::MeshData::getChronologicalNormalsCoords() const
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
                                int& numVertices) const
{
    // "-1" because vertIndices contains 1 more number in the end
    Q_ASSERT(polygonID > intervalSource.size() - 1);

    firstVertexNumber = intervalSource[polygonID];
    numVertices = intervalSource[polygonID + 1] - firstVertexNumber;
}

bool Model3D::MeshData::isValid()
{
    return true;
}


