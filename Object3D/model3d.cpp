#include <Object3D/model3d.h>
#include <QDebug>
#include <Utils/utils.h>
#include "triangulator.h"

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
    polygonElementsIndices(other.polygonElementsIndices)
{}

void Model3D::MeshData::triangulate()
{
    Triangulation::NaiveTriangulator tr;
    verticesIndicesTriangulated = tr.triangulate(verticesIndices,
                                                 polygonElementsIndices);
    texturesIndicesTriangulated = tr.triangulate(texturesIndices,
                                                 polygonElementsIndices);
    normalsIndicesTriangulated = tr.triangulate(normalsIndices,
                                                polygonElementsIndices);

    isTriangulated = true;
}

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

QVector<QVector3D> Model3D::MeshData::getTriangulatedVertices() const
{
    QVector<QVector3D> coords(verticesIndicesTriangulated.size());

    for (int i = 0; i < verticesIndicesTriangulated.size(); i++) {
        coords[i] = positions[verticesIndicesTriangulated[i]];
    }

    return coords;
}

QVector<QVector2D> Model3D::MeshData::getTriangulatedTextureCoords() const
{
    QVector<QVector2D> tex(texturesIndicesTriangulated.size());

    for (int i = 0; i < texturesIndicesTriangulated.size(); i++) {
        tex[i] = textureCoords[texturesIndicesTriangulated[i]];
    }

    return tex;
}

QVector<QVector3D> Model3D::MeshData::getTriangulatedNormals() const
{
    QVector<QVector3D> norms(normalsIndicesTriangulated.size());

    for (int i = 0; i < normalsIndicesTriangulated.size(); i++) {
        norms[i] = normals[normalsIndicesTriangulated[i]];
    }

    return norms;
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

bool Model3D::MeshData::operator==(const MeshData &other) const
{
// doing a trivial member-by-memeber comparison here
    using namespace ObjIntUtils;

    bool equals = true;

    equals = equals && Utils::fuzzyCompare(this->positions, other.positions);
    equals = equals && Utils::fuzzyCompare(this->normals, other.normals);
    equals = equals && Utils::fuzzyCompare(this->textureCoords, other.textureCoords);
    equals = equals && this->polygonElementsIndices ==
                                           other.polygonElementsIndices;
    equals = equals && this->verticesIndices == other.verticesIndices;
    equals = equals && this->verticesIndicesTriangulated ==
                                           other.verticesIndicesTriangulated;
    equals = equals && this->texturesIndices == other.texturesIndices;
    equals = equals && this->texturesIndicesTriangulated ==
                                           other.texturesIndicesTriangulated;
    equals = equals && this->normalsIndices == other.normalsIndices;
    equals = equals && this->normalsIndicesTriangulated ==
                                           other.normalsIndicesTriangulated;
    return equals;

}

bool Model3D::MeshData::operator!=(const Model3D::MeshData &other) const
{
    return !(*this == other);
}

Model3D::MeshData &Model3D::MeshData::operator=(const Model3D::MeshData &other)
{
    positions                          = other.positions;
    normals                            = other.normals;
    textureCoords                      = other.textureCoords;
    verticesIndices                    = other.verticesIndices;
    verticesIndicesTriangulated        = other.verticesIndicesTriangulated;
    texturesIndices                    = other.texturesIndices;
    texturesIndicesTriangulated        = other.texturesIndicesTriangulated;
    normalsIndices                     = other.normalsIndices;
    normalsIndicesTriangulated         = other.normalsIndicesTriangulated;
    polygonElementsIndices             = other.polygonElementsIndices;

    return *this;
}

bool Model3D::MeshData::isValid()
{
    // TODO
    return true;
}


