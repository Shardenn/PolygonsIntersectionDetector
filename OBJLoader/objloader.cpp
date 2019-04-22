#include "objloader.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <memory>

MeshData *OBJLoader::OBJLoader::load(const QString &filePath,
                                     bool smoothNormals, bool doTriangulation)
{
    QFile objFile(filePath);
    if(!objFile.exists()) {
        qDebug() << "File for loading obj not found";
        return nullptr;
    }

    objFile.open(QIODevice::ReadOnly);

    QTextStream textStream(&objFile);

    auto ret = load(textStream, smoothNormals, doTriangulation);

    objFile.close();

    return ret;
}

MeshData *OBJLoader::OBJLoader::load(QTextStream &textStream,
                                     bool smoothNormals, bool doTriangulation)
{
    using namespace Model3D;

    // data for the model contained in the file
    auto modelData = new MeshData;

    if (textStream.atEnd()) {
        delete modelData;
        return nullptr;
    }

    while (!textStream.atEnd()) {
        QString str = textStream.readLine();
        str = str.simplified(); // delete leading and trailing whitespaces
                                // and remove several whitespaces
        QStringList lineTokens = str.split(" ");

        if(lineTokens[0] == "#") {
            qDebug() << "Comment in obj file: " << str;
        } else if (lineTokens[0] == "mtllib") {
            // process the provided material
            qDebug() << "Obj file has the material assigned:" << str;
        } else if (lineTokens[0] == "g") {
            qDebug() << "Obj file has group in line " + str;
        } else if (lineTokens[0] == "v") {
            QVector<float> parsedCoords = parseLine(str);

            if (parsedCoords.size() != 3) {
                qDebug() << "Line \'" + str + "\' has either more or less than 3 arguments for vertex";
                m_isLoadSuccessful = false;
                break;
            }

            QVector3D newCoord(parsedCoords[0], parsedCoords[1], parsedCoords[2]);
            modelData->positions.append(newCoord);
        } else if (lineTokens[0] == "vt") {
            QVector<float> parsedCoords = parseLine(str);

            if (parsedCoords.size() != 2) {
                qDebug() << "Line " + str + " has either more or less than 2 arguments for texture";
                m_isLoadSuccessful = false;
                break;
            }

            QVector2D newTexture(parsedCoords[0], parsedCoords[1]);
            modelData->textureCoords.append(newTexture);
        } else if (lineTokens[0] == "vn") {
            QVector<float> parsedCoords = parseLine(str);

            if (parsedCoords.size() != 3) {
                qDebug() << "Line " + str + " has either more or less than 3 arguments for normal";
                m_isLoadSuccessful = false;
                break;
            }

            QVector3D newNormal(parsedCoords[0], parsedCoords[1], parsedCoords[2]);
            modelData->normals.append(newNormal);
        } else if (lineTokens[0] == "f") {
            modelData->polygonElementsIndices.append(modelData->verticesIndices.size());

            QVector<QVector3D> newPolygon = getPolygonInformation(lineTokens);

            // each vertex info is vertex info in the polygon
            // e.g. f 1/2/3 will come out in QVector3D(1,2,3)
            for (auto vertexInfo : newPolygon) {
                if (vertexInfo[0] > 0) {
                    modelData->verticesIndices.append(vertexInfo[0] - 1);
                }
                if (vertexInfo[1] > 0) {
                    modelData->texturesIndices.append(vertexInfo[1] - 1);
                }
                if (vertexInfo[2] > 0) {
                    modelData->normalsIndices.append(vertexInfo[2] - 1);
                }
            }

        }
    }

    if (modelData->polygonElementsIndices.size() > 0)
        modelData->polygonElementsIndices.append(modelData->verticesIndices.size());

    // check if the OBJ file had enough data and faces indices
    // are not out of range


    if (!m_isLoadSuccessful) {
        qDebug() << "OBJ file was loaded with errors. See output for details";
        delete modelData;
        return nullptr;
    }

    if (doTriangulation)
        modelData->triangulate();

    if (modelData->isTriangulated && smoothNormals)
        modelData->smoothNormals();

    return modelData;
}


// TODO process the errors like convertion errors
QVector<QVector3D> OBJLoader::OBJLoader::getPolygonInformation(QStringList &polygonInfoLine)
{
    QVector<QVector3D> polygonInformaion;
    // polygonInfoLine[0] == "f"
    for(int i = 1; i < polygonInfoLine.size(); ++i) {
        QVector3D vertexIndicesInfo;
        auto vertexInfo = polygonInfoLine[i].split("/");
        // vertexInfo[i] contains info about 1 vertex
        // for example, 1/2/3, where 1 is position index,
        // 2 is texture coord index, 3 is normal index
        bool isConvertionSuccess = true;

        int j = 0;
        while (j < vertexInfo.size() && j < 3) {
            unsigned int index = vertexInfo[j].toUInt(&isConvertionSuccess);
            if (isConvertionSuccess) {
                vertexIndicesInfo[j] = index;
            }
            j++;
        }

        polygonInformaion.append(vertexIndicesInfo);
    }
    return polygonInformaion;
}

QVector<float> OBJLoader::OBJLoader::parseLine(const QString& line)
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
        m_isLoadSuccessful &= isConvertionSuccess;
        if (isConvertionSuccess) {
            parsedCoords.append(coord);
        }
    }
    return parsedCoords;
}
