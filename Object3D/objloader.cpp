#include "objloader.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

MeshData OBJLoader::OBJLoader::load(const QString &filePath)
{
    using namespace Model3D;

    QFile objFile(filePath);
    if(!objFile.exists()) {
        qDebug() << "File for loading obj not found";
        return MeshData();
    }

    objFile.open(QIODevice::ReadOnly);

    QTextStream objFileText(&objFile);

    // temporary arrays for vertices and normals
    QVector<QVector3D> coords, normals;
    QVector<QVector2D> texturesCoords;

    // data for the model contained in the file
    MeshData modelData;

    while(!objFileText.atEnd()) {
        QString str = objFileText.readLine();
        QStringList lineTokens = str.split(" ");

        if(lineTokens[0] == "#") {
            qDebug() << "Comment in obj file: " << str;
        } else if (lineTokens[0] == "mtllib") {
            // process the provided material
            qDebug() << "Obj file has the material assigned:" << str;
        } else if (lineTokens[0] == "v") {
            coords.append(QVector3D(lineTokens[1].toFloat(),
                                        lineTokens[2].toFloat(),
                                        lineTokens[3].toFloat()));
        } else if (lineTokens[0] == "vt") {
            texturesCoords.append(QVector2D(lineTokens[1].toFloat(),
                                                  lineTokens[2].toFloat()));
        } else if (lineTokens[0] == "vn") {
            normals.append(QVector3D(lineTokens[1].toFloat(),
                                       lineTokens[2].toFloat(),
                                       lineTokens[3].toFloat()));
        } else if (lineTokens[0] == "f") {
            // in obj file, indeces are going from 1, they are not 0-based
            // 1 index - index of vertices array
            // 2 index - index of texture coordinates array
            // 3 index - index of normals array
            modelData.m_polygonVertices.append(modelData.m_positions.size());

            auto newPolygon = getPolygonInformation(lineTokens);

            for (auto vertex : newPolygon) {
                // each "vertex" contains info about
                // vertex that belongs to the polygon
                if (vertex[0] > 0)
                    modelData.m_positions.append(coords[vertex[0] - 1]);

                if (vertex[1] > 0) {
                    modelData.m_textureCoords.append(texturesCoords[vertex[1] - 1]);
                    modelData.m_polygonTextures.append(modelData.m_textureCoords.size());
                }
                if (vertex[2] > 0)
                    modelData.m_normals.append(normals[vertex[2] - 1]);
            }

        }
    }

    objFile.close();

    if (modelData.m_polygonVertices.size() > 0)
        modelData.m_polygonVertices.append(modelData.m_positions.size());
    if (modelData.m_polygonTextures.size() > 0)
        modelData.m_polygonTextures.append(modelData.m_textureCoords.size());

    return modelData;
}

MeshData OBJLoader::OBJLoader::triangulate(const MeshData &originalData)
{

}

// TODO process the errors like convertion errors
QVector<QVector3D> OBJLoader::OBJLoader::getPolygonInformation(QStringList &polygonInfoLine)
{
    QVector<QVector3D> polygonInformaion;
    // polygonInfoLine[0] == "f"
    for(int i = 1; i < polygonInfoLine.size(); i++) {
        QVector3D vertexIndicesInfo(0, 0, 0);
        auto vertexInfo = polygonInfoLine[i].split("/");
        // vertexInfo[i] contains info about 1 vertex
        // for example, 1/2/3, where 1 is position index,
        // 2 is texture coord index, 3 is normal index
        bool convertionSuccess = true;
        unsigned int positionInd = vertexInfo[0].toUInt(&convertionSuccess);
        if (convertionSuccess) {
            vertexIndicesInfo[0] = positionInd;
        }
        unsigned int textureInd = vertexInfo[1].toUInt(&convertionSuccess);
        if (convertionSuccess) {
            vertexIndicesInfo[1] = textureInd;
        }
        unsigned int normalInd = vertexInfo[2].toUInt(&convertionSuccess);
        if (convertionSuccess) {
            vertexIndicesInfo[2] = normalInd;
        }
        polygonInformaion.append(vertexIndicesInfo);
    }
    return polygonInformaion;
}
