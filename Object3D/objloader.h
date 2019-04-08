#ifndef OBJDLOADER_H
#define OBJDLOADER_H

#include <GL/gl.h>
#include <QString>
#include <QVector>
#include "model3d.h"

using namespace Model3D;

namespace OBJLoader {

namespace OBJLoader
{
    /*!
     * \brief Loads information about the model from *.obj file.
     * The data is returned as-is, without triangulation.
     * \param filePath path to *.obj file
     * \return vector of data for each vertex from obj file
    */
    MeshData *load(const QString &filePath);
    // argument not const because readLine() is not const
    MeshData *load(QTextStream &textStream);
    /*!
     * \brief When given model data, returns its triangulated copy.
     * \param originalData - original model data
     * \return triangulated data
     */
    MeshData triangulate(const MeshData &originalData);

    bool getFaceData(QVector<unsigned int> vertices,
                     QVector<unsigned int> textures,
                     QVector<unsigned int> normals);

    QVector<QVector3D> getPolygonInformation(QStringList& polygonInfoLine);

    QVector<float> parseCoordsLine(const QString& line);
};

}
#endif // QOBJLOADER_H
