#ifndef OBJDATAPROCESSOR_H
#define OBJDATAPROCESSOR_H

#include <GL/gl.h>
#include <QString>
#include <QVector>
#include "modeldata.h"

// TODO loadModel should return new object
class ObjDataProcessor
{
public:
    /*!
     * \brief Loads information about the model from *.obj file.
     * The data is returned as-is, without triangulation.
     * \param filePath path to *.obj file
     * \return vector of data for each vertex from obj file
    */
    static MeshData getModelData(const QString &filePath);
    /*!
     * \brief When given model data, returns its triangulated copy.
     * \param originalData - original model data
     * \return triangulated data
     */
    static MeshData triangulate(const MeshData &originalData);
private:

};

#endif // QOBJLOADER_H
