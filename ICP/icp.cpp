#include "icp.h"
#include <limits>
#include <QDebug>

ICP::ICP()
{

}

QMap<int, int> ICP::findCorrespondence(const QVector<QVector3D> &baseShape,
                                       const QVector<QVector3D> &targetShape)
{
    QMap<int, int> correspondence;

    // error for each point that we need to find
    // correspondence with
    QVector<double> distances(baseShape.count(),
                           std::numeric_limits<double>::max());

    for (int i = 0; i < baseShape.size(); ++i) {
        for (int j = 0; j < targetShape.size(); ++j) {
            double distance = (baseShape[i] - targetShape[j]).lengthSquared();
            if (distance < distances[i]) {
                distances[i] = distance;
                if (correspondence.find(i) != correspondence.end())
                    correspondence.remove(i);
                correspondence.insert(i, j);
            }
        }
    }

    return correspondence;
}

void ICP::performRigidAlignment(QVector<QVector3D> &baseShape,
                                QVector<QVector3D> &targetShape,
                                const QMatrix4x4& baseModelMat,
                                const QMatrix4x4& targetModelMat,
                                const float &error)
{
    for (auto &point : baseShape) {
        point = baseModelMat * point;
    }

    for (auto &point : targetShape) {
        point = targetModelMat * point;
    }

    for (int i = 0; i < 5; ++i) {
        auto corresp = findCorrespondence(baseShape, targetShape);
        rigidAlignmentIteration(baseShape, targetShape, corresp);
    }
}

void ICP::rigidAlignmentIteration(QVector<QVector3D> &baseShape,
                                  QVector<QVector3D> &targetShape,
                                  const QMap<int, int> &correspondence)
{
    QVector<QVector3D> baseRearranged(baseShape.size());
    QVector<QVector3D> targetRearranged(targetShape.size());

    int i = 0;
    for (auto corr = correspondence.begin();
         corr != correspondence.end(); ++corr, ++i) {
        baseRearranged[i] = baseShape[corr.key()];
        targetRearranged[i] = targetShape[corr.value()];
    }

    auto baseMean = getPointCloudMean(baseShape);
    auto targetMean = getPointCloudMean(targetShape);

    auto baseNormalized = moveByValue(baseShape, -baseMean);
    auto targetNormalized = moveByValue(targetShape, -targetMean);

    Eigen::Matrix3f rotation = getRotation(baseNormalized,
                                           targetNormalized);


    for (int i = 0; i <  baseShape.size(); ++i) {
        auto point = baseShape[i];

        Eigen::Vector3f p{ point.x(), point.y(), point.z() };
        Eigen::Vector3f transformed =
                rotation * (p - baseMean) + targetMean;
        QVector3D moved { transformed.x(), transformed.y(), transformed.z() };

        baseShape[i] = moved;
    }
}

Eigen::Matrix3f ICP::getRotation(const QVector<QVector3D> &baseShape,
                                 const QVector<QVector3D> &targetShape)
{
    using namespace Eigen;

    Matrix3f corrMatrix = getCorrelationMatrix(baseShape,
                                                      targetShape);

    JacobiSVD<Matrix3f> svd(corrMatrix, ComputeFullU | ComputeFullV);

    return (svd.matrixV()*(svd.matrixU().transpose())).transpose();
}

QVector<QVector3D> ICP::moveByValue(const QVector<QVector3D> &points,
                                    const Eigen::Vector3f &value)
{
    QVector<QVector3D> res(points.size());

    QVector3D offset { value.x(), value.y(), value.z() };
    for (int i = 0; i < points.size(); ++i) {
        res[i] = points[i] + offset;
    }
    return res;
}

Eigen::Vector3f ICP::getPointCloudMean(const QVector<QVector3D> &points)
{
    QVector3D mean;
    for (auto point : points) {
        mean += point;
    }

    mean /= points.size();
    Eigen::Vector3f meanEigen { mean.x(), mean.y(), mean.z() };
    return meanEigen;
}

Eigen::Matrix3f ICP::getCorrelationMatrix(const QVector<QVector3D> &a,
                                          const QVector<QVector3D> &b)
{
    if (a.size() != b.size()) {
        qDebug() << "Sizes of point clouds in getCorrelationMatrix are not the same";
        return Eigen::Matrix3f();
    }

    Eigen::Matrix3f correlation;

    int n = a.size();

    for (int dim_a = 0; dim_a < 3; ++dim_a) {
        for (int dim_b = 0; dim_b < 3; ++dim_b) {
            float sum = 0;
            for (int i = 0; i < n; ++i) {
                sum += a[i][dim_a] * b[i][dim_b];
            }
            correlation(dim_a, dim_b) = sum;
        }
    }

    return correlation;
}
