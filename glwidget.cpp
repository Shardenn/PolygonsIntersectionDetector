#include "glwidget.h"
#include <QDir>
#include <QMouseEvent>
#include <QTimer>
#include <objdataprocessor.h>

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(), parent)
{
    m_alpha = 25;
    m_beta = -25;
    m_distance = 2.5;

    m_light_angle = 0;

    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
    timer->start(20);
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

//    auto path = QDir::currentPath() + "/../Object_intersections/";
    QString path = ":";

    m_lighting_shader_program.addShaderFromSourceFile(QGLShader::Vertex, path + "/Shaders/lightingVertexShader.vsh");
    m_lighting_shader_program.addShaderFromSourceFile(QGLShader::Fragment, path + "/Shaders/lightingFragmentShader.fsh");
    m_lighting_shader_program.link();

    // vertices initiatlized with the piramyde
    // rear (from starting perspective)
    m_pyramide_vertices.push_back(QVector3D(0,0,0));    // 0
    m_pyramide_vertices.push_back(QVector3D(0.5,1,0));  // 1
    m_pyramide_vertices.push_back(QVector3D(1,0,0));    // 2
    // left (from starting perspective)
    m_pyramide_vertices.push_back(QVector3D(0,0,0));    // 3
    m_pyramide_vertices.push_back(QVector3D(0.5,0.5,1));// 4
    m_pyramide_vertices.push_back(QVector3D(0.5,1,0));  // 5
    // bottom (from starting perspective)
    m_pyramide_vertices.push_back(QVector3D(0,0,0));    // 6
    m_pyramide_vertices.push_back(QVector3D(1,0,0));    // 7
    m_pyramide_vertices.push_back(QVector3D(0.5,0.5,1));// 8
    // right (from starting perspective)
    m_pyramide_vertices.push_back(QVector3D(1,0,0));    // 9
    m_pyramide_vertices.push_back(QVector3D(0.5,1,0));  // 10
    m_pyramide_vertices.push_back(QVector3D(0.5,0.5,1));// 11

    // pyramide normals
    // for a triangle, its normal equals cross product of two its edges.
    // Edge vector = point1 - point2
    auto curr_normal = QVector3D::crossProduct((m_pyramide_vertices[1] - m_pyramide_vertices[0]),
            (m_pyramide_vertices[1] - m_pyramide_vertices[0]));
    m_pyramide_normals.push_back(curr_normal);
    m_pyramide_normals.push_back(curr_normal);
    m_pyramide_normals.push_back(curr_normal);

    curr_normal = QVector3D::crossProduct((m_pyramide_vertices[4] - m_pyramide_vertices[3]),
            (m_pyramide_vertices[5] - m_pyramide_vertices[4]));
    m_pyramide_normals.push_back(curr_normal);
    m_pyramide_normals.push_back(curr_normal);
    m_pyramide_normals.push_back(curr_normal);

    curr_normal = QVector3D::crossProduct((m_pyramide_vertices[7] - m_pyramide_vertices[6]),
            (m_pyramide_vertices[8] - m_pyramide_vertices[7]));
    m_pyramide_normals.push_back(curr_normal);
    m_pyramide_normals.push_back(curr_normal);
    m_pyramide_normals.push_back(curr_normal);

    curr_normal = QVector3D::crossProduct((m_pyramide_vertices[10] - m_pyramide_vertices[9]),
            (m_pyramide_vertices[11] - m_pyramide_vertices[10]));
    m_pyramide_normals.push_back(curr_normal);
    m_pyramide_normals.push_back(curr_normal);
    m_pyramide_normals.push_back(curr_normal);

    // texture coordinates
    m_pyramide_texture_coordinates.push_back(QVector2D(0,0));
    m_pyramide_texture_coordinates.push_back(QVector2D(1,0));
    m_pyramide_texture_coordinates.push_back(QVector2D(1,1));

    m_pyramide_texture_coordinates.push_back(QVector2D(0,0));
    m_pyramide_texture_coordinates.push_back(QVector2D(0.5,0.5));
    m_pyramide_texture_coordinates.push_back(QVector2D(0.5,1));

    m_pyramide_texture_coordinates.push_back(QVector2D(0,0));
    m_pyramide_texture_coordinates.push_back(QVector2D(1,0));
    m_pyramide_texture_coordinates.push_back(QVector2D(0.5,0.5));

    m_pyramide_texture_coordinates.push_back(QVector2D(1,0));
    m_pyramide_texture_coordinates.push_back(QVector2D(0.5,1));
    m_pyramide_texture_coordinates.push_back(QVector2D(0.5,0.5));

    m_pyramide_texture = bindTexture(QPixmap("../../PolygonsIntersectionDetector/Textures/QtCreator.png"));
//    m_pyramide_texture = bindTexture(QPixmap(path + "/Textures/QtCreator.png"));

    int num_pyramide_vertices = m_pyramide_vertices.size();

    m_pyramide_buffer.create();
    m_pyramide_buffer.bind();
    m_pyramide_buffer.allocate(num_pyramide_vertices * (3 + 3 + 2) * sizeof(GLfloat));

    int offset = 0;
    m_pyramide_buffer.write(offset, m_pyramide_vertices.constData(), num_pyramide_vertices * 3 * sizeof(GLfloat));
    offset += num_pyramide_vertices * 3 * sizeof(GLfloat);
    m_pyramide_buffer.write(offset, m_pyramide_normals.constData(), num_pyramide_vertices * 3 * sizeof(GLfloat));
    offset += num_pyramide_vertices * 3 * sizeof(GLfloat);
    m_pyramide_buffer.write(offset, m_pyramide_texture_coordinates.constData(), num_pyramide_vertices * 2 * sizeof(GLfloat));

    m_pyramide_buffer.release();
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

    mMatrix.setToIdentity();

    QMatrix4x4 mvMatrix;
    mvMatrix = mMatrix * vMatrix;

    QMatrix3x3 normalMatrix;
    normalMatrix = mvMatrix.normalMatrix();

    QMatrix4x4 lightTransformation;
    lightTransformation.rotate(m_light_angle, 0, 1, 0);

    QVector3D lightPosition = lightTransformation * QVector3D(0, 1, 1);

    m_lighting_shader_program.bind();
    m_lighting_shader_program.setUniformValue("mvpMatrix", m_projectoin_matrix * mvMatrix);
    m_lighting_shader_program.setUniformValue("mvMatrix", mvMatrix);
    m_lighting_shader_program.setUniformValue("normalMatrix", normalMatrix);
    m_lighting_shader_program.setUniformValue("lightPosition", vMatrix*lightPosition);

    m_lighting_shader_program.setUniformValue("ambientColor", QColor(32, 32, 32));
    m_lighting_shader_program.setUniformValue("diffuseColor", QColor(128, 128, 128));
    m_lighting_shader_program.setUniformValue("specularColor", QColor(255, 255, 255));
    m_lighting_shader_program.setUniformValue("ambientReflection", GLfloat(1.0));
    m_lighting_shader_program.setUniformValue("diffuseReflection", GLfloat(1.0));
    m_lighting_shader_program.setUniformValue("specularReflection", GLfloat(1.0));
    m_lighting_shader_program.setUniformValue("shininess", GLfloat(100.0));
    m_lighting_shader_program.setUniformValue("texture", 0);

    glBindTexture(GL_TEXTURE_2D, m_pyramide_texture);

    m_pyramide_buffer.bind();
    int offset = 0;
    int num_pyramide_vertices = m_pyramide_vertices.size();

    m_lighting_shader_program.setAttributeBuffer("vertex", GL_FLOAT, offset, 3, 0);
    m_lighting_shader_program.enableAttributeArray("vertex");
    offset += num_pyramide_vertices * 3 * sizeof(GLfloat);

    m_lighting_shader_program.setAttributeBuffer("normal", GL_FLOAT, offset, 3, 0);
    m_lighting_shader_program.enableAttributeArray("normal");
    offset += num_pyramide_vertices * 3 * sizeof(GLfloat);

    m_lighting_shader_program.setAttributeBuffer("textureCoordinate", GL_FLOAT, offset, 2, 0);
    m_lighting_shader_program.enableAttributeArray("textureCoordinate");
    m_pyramide_buffer.release();

    glDrawArrays(GL_TRIANGLES, 0, m_pyramide_vertices.size());
/*
    m_lighting_shader_program.disableAttributeArray("vertex");
    m_lighting_shader_program.disableAttributeArray("normal");
    m_lighting_shader_program.disableAttributeArray("textureCoordinate");
*/
    m_lighting_shader_program.release();

    mMatrix.setToIdentity();
    mMatrix.translate(lightPosition);
    mMatrix.rotate(m_light_angle, 0, 1, 0);
    mMatrix.rotate(45, 1, 0, 0);
    mMatrix.scale(0.1);
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

void GLWidget::timeout()
{
    m_light_angle += 1;
    while(m_light_angle >= 360)
    {
        m_light_angle -= 360;
    }
    updateGL();
}
