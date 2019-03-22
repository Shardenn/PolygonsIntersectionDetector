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

    //glViewport(0, 0, w, h);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 vMatrix;
    vMatrix.setToIdentity();
    //vMatrix.translate(0.0,0.0,-5.0);
    //vMatrix.rotate(m_rotation);

    m_shaderProgram.bind();
    m_shaderProgram.setUniformValue("u_projectionMatrix", m_projectoinMatrix);
    m_shaderProgram.setUniformValue("u_viewMatrix", vMatrix);
    m_shaderProgram.setUniformValue("u_lightPosition", QVector4D(0.0, 0.0, 0.0, 1.0));
    m_shaderProgram.setUniformValue("u_lightPower", 5.0f);

    for (auto object : m_objects) {
        //object->draw(&m_shaderProgram, context()->functions());
    }

    m_shaderProgram.setUniformValue("u_texture", 0);

    QMatrix4x4 modelMatrix;
    modelMatrix.setToIdentity();
    modelMatrix.translate(QVector3D(5.0, 5.0, 5.0));
    m_shaderProgram.setUniformValue("modelMatrix", modelMatrix);

    m_vertexBuffer.bind();

    int offset = 0;

    int vertLoc = m_shaderProgram.attributeLocation("vertexPosition");
    m_shaderProgram.enableAttributeArray(vertLoc);
    m_shaderProgram.setAttributeBuffer(vertLoc, GL_FLOAT, offset, 3, sizeof(QVector3D));
//    offset += sizeof(QVector3D);

//    int texLoc = m_shaderProgram.attributeLocation("textureCoordinate");
//    m_shaderProgram.enableAttributeArray(texLoc);
//    m_shaderProgram.setAttributeBuffer(texLoc, GL_FLOAT, offset, 2, sizeof(VertexData));

//    offset += sizeof(QVector2D);

//    int normLoc = m_shaderProgram.attributeLocation("normal");
//    m_shaderProgram.enableAttributeArray(normLoc);
//    m_shaderProgram.setAttributeBuffer(normLoc, GL_FLOAT, offset, 3, sizeof(VertexData));


//    qDebug() << m_vertexBuffer.size();
//    qDebug() << m_indexBuffer.size();

    m_indexBuffer.bind();
    glDrawElements(GL_TRIANGLES, m_indexBuffer.size(), GL_UNSIGNED_INT, nullptr);

    m_indexBuffer.release();
    m_vertexBuffer.release();
    //m_texture->release();
}

void GLWidget::initShaders()
{
    if (!m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                                 QDir::currentPath() + "/../PolygonsIntersectionDetector/Shaders/lightingVertexShader.vsh")) {
        qDebug() << "The vertex shader was not compiled";
    }

    if (!m_shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                                 QDir::currentPath() + "/../PolygonsIntersectionDetector/Shaders/lightingFragmentShader.fsh")) {
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
    auto cubeData = ObjDataProcessor::getModelData(QDir::currentPath() + "/../PolygonsIntersectionDetector/Objects/cube.obj");

    //Model3D* cube = new Model3D (cubeData.m_vertexesData, cubeData.m_polygonVertIndexes, texture);
    //m_objects.append(cube);

    m_texture = new QOpenGLTexture(QImage(QDir::currentPath() + "/../PolygonsIntersectionDetector/Textures/QtCreator.png").mirrored());
    m_texture->setMinificationFilter(QOpenGLTexture::Nearest);

    // Set bilinear filtering mode for texture magnification
    m_texture->setMagnificationFilter(QOpenGLTexture::Linear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    m_texture->setWrapMode(QOpenGLTexture::Repeat);
//    QVector<VertexData> vertexes;
//    vertexes.append(VertexData(QVector3D(-1, 1, 0), QVector2D(0.0, 1.0), QVector3D(1.0, 1.0, 1.0)));
//    vertexes.append(VertexData(QVector3D(1, 1, 0), QVector2D(0.0, 0.0), QVector3D(1.0, 1.0, 1.0)));
//    vertexes.append(VertexData(QVector3D(1, -1, 0), QVector2D(1.0, 1.0), QVector3D(1.0, 1.0, 1.0)));
//    vertexes.append(VertexData(QVector3D(-1, -1, 0), QVector2D(1.0, 0.0), QVector3D(1.0, 1.0, 1.0)));
    QVector<QVector3D> vertexes;
    QVector<QVector3D> normals;
    QVector<QVector2D> textureCoords;

    vertexes.append(QVector3D(-0.5, 0.5, 0));
    vertexes.append(QVector3D(0.5, 0.5, 0));
    vertexes.append(QVector3D(0.5, -0.5, 0));
    vertexes.append(QVector3D(-0.5, -0.5, 0));

    auto currNormal = QVector3D::crossProduct(vertexes[1] - vertexes[0], vertexes[2] - vertexes[1]);
    for (int i = 0; i < 4; i++) {
        normals.append(currNormal);
    }

    textureCoords.append(QVector2D(0, 0));
    textureCoords.append(QVector2D(1, 0));
    textureCoords.append(QVector2D(0, 1));
    textureCoords.append(QVector2D(1, 1));

    QVector<GLuint> indexes;

    indexes.append(0);
    indexes.append(3);
    indexes.append(2);
    indexes.append(0);
    indexes.append(1);
    indexes.append(2);

    m_vertexBuffer.create();
    m_vertexBuffer.bind();
    m_vertexBuffer.allocate(vertexes.constData(), vertexes.size() * sizeof(QVector3D));
    m_vertexBuffer.release();


    m_indexBuffer.create();
    m_indexBuffer.bind();
    m_indexBuffer.allocate(indexes.constData(), indexes.size() * sizeof(GLuint));
    m_indexBuffer.release();

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
