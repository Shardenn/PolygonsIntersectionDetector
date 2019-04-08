#include "objloader.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

MeshData *OBJLoader::OBJLoader::load(const QString &filePath)
{
    QFile objFile(filePath);
    if(!objFile.exists()) {
        qDebug() << "File for loading obj not found";
        return nullptr;
    }

    objFile.open(QIODevice::ReadOnly);

    QTextStream textStream(&objFile);

    auto ret =  load(textStream);

    objFile.close();

    return ret;
}

MeshData *OBJLoader::OBJLoader::load(QTextStream &textStream)
{
    using namespace Model3D;

    // temporary arrays for vertices and normals
    QVector<QVector3D> coords, normals;
    QVector<QVector2D> texturesCoords;

    // data for the model contained in the file
    MeshData *modelData = new MeshData;

    while(!textStream.atEnd()) {
        QString str = textStream.readLine();
        QStringList lineTokens = str.split(" ");

        if(lineTokens[0] == "#") {
            qDebug() << "Comment in obj file: " << str;
        } else if (lineTokens[0] == "mtllib") {
            // process the provided material
            qDebug() << "Obj file has the material assigned:" << str;
        } else if (lineTokens[0] == "v") {
            QVector<float> parsedCoords = parseCoordsLine(str);

            if (parsedCoords.size() != 3) {
                qDebug() << "Line " + str + " has either more or less than 3 arguments for texture";
                continue;
            }

            QVector3D newCoord(parsedCoords[0], parsedCoords[1], parsedCoords[2]);
            coords.append(newCoord);
        } else if (lineTokens[0] == "vt") {
            QVector<float> parsedCoords = parseCoordsLine(str);

            if (parsedCoords.size() != 2) {
                qDebug() << "Line " + str + " has either more or less than 2 arguments for texture";
                continue;
            }

            QVector2D newTexture(parsedCoords[0], parsedCoords[1]);
            texturesCoords.append(newTexture);
        } else if (lineTokens[0] == "vn") {
            QVector<float> parsedCoords = parseCoordsLine(str);

            if (parsedCoords.size() != 3) {
                qDebug() << "Line " + str + " has either more or less than 3 arguments for texture";
                continue;
            }

            QVector3D newNormal(parsedCoords[0], parsedCoords[1], parsedCoords[2]);
            normals.append(newNormal);
        } else if (lineTokens[0] == "f") {
            modelData->polygonVertices.append(modelData->positions.size());

            QVector<QVector3D> newPolygon = getPolygonInformation(lineTokens);

            for (auto vertexInfo : newPolygon) {
                // each "vertex" contains info about
                // vertex that belongs to the polygon
                if (vertexInfo[0] > 0)
                    modelData->positions.append(coords[vertexInfo[0] - 1]);

                if (vertexInfo[1] > 0) {
                    modelData->textureCoords.append(texturesCoords[vertexInfo[1] - 1]);
                    modelData->polygonTextures.append(modelData->textureCoords.size());
                }
                if (vertexInfo[2] > 0)
                    modelData->normals.append(normals[vertexInfo[2] - 1]);
            }

        }
    }

    if (modelData->polygonVertices.size() > 0)
        modelData->polygonVertices.append(modelData->positions.size());
    if (modelData->polygonTextures.size() > 0)
        modelData->polygonTextures.append(modelData->textureCoords.size());

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
        QVector3D vertexIndicesInfo;
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

QVector<float> parseLine(const QString& line)
{
    QStringList tokens = line.split(" ");

    QVector<QString> readTokens;
    for (auto token : tokens) {
        readTokens.append(token);
    }
    // remove "v / vt / vn" symbol itself
    readTokens.erase(readTokens.begin());

    QVector<float> parsedCoords;
    for (auto token : readTokens) {
        bool isConvertionSuccess{ false };
        float coord = token.toFloat(&isConvertionSuccess);
        if (isConvertionSuccess) {
            parsedCoords.append(coord);
        }
    }
    return parsedCoords;
}
