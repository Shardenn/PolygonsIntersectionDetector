#ifndef VERTEXDATA_H
#define VERTEXDATA_H

#include <QVector3D>
#include <QVector2D>

struct QVertexData
{
    QVector3D m_position;
    QVector2D m_textureCoordinate;
    QVector3D m_normal;

    QVertexData(){}

    QVertexData(QVector3D position, QVector2D textureCoord, QVector3D normal) :
        m_position(position),
        m_textureCoordinate(textureCoord),
        m_normal(normal)
    {
    }
};

#endif // VERTEXDATA_H
