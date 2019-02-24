#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QGLShaderProgram>

class GLWidget : public QGLWidget
{

    Q_OBJECT

public:
    GLWidget(QWidget *parent = nullptr);
    ~GLWidget() override;

    QSize sizeHint() const override;

protected:

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    // input handling on widget press
    void mousePressEvent(QMouseEvent*event) override;
    void mouseMoveEvent(QMouseEvent*event) override;
    void wheelEvent(QWheelEvent*event) override;

private:
    QMatrix4x4 m_projectoin_matrix;
    QGLShaderProgram m_shader_program;

    QVector<QVector3D> m_vertices;
    QVector<QVector2D> m_texture_coordinates;

    GLuint texture;

    // mouse input stuff
    float m_alpha;
    float m_beta;
    float m_distance;
    QPoint m_last_mouse_position;
};

#endif // GLWIDGET_H
