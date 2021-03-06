#ifndef OBJDLOADER_H
#define OBJDLOADER_H

#include <GL/gl.h>
#include <QString>
#include <QVector>
#include "Object3D/model3d.h"

using namespace Model3D;

namespace OBJLoader {

// TODO
// triangulation
// adding default normals if they are not provided
class OBJLoader
{
public:
    /*!
     * \brief Loads information about the model from *.obj file.
     * The data is returned as-is, without triangulation.
     * \param filePath path to *.obj file
     * \return vector of data for each vertex from obj file
    */
    MeshData *load(const QString &filePath,
                   bool smoothNormals = true, bool triangulte = true);
    // argument not const because readLine() is not const
    MeshData *load(QTextStream &textStream,
                   bool smoothNormals = true, bool triangulate = true);

private:
    QVector<QVector3D> getPolygonInformation(QStringList& polygonInfoLine);

    QVector<float> parseLine(const QString& line);

    bool m_isLoadSuccessful{ true };
};

}
#endif // QOBJLOADER_H
