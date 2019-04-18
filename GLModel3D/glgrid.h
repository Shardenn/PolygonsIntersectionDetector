#ifndef GLGRID_H
#define GLGRID_H

#include "gltransformable.h"
#include <QColor>

namespace GLObject {

class GLGrid : public GLDrawable
{
public:
    GLGrid(float gridSize = 1.f,
           QColor color = QColor{ 128, 128, 128});

    void draw(QOpenGLShaderProgram *shaderProgram) override;

protected:
    float m_gridSize{ 20.f };
    QColor m_color{ 128, 128, 128 };
};

}
#endif // GLGRID_H
