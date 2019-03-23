#include "glwidget.h"
#include <QDir>
#include <QMouseEvent>
#include <QTimer>
#include <objdataprocessor.h>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include "model3d.h"

#include <QDebug>

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    m_alpha = 25;
    m_beta = -25;
    m_distance = 2.5;

//    m_light_angle = 0;

    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
    timer->start(20);
}

GLWidget::~GLWidget()
{
    for (auto object : m_objects) {
        delete object;
    }
}

QSize GLWidget::sizeHint() const
{
    return QSize(640, 480);
}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    qDebug() << "initializeGL";
    glClearColor(0, 0.3, 0, 1);

    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);

    initShaders();
    initCube();
    qDebug() << "initializeGL Done";
}

void GLWidget::resizeGL(int w, int h)
{
    if(h == 0) h = 1;
    float aspect = w / float(h);

    m_projectoinMatrix.setToIdentity();
    m_projectoinMatrix.perspective(60.0, aspect, 0.1f, 10.f);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 vMatrix;
    vMatrix.setToIdentity();
    vMatrix.translate(0.0,0.0,-5.0); // move camera a bit further from origin

    m_shaderProgram.bind();

    m_shaderProgram.setUniformValue("u_projectionMatrix", m_projectoinMatrix);
    m_shaderProgram.setUniformValue("u_viewMatrix", vMatrix);
    m_shaderProgram.setUniformValue("u_lightPosition", QVector4D(0.0, 0.0, 0.0, 1.0));
    m_shaderProgram.setUniformValue("u_lightPower", 5.0f);

    for (auto object : m_objects) {
        object->draw(&m_shaderProgram, context()->functions());
    }
}

void GLWidget::initShaders()
{
    if (!m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                                 ":/Shaders/lightingVertexShader.vsh")) {
        qDebug() << "The vertex shader was not compiled";
    }

    if (!m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                                 ":/Shaders/lightingFragmentShader.fsh")) {
        qDebug() << "The fragment shader was not compiled";
    }

    if (!m_shaderProgram.link()) {
        qDebug() << "The shader program was not linked in InitShaders()";
    }

    if(!m_shaderProgram.bind()) {
        qDebug() << "The shader program was not binded in InitShaders()";
    }
}

void GLWidget::initCube()
{
    QVector<VertexData> vertexData;

    float cubeLength = 1.0;
    float l = cubeLength / 2;

    vertexData.append(VertexData(QVector3D(-l, l, l),   QVector2D(0.0, 1.0),   QVector3D(0.0, 0.0, 1.0))); // normal is up
    vertexData.append(VertexData(QVector3D(-l, -l, l),  QVector2D(0.0, 0.0),   QVector3D(0.0, 0.0, 1.0)));
    vertexData.append(VertexData(QVector3D(l, l, l),    QVector2D(1.0, 1.0),   QVector3D(0.0, 0.0, 1.0)));
    vertexData.append(VertexData(QVector3D(l, -l, l),   QVector2D(1.0, 0.0),   QVector3D(0.0, 0.0, 1.0)));

    vertexData.append(VertexData(QVector3D(l, l, l),     QVector2D(0.0, 1.0),   QVector3D(1.0, 0.0, 0.0)));
    vertexData.append(VertexData(QVector3D(l, -l, l),    QVector2D(0.0, 0.0),   QVector3D(1.0, 0.0, 0.0)));
    vertexData.append(VertexData(QVector3D(l, l, -l),    QVector2D(1.0, 1.0),   QVector3D(1.0, 0.0, 0.0)));
    vertexData.append(VertexData(QVector3D(l, -l, -l),   QVector2D(1.0, 0.0),   QVector3D(1.0, 0.0, 0.0)));

    vertexData.append(VertexData(QVector3D(l, l, l),     QVector2D(0.0, 1.0),   QVector3D(0.0, 1.0, 0.0)));
    vertexData.append(VertexData(QVector3D(l, l, -l),    QVector2D(0.0, 0.0),   QVector3D(0.0, 1.0, 0.0)));
    vertexData.append(VertexData(QVector3D(-l, l, l),    QVector2D(1.0, 1.0),   QVector3D(0.0, 1.0, 0.0)));
    vertexData.append(VertexData(QVector3D(-l, l, -l),   QVector2D(1.0, 0.0),   QVector3D(0.0, 1.0, 0.0)));

    vertexData.append(VertexData(QVector3D(l, l, -l),   QVector2D(0.0, 1.0),   QVector3D(0.0, 0.0, -1.0)));
    vertexData.append(VertexData(QVector3D(l, -l, -l),  QVector2D(0.0, 0.0),   QVector3D(0.0, 0.0, -1.0)));
    vertexData.append(VertexData(QVector3D(-l, l, -l),    QVector2D(1.0, 1.0),   QVector3D(0.0, 0.0, -1.0)));
    vertexData.append(VertexData(QVector3D(-l, -l, -l),   QVector2D(1.0, 0.0),   QVector3D(0.0, 0.0, -1.0)));

    vertexData.append(VertexData(QVector3D(-l, l, l),     QVector2D(0.0, 1.0),   QVector3D(-1.0, 0.0, 0.0)));
    vertexData.append(VertexData(QVector3D(-l, l, -l),    QVector2D(0.0, 0.0),   QVector3D(-1.0, 0.0, 0.0)));
    vertexData.append(VertexData(QVector3D(-l, -l, l),    QVector2D(1.0, 1.0),   QVector3D(-1.0, 0.0, 0.0)));
    vertexData.append(VertexData(QVector3D(-l, -l, -l),   QVector2D(1.0, 0.0),   QVector3D(-1.0, 0.0, 0.0)));

    vertexData.append(VertexData(QVector3D(-l, -l, l),     QVector2D(0.0, 1.0),   QVector3D(0.0, -1.0, 0.0)));
    vertexData.append(VertexData(QVector3D(-l, -l, -l),    QVector2D(0.0, 0.0),   QVector3D(0.0, -1.0, 0.0)));
    vertexData.append(VertexData(QVector3D(l, -l, l),    QVector2D(1.0, 1.0),   QVector3D(0.0, -1.0, 0.0)));
    vertexData.append(VertexData(QVector3D(l, -l, -l),   QVector2D(1.0, 0.0),   QVector3D(0.0, -1.0, 0.0)));

    QVector<GLuint> indexesData;

    for (unsigned int i = 0; i < 24; i += 4) {
        indexesData.append(i + 0);
        indexesData.append(i + 1);
        indexesData.append(i + 2);
        indexesData.append(i + 2);
        indexesData.append(i + 1);
        indexesData.append(i + 3);
    }

    using namespace Model3D;
    GLModel3D *cube = new GLModel3D(vertexData, indexesData, QImage(":/Textures/QtCreator.png"));
    m_objects.append(cube);

}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    m_lastMousePosition = event->pos();
    event->accept();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int deltaX = event->x() - m_lastMousePosition.x();
    int deltaY = event->y() - m_lastMousePosition.y();

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

        //paintGL();
    }
    m_lastMousePosition = event->pos();
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

        //paintGL();
    }
    event->accept();
}

void GLWidget::timeout()
{
    /*
    m_light_angle += 1;
    while(m_light_angle >= 360)
    {
        m_light_angle -= 360;
    }*/
    //paintGL();
}
