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
    QGLShaderProgram m_lighting_shader_program;

    QVector<QVector3D> m_pyramide_vertices;
    QVector<QVector3D> m_pyramide_normals;
    QVector<QVector2D> m_pyramide_texture_coordinates;

    GLuint m_pyramide_texture;

    QGLShaderProgram m_colorint_shader_program;

    QVector<QVector3D> m_spotlight_vertices;
    QVector<QVector3D> m_spotlight_colors;
    double m_light_angle;

    // mouse input stuff
    float m_alpha;
    float m_beta;
    float m_distance;
    QPoint m_last_mouse_position;

private Q_SLOTS:
    void timeout();
};

#endif // GLWIDGET_H
