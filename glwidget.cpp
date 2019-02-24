#include "glwidget.h"
#include <QDir>
#include <QMouseEvent>

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(), parent)
{
    m_alpha = 25;
    m_beta = -25;
    m_distance = 2.5;
}

GLWidget::~GLWidget()
{

}

QSize GLWidget::sizeHint() const
{
    return QSize(640, 480);
}

void GLWidget::initializeGL()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    qglClearColor(QColor(128, 128, 128));

    auto path = QDir::currentPath() + "/../Object_intersections/";

    m_shader_program.addShaderFromSourceFile(QGLShader::Vertex, path + "/Shaders/vertexShader.vsh");
    m_shader_program.addShaderFromSourceFile(QGLShader::Fragment, path + "/Shaders/fragmentShader.fsh");
    m_shader_program.link();

    // vertices initiatlized with the piramyde
    m_vertices.push_back(QVector3D(0,0,0));
    m_vertices.push_back(QVector3D(0.5,1,0));
    m_vertices.push_back(QVector3D(1,0,0));

    m_vertices.push_back(QVector3D(0,0,0));
    m_vertices.push_back(QVector3D(0.5,0.5,1));
    m_vertices.push_back(QVector3D(0.5,1,0));

    m_vertices.push_back(QVector3D(0,0,0));
    m_vertices.push_back(QVector3D(1,0,0));
    m_vertices.push_back(QVector3D(0.5,0.5,1));

    m_vertices.push_back(QVector3D(1,0,0));
    m_vertices.push_back(QVector3D(0.5,1,0));
    m_vertices.push_back(QVector3D(0.5,0.5,1));

    // texture coordinates
    m_texture_coordinates.push_back(QVector2D(0,0));
    m_texture_coordinates.push_back(QVector2D(0.5,1));
    m_texture_coordinates.push_back(QVector2D(1,0));

    m_texture_coordinates.push_back(QVector2D(0,0));
    m_texture_coordinates.push_back(QVector2D(0.5,0.5));
    m_texture_coordinates.push_back(QVector2D(0.5,1));

    m_texture_coordinates.push_back(QVector2D(0,0));
    m_texture_coordinates.push_back(QVector2D(1,0));
    m_texture_coordinates.push_back(QVector2D(0.5,0.5));

    m_texture_coordinates.push_back(QVector2D(1,0));
    m_texture_coordinates.push_back(QVector2D(0.5,1));
    m_texture_coordinates.push_back(QVector2D(0.5,0.5));

    texture = bindTexture(QPixmap(path + "/Textures/QtCreator.png"));
}

void GLWidget::resizeGL(int w, int h)
{
    if(h == 0) h = 1;

    m_projectoin_matrix.setToIdentity();
    m_projectoin_matrix.perspective(60.0, float(w) / float(h), 0.1f, 1000);

    glViewport(0, 0, w, h);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    QMatrix4x4 mMatrix;
    QMatrix4x4 vMatrix;

    QMatrix4x4 cameraTransformation;
    cameraTransformation.rotate(m_alpha, 0, 1, 0);
    cameraTransformation.rotate(m_beta, 1, 0, 0);
    QVector3D cameraPosition = cameraTransformation * QVector3D(0, 0, m_distance);
    QVector3D cameraUpDirection = cameraTransformation * QVector3D(0, 1, 0);
    vMatrix.lookAt(cameraPosition, QVector3D(0, 0, 0), cameraUpDirection);

    m_shader_program.bind();

    m_shader_program.setUniformValue("mvpMatrix", m_projectoin_matrix * vMatrix * mMatrix);
    m_shader_program.setUniformValue("texture", 0);

    glBindTexture(GL_TEXTURE_2D, texture);

    m_shader_program.setAttributeArray("vertex", m_vertices.constData());
    m_shader_program.enableAttributeArray("vertex");

    m_shader_program.setAttributeArray("textureCoordinate", m_texture_coordinates.constData());
    m_shader_program.enableAttributeArray("textureCoordinate");

    glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());

    m_shader_program.disableAttributeArray("vertex");
    m_shader_program.disableAttributeArray("textureCoordinates");

    m_shader_program.release();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    m_last_mouse_position = event->pos();
    event->accept();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int deltaX = event->x() - m_last_mouse_position.x();
    int deltaY = event->y() - m_last_mouse_position.y();

    if(event->buttons() & Qt::LeftButton)
    {
        m_alpha -= deltaX;
        while(m_alpha < 0)
            m_alpha += 360;
        while(m_alpha >= 360)
            m_alpha -= 360;

        m_beta -= deltaY;
        while(m_beta < -90)
            m_beta = -90;
        while(m_beta > 90)
            m_beta = 90;

        updateGL();
    }
    m_last_mouse_position = event->pos();
    event->accept();
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    int delta = event->delta();

    if(event->orientation() == Qt::Vertical)
    {
        if(delta < 0)
            m_distance *= 1.1f;
        if(delta > 0)
            m_distance *= 0.9f;

        updateGL();
    }
    event->accept();
}
