#include "objloader.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "vertexdata.h"

QObjLoader::QObjLoader(const QString &filePath) :
    m_filePath(filePath)
{

}

void QObjLoader::LoadModel(const QString &filePath)
{
    m_filePath = filePath;
    LoadModel();
}

void QObjLoader::LoadModel()
{
    QFile objFile(m_filePath);
    if(!objFile.exists()) {
        qDebug() << "File for loading obj not found";
        return;
    }

    objFile.open(QIODevice::ReadOnly);

    QTextStream objFileText(&objFile);

    while(!objFileText.atEnd()) {
        QString str = objFileText.readLine();
        QStringList lineTokens = str.split(" ");

//        switch(lineTokens[0]) { // checking what type is the line describing
//        case "#":
//            qDebug() << "Comment in obj file: " << str;
//            continue;
//            break;
//        case "mtllib":
//            // process the provided material
//            qDebug() << "Obj file has the material assigned:" << str;
//            continue;
//            break;
//        case "v":
//            m_verticesCoord.append(QVector3D(lineTokens[1].toFloat(),
//                                        lineTokens[2].toFloat(),
//                                        lineTokens[3].toFloat()));
//            continue;
//            break;
//        case "vt":
//            m_textureCoordinates.append(QVector2D(lineTokens[1].toFloat(),
//                                                  lineTokens[2].toFloat()));
//            continue;
//            break;
//        case "vn":
//            m_normals.append(QVector3D(lineTokens[1].toFloat(),
//                                       lineTokens[2].toFloat(),
//                                       lineTokens[3].toFloat()));
//            continue;
//            break;
//        case "f":
//            // in obj file, indeces are going from 1, they are not 0-based
//            // 1 index - index of vertices array
//            // 2 index - index of texture coordinates array
//            // 3 index - index of normals array
//            // assuming that the model is triangulate
//            // TODO : triangulate, but store the original topology
//            for(int i = 1; i < lineTokens.size(); i++) {
//                QStringList vertexData = lineTokens[i].split("/");
//                m_vertices.append(QVertexData(     m_verticesCoord[vertexData[0].toLong() - 1],
//                                              m_textureCoordinates[vertexData[1].toLong() - 1],
//                                                         m_normals[vertexData[2].toLong() - 1]));
//                m_indeces.append(m_indeces.size());
//            }
//            continue;
//            break;
//        }
    }

    objFile.close();
}
