#include "objdataprocessor.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

MeshData ObjDataProcessor::getModelData(const QString &filePath)
{
    QFile objFile(filePath);
    if(!objFile.exists()) {
        qDebug() << "File for loading obj not found";
        return MeshData();
    }

    objFile.open(QIODevice::ReadOnly);

    QTextStream objFileText(&objFile);

    // temp containers for setting data according to "faces" data
    QVector<QVector3D> verticesCoord;
    QVector<QVector3D> normals;
    QVector<QVector2D> textureCoordinates;

    // data for the model contained in the file
    MeshData modelData;

    while(!objFileText.atEnd()) {
        QString str = objFileText.readLine();
        QStringList lineTokens = str.split(" ");

        if(lineTokens[0] == "#") {
            qDebug() << "Comment in obj file: " << str;
        } else if(lineTokens[0] == "mtllib") {
            // process the provided material
            qDebug() << "Obj file has the material assigned:" << str;
        } else if (lineTokens[0] == "v") {
            verticesCoord.append(QVector3D(lineTokens[1].toFloat(),
                                        lineTokens[2].toFloat(),
                                        lineTokens[3].toFloat()));
        } else if(lineTokens[0] == "vt") {
            textureCoordinates.append(QVector2D(lineTokens[1].toFloat(),
                                                  lineTokens[2].toFloat()));
        } else if(lineTokens[0] == "vn") {
            normals.append(QVector3D(lineTokens[1].toFloat(),
                                       lineTokens[2].toFloat(),
                                       lineTokens[3].toFloat()));
        } else if(lineTokens[0] == "f") {
            // in obj file, indeces are going from 1, they are not 0-based
            // 1 index - index of vertices array
            // 2 index - index of texture coordinates array
            // 3 index - index of normals array
            modelData.m_polygonVertIndexes.append(modelData.m_polygonVertIndexes.size());

            for(int i = 1; i < lineTokens.size(); i++) {
                QStringList vertexDataIndex = lineTokens[i].split("/");
                VertexData newVertex(     verticesCoord[vertexDataIndex[0].toLong() - 1],
                                     textureCoordinates[vertexDataIndex[1].toLong() - 1],
                                                normals[vertexDataIndex[2].toLong() - 1]);

                modelData.m_vertexesData.append(newVertex);
            }
        }
    }

    objFile.close();

    return modelData;
}

MeshData ObjDataProcessor::triangulate(const MeshData &originalData)
{

}
