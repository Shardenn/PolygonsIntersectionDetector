#include "glgrid.h"

GLObject::GLGrid::GLGrid(float gridSize,
                         QColor color)
{
    m_gridSize = gridSize;
    m_color = color;
}

void GLObject::GLGrid::draw(QOpenGLShaderProgram *shaderProgram,
                            QOpenGLFunctions *functions)
{
    glBegin(GL_LINES);
    glColor3f(m_color.red(), m_color.green(), m_color.blue());
    int halfGridSize = m_gridSize / 0.5f;
    for(int i = -halfGridSize; i <= halfGridSize; i++) {
        float fi = static_cast<float>(i);
        float fgs = static_cast<float>(halfGridSize);

        glVertex3f(fi, 0, -fgs);
        glVertex3f(fi, 0, fgs);

        glVertex3f(-fgs, 0, fi);
        glVertex3f(fgs,0,fi);
    }
    glEnd();
}
