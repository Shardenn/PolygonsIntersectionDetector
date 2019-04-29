#ifndef ICP_H
#define ICP_H

#include <QMap>
#include <QVector>
#include <QVector3D>
#include <Eigen/Dense>
#include <QMatrix4x4>

class ICP
{
public:
    ICP();

    QMap<int, int> findCorrespondence(const QVector<QVector3D>& baseShape,
                                      const QVector<QVector3D>& targetShape);

    void performRigidAlignment(QVector<QVector3D>& baseShape,
                               QVector<QVector3D>& targetShape,
                               const QMatrix4x4& baseModelMat,
                               const QMatrix4x4& targetModelMat,
                               const float& error = 0.1f);

private:
    void rigidAlignmentIteration(QVector<QVector3D>& baseShape,
                                 QVector<QVector3D>& targetShape,
                                 const QMap<int, int>& correspondence);

    Eigen::Vector3f getPointCloudMean(const QVector<QVector3D>& points);

    Eigen::Matrix3f getCorrelationMatrix(const QVector<QVector3D>& a,
                                         const QVector<QVector3D>& b);

    Eigen::Matrix3f getRotation(const QVector<QVector3D>& baseShape,
                                const QVector<QVector3D>& targetShape);

    QVector<QVector3D> moveByValue(const QVector<QVector3D>& points,
                                   const Eigen::Vector3f& value);
};

#endif // ICP_H
