#include "glwidget.h"
#include <QDir>
#include <QMouseEvent>
#include <QTimer>
#include <Object3D/objloader.h>
#include <QOpenGLContext>
#include <QOpenGLFunctions>

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

    glClearColor(0.0f, 0.3f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    initShaders();
    initCube();

    if (glGetError() != 0) {
        qDebug() << "InitializeGL got an error";
        Q_ASSERT(glGetError() == 0);
    }
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
    vMatrix.rotate(m_rotation);

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
        Q_ASSERT(m_shaderProgram.link());
    }

    if(!m_shaderProgram.bind()) {
        qDebug() << "The shader program was not binded in InitShaders()";
    }
}

void GLWidget::initCube()
{
    using namespace Model3D;

    //MeshData mesh(verts, texts, normals, inds);
    OBJLoader::OBJLoader loader;
    MeshData *mesh = loader.load(":/Objects/cube_tr.obj");

    Q_ASSERT(mesh != nullptr);

    GLModel3D *model = new GLModel3D(*mesh, QImage(":/Textures/QtCreator.png"));
    m_objects.append(model);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton) {
        m_lastMousePosition = QVector2D(event->localPos());
    }
    event->accept();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() != Qt::LeftButton)
        return;

    QVector2D diff = QVector2D(event->localPos()) - m_lastMousePosition;
    m_lastMousePosition = QVector2D(event->localPos());

    float angle = diff.length() / 2.0f;

    QVector3D axis(diff.y(), diff.x(), 0.0);

    m_rotation = QQuaternion::fromAxisAndAngle(axis, angle) * m_rotation;

    update();

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
