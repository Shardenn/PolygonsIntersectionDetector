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
#include "ICP/icp.h"

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

    // TODO
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

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

    vMatrix.translate(m_cameraTranslation.x(), m_cameraTranslation.y(),
                      m_distance);
    vMatrix.rotate(m_rotation);
//    vMatrix.rotate(m_cameraVerticalRotation);
//    vMatrix.rotate(m_cameraHorizontalRotation);

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
    glEnd();
    */
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
    //m_objects.append(grid);

    OBJLoader::OBJLoader loader;


// ------------LEG
    MeshData *legMesh = loader.load(":/Objects/leg_simplified.obj");

    Q_ASSERT(legMesh != nullptr);

    GLObject::GLObject3D *legModel = new GLObject::GLObject3D(*legMesh);
    legModel->translate(QVector3D(-1.5, -7, 0));
    m_objects.append(legModel);

// ----------- HEAD
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
            event->button() == Qt::RightButton ||
            event->buttons() == Qt::MiddleButton) {
        m_lastMousePosition = QVector2D(event->localPos());
    }
    event->accept();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() != Qt::LeftButton &&
            event->buttons() != Qt::RightButton &&
            event->buttons() != Qt::MiddleButton)
        return;

    if (QApplication::keyboardModifiers() == Qt::Modifier::ALT) {
        QVector2D diff = QVector2D(event->localPos()) - m_lastMousePosition;

        if (event->buttons() == Qt::LeftButton) {
            float angle = diff.length() / 2.0f;
            float angleHor = abs(diff.y());
            float angleVert = diff.x();

            QVector3D axisHor(diff.y(), 0.0, 0.0);
            QVector3D axisVert(0.0, 1.0, 0.0);

            m_cameraVerticalRotation = QQuaternion::fromAxisAndAngle(
                        axisVert, angleVert) * m_cameraVerticalRotation;
            m_cameraHorizontalRotation = QQuaternion::fromAxisAndAngle(
                        axisHor, angleHor) * m_cameraHorizontalRotation;

            QVector3D axis(diff.y(), diff.x(), 0.0);
            m_rotation = m_cameraVerticalRotation * m_cameraHorizontalRotation;
        } else if (event->buttons() == Qt::RightButton) {
            m_distance += diff.x() * m_zoomSpeed;
        } else if (event->buttons() == Qt::MiddleButton) {
            const float prev_x = m_cameraTranslation.x();
            const float prev_y = m_cameraTranslation.y();

            m_cameraTranslation.setX(prev_x + (diff.x() * 0.01f));
            m_cameraTranslation.setY(prev_y + (-diff.y() * 0.01f));
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

void GLWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Q) {
        OBJLoader::OBJLoader loader;

        // ------------SUSANNE SMOOTHED
        MeshData *susMeshSmoothed = loader.load(":/Objects/cube.obj");
        Q_ASSERT(susMeshSmoothed != nullptr);

        GLObject::GLObject3D *susannaSmoothed =
                new GLObject::GLObject3D(*susMeshSmoothed);
        susannaSmoothed->translate(QVector3D(1.0, 2.5, 0.0));
        m_objects.append(susannaSmoothed);

    // -----------SUSANNE 2
        MeshData *susanna2 = loader.load(":/Objects/cube.obj");
        Q_ASSERT(susanna2 != nullptr);

        GLObject::GLObject3D *sussanna2obj =
                new GLObject::GLObject3D(*susanna2);
        sussanna2obj->translate(QVector3D(-4.0, 0.5, -1.0));
        //sussanna2obj->rotate(QQuaternion::fromAxisAndAngle(
          //                       QVector3D(-1.0, -1.0, -1.0), 50));
        m_objects.append(sussanna2obj);

        ICP icp;
        icp.performRigidAlignment(susMeshSmoothed->positions, susanna2->positions,
                                  susannaSmoothed->getModelMatrix(), sussanna2obj->getModelMatrix() );
    }
}

void GLWidget::keyReleaseEvent(QKeyEvent *event)
{

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
