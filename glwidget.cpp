#include "glwidget.h"
#include <QDir>
#include <QMouseEvent>
#include <QTimer>
#include <objdataprocessor.h>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include "model3d.h"

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

}

QSize GLWidget::sizeHint() const
{
    return QSize(640, 480);
}

void GLWidget::initializeGL()
{
    glClearColor(128, 128, 0, 1);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    initShaders();
    initCube();
}

void GLWidget::resizeGL(int w, int h)
{
    if(h == 0) h = 1;
    float aspect = w / float(h);

    m_projectoinMatrix.setToIdentity();
    m_projectoinMatrix.perspective(60.0, aspect, 0.1f, 10.f);

    //glViewport(0, 0, w, h);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 vMatrix;
    vMatrix.setToIdentity();
    vMatrix.translate(0.0,0.0,-5.0);
    //vMatrix.rotate(m_rotation);

    m_shaderProgram.bind();
    m_shaderProgram.setUniformValue("u_projectionMatrix", m_projectoinMatrix);
    m_shaderProgram.setUniformValue("u_viewMatrix", vMatrix);
    m_shaderProgram.setUniformValue("u_lightPosition", QVector4D(0.0, 0.0, 0.0, 1.0));
    m_shaderProgram.setUniformValue("u_lightPower", 5.0f);

    for (auto object : m_objects) {
        object->draw(&m_shaderProgram, context()->currentContext()->functions());
    }
}

void GLWidget::initShaders()
{
    if (!m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                                 QDir::currentPath() + "/../PolygonsIntersectionDetector/Shaders/lightingVertexShader.vsh"))
        close();

    if (!m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                                 QDir::currentPath() + "/../PolygonsIntersectionDetector/Shaders/lightingFragmentShader.fsh"))
        close();

    if (!m_shaderProgram.link())
        close();
}

void GLWidget::initCube()
{
    auto cubeData = ObjDataProcessor::getModelData(QDir::currentPath() + "/../PolygonsIntersectionDetector/Objects/cube.obj");
    auto texture = QImage(QDir::currentPath() + "/../PolygonsIntersectionDetector/Textures/QtCreator.png");
    if (texture.isNull()) {
        qDebug() << "Texture was not opened in initCube()";
        return;
    }
    Model3D cube(cubeData.m_vertexesData, cubeData.m_polygonVertIndexes, texture);
    m_objects.append(&cube);

    /*
    QVector<VertexData> vertexes;
    vertexes.append(VertexData(QVector3D(-1, 1, 1), QVector2D(0.0, 1.0), QVector3D(1.0, 1.0, 1.0)));
    vertexes.append(VertexData(QVector3D(-1, -1, 1), QVector2D(0.0, 0.0), QVector3D(1.0, 1.0, 1.0)));
    vertexes.append(VertexData(QVector3D(1, -1, 1), QVector2D(1.0, 1.0), QVector3D(1.0, 1.0, 1.0)));
    vertexes.append(VertexData(QVector3D(-1, -1, 1), QVector2D(1.0, 0.0), QVector3D(1.0, 1.0, 1.0)));

    QVector<GLuint> indexes;

    indexes.append(0);
    indexes.append(1);
    indexes.append(2);
    indexes.append(1);
    indexes.append(2);
    indexes.append(3);
    */
    //m_objects.append(new Model3D(vertexes, indexes, texture));

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
