#include "glwidget.h"
#include <QDir>
#include <QMouseEvent>
#include <QTimer>
#include <QApplication>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QtGlobal>
#include <QDebug>
#include <algorithm>

#include "OBJLoader/objloader.h"
#include "Object3D/triangulator.h"
#include "GLModel3D/globject3d.h"
#include "GLModel3D/glgrid.h"

GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{   QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
    timer->start(20);
}

GLWidget::~GLWidget()
{
    for (auto object : m_objects) {
        makeCurrent();
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
    initShapes();

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
    m_projectoinMatrix.perspective(60.0, aspect, 0.1f, 100.f);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 vMatrix;
    vMatrix.setToIdentity();
    vMatrix.translate(0.0, 0.0, m_distance);
    vMatrix.rotate(m_rotation);

    m_shaderProgram.bind();

    m_shaderProgram.setUniformValue("u_projectionMatrix",
                                    m_projectoinMatrix);
    m_shaderProgram.setUniformValue("u_viewMatrix", vMatrix);

    m_shaderProgram.setUniformValue("u_lightPosition",
                                    QVector3D(0.0, 5.0, 10.0));
    m_shaderProgram.setUniformValue("u_lightPower", 1.5f);
    m_shaderProgram.setUniformValue("u_lightColor",
                                    QVector3D(1.0, 1.0, 1.0));
    m_shaderProgram.setUniformValue("u_objectColor",
                                    QVector3D(0.2, 0.35, 0.7));

    for (auto object : m_objects) {
        object->draw(&m_shaderProgram);
    }
/*
    glBegin(GL_LINES);
    glColor3f(1, 1, 1);
    float m_gridSize = 20.f;
    float halfGridSize = m_gridSize / 2.f;
    glVertex3f(0, 0, 0);
    glVertex3f(10, 0, 0);
    for(int i = -halfGridSize; i <= halfGridSize; i++) {
        float fi = static_cast<float>(i);
        float fgs = static_cast<float>(halfGridSize);

        glVertex3f(fi, 0, -fgs);
        glVertex3f(fi, 0, fgs);

        glVertex3f(-fgs, 0, fi);
        glVertex3f(fgs,0,fi);
    }
    glEnd();*/
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

void GLWidget::initShapes()
{
    using namespace Model3D;

    GLObject::GLGrid *grid = new GLObject::GLGrid();
    m_objects.append(grid);

    //MeshData mesh(verts, texts, normals, inds);
    OBJLoader::OBJLoader loader;
    MeshData *cubeMesh = loader.load(":/Objects/cube.obj");

    Q_ASSERT(cubeMesh != nullptr);

    GLObject::GLObject3D *cubeModel = new GLObject::GLObject3D(*cubeMesh);
    cubeModel->translate(QVector3D(-1.5, 0, 0));
    //cubeModel->drawMode = GL_LINES;
    m_objects.append(cubeModel);

    MeshData *headMesh = loader.load(":/Objects/male_head.obj");
    Q_ASSERT(headMesh != nullptr);

    GLObject::GLObject3D *headObj = new GLObject::GLObject3D(*headMesh);
    headObj->translate(QVector3D(1, 0, 0));
    headObj->scale(0.4);
    m_objects.append(headObj);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::LeftButton ||
            event->button() == Qt::RightButton) {
        m_lastMousePosition = QVector2D(event->localPos());
    }
    event->accept();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() != Qt::LeftButton &&
            event->buttons() != Qt::RightButton)
        return;

    if (QApplication::keyboardModifiers() == Qt::Modifier::ALT) {
        QVector2D diff = QVector2D(event->localPos()) - m_lastMousePosition;

        if (event->buttons() == Qt::LeftButton) {
            float angle = diff.length() / 2.0f;

            QVector3D axis(diff.y(), diff.x(), 0.0);
            m_rotation = QQuaternion::fromAxisAndAngle(axis, angle) * m_rotation;
        } else if (event->buttons() == Qt::RightButton) {
            m_distance += diff.x() * m_zoomSpeed;
        } else if (event->buttons() == Qt::MiddleButton) {
            m_cameraTranslation = diff;
        }
    }

    update();
    m_lastMousePosition = QVector2D(event->localPos());
    event->accept();
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    int delta = event->delta();

    if(event->orientation() == Qt::Vertical)
    {
        if(delta > 0)
            m_distance += 1.f * m_zoomSpeed;
        if(delta < 0)
            m_distance -= 1.f * m_zoomSpeed;

        paintGL();
    }
    update();
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
