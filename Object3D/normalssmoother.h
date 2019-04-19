#ifndef NORMALSSMOOTHER_H
#define NORMALSSMOOTHER_H

#include <QVector>
#include <QVector3D>

class NormalsSmoother
{
public:
    NormalsSmoother();

    /*!
     * \brief calculates smoothed normals for vertices
     * \param positions - vertices, triangulated
     * \return smoothed normals
     */
    QVector<QVector3D> smooth(const QVector<QVector3D>& positions);
};

#endif // NORMALSSMOOTHER_H
