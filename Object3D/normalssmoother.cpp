#include "normalssmoother.h"

QVector<QVector3D> NormalsSmoother::smooth(const QVector<QVector3D>& positions,
                                           const QVector<int>& indices)
{
    if (positions.size() < 1) {
        return QVector<QVector3D>();
    }

    QVector<QVector3D> normals(positions.size());

    for (int i = 0; i < indices.size() - 2; i += 3) {
        int positionInd_0 = indices[i],
                positionInd_1 = indices[i + 1],
                positionInd_2 = indices[i + 2];

        QVector3D polygonFirstVector = positions[positionInd_1] -
                positions[positionInd_0];
        QVector3D polygonSecondVector = positions[positionInd_2] -
                positions[positionInd_0];

        QVector3D polygonNormal = QVector3D::crossProduct(polygonFirstVector,
                                                          polygonSecondVector);
        polygonNormal.normalize();

        normals[positionInd_0] += polygonNormal;
        normals[positionInd_1] += polygonNormal;
        normals[positionInd_2] += polygonNormal;
    }

    for (QVector3D &normal : normals) {
        normal.normalize();
    }

    return normals;
}
