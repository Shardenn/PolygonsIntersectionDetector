#ifndef TRIANGULATOR_H
#define TRIANGULATOR_H

#include <QVector>
#include <QVector3D>
#include "model3d.h"

using namespace Model3D;

class Triangulator
{    

public:
    Triangulator();
    virtual ~Triangulator();

    virtual void triangulate(MeshData &mesh) = 0;
};

class NaiveTriangulator : public Triangulator
{
public:
    NaiveTriangulator();
    virtual ~NaiveTriangulator() override;

    virtual void triangulate(MeshData &mesh) override;
};

class EarClippingTriangulator : public Triangulator
{
public:
    EarClippingTriangulator();
    virtual ~EarClippingTriangulator() override;

    virtual void triangulate(MeshData &mesh) override;

private:
    /*!
     * \brief earClips the polygon once
     * \param polygon - list of polygon vertices in
     * counter-clockwise order
     * \return list of point !indices! related to the
     * input array.
     * Example: given an array of points coordinates:
     * (1,0,2); (2,0.2,3); ... (3, 7.8, 2.1)
     * output may be
     * (1,3,2); (2,3,4); (1,3,4)
     * where 1,3,2 are indices of the points
     * from the input array
     */
    QVector<QVector3D> earClip(const QVector<QVector3D> &polygon);

    /*!
     * \brief Tells if the given vertex with its
     * adjacant ones is convex. The funcion assumes
     * that points are stores counter-clockwise
     * \param previousVertex - the vertex that goes
     * before the needed one
     * \param vertex - the actual vertex
     * \param nextVertex - the vertex that goes
     * after the needed one
     * \return true if the given vertex is convex
     * if the vertices are stored counter-clockwise
     */
    bool isVertexConvex(const QVector3D previousVertex,
                        const QVector3D vertex,
                        const QVector3D nextVertex);


    /*!
     * \brief isPointInsideTriangle - determines
     * if the given point lies inside of the triangle
     * \param point - the point which position should
     * be determined
     * \param a - triangle vertex
     * \param b - triangle vertex
     * \param c - trianlge vertex
     * \return true if the point is inside the triangle
     */
    bool isPointInsideTriangle(const QVector3D point,
                               const QVector3D a,
                               const QVector3D b,
                               const QVector3D c);
};

#endif // TRIANGULATOR_H
