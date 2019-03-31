#-------------------------------------------------
#
# Project created by QtCreator 2019-02-20T20:19:12
#
#-------------------------------------------------

QT       += core gui opengl testlib
#LIBS     += -lopengl32 # do we need it for Windows?
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Object_intersections
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++14

include(GeomStack/GeomStack.pri)
include(GeomStack/GeomStackTests.pri)

SOURCES += \
        main.cpp \
    model3d.cpp \
    objdataprocessor.cpp \
    glwidget.cpp

HEADERS += \
        glwidget.h \
    model3d.h \
    objloader.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    textures.qrc \
    shaders.qrc
