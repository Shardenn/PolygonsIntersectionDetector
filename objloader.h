#ifndef OBJDLOADER_H
#define OBJDLOADER_H

#include <GL/gl.h>
#include <QString>
#include <QVector>
#include "model3d.h"

using namespace Model3D;

class ObjLoader
{
public:
    /*!
     * \brief Loads information about the model from *.obj file.
     * The data is returned as-is, without triangulation.
     * \param filePath path to *.obj file
     * \return vector of data for each vertex from obj file
    */
    static MeshData load(const QString &filePath);
    /*!
     * \brief When given model data, returns its triangulated copy.
     * \param originalData - original model data
     * \return triangulated data
     */
    static MeshData triangulate(const MeshData &originalData);
private:
    bool getFaceData(QVector<unsigned int> vertices,
                     QVector<unsigned int> textures,
                     QVector<unsigned int> normals);

    static QVector<QVector3D> getPolygonInformation(QStringList& polygonInfoLine);
};

#endif // QOBJLOADER_H
