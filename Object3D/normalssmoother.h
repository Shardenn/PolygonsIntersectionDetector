#ifndef NORMALSSMOOTHER_H
#define NORMALSSMOOTHER_H

#include <QVector>
#include <QVector3D>

namespace NormalsSmoother
{
    /*!
     * \brief calculates smoothed normals for vertices
     * assuming that the vertices are from
     * triangulated mesh
     * \param positions - vertices, triangulated
     * \return smoothed normals
     */
    QVector<QVector3D> smooth(const QVector<QVector3D>& positions,
                              const QVector<int>& triangulatedIndices);
};

#endif // NORMALSSMOOTHER_H
