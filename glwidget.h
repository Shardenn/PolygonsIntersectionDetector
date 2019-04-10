#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QOpenGLFunctions>
//#include <Object3D/model3d.h>
#include "GLModel3D/glmodel3d.h"
class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
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
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

    void initShaders();
    void initCube();
private:
    QMatrix4x4 m_projectoinMatrix;
    QOpenGLShaderProgram m_shaderProgram;
    QVector<GLModel3D::GLModel3D*> m_objects;

    // mouse input stuff
    float m_alpha;
    float m_beta;
    float m_distance;
    QVector2D m_lastMousePosition;
    QQuaternion m_rotation;

private Q_SLOTS:
    void timeout();
};

#endif // GLWIDGET_H
