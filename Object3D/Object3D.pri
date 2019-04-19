!OBJECT_3D {
CONFIG += OBJECT_3D
INCLUDEPATH += $$PWD/../

HEADERS += \
    $$PWD/model3d.h \
    $$PWD/triangulator.h
SOURCES += \
    $$PWD/model3d.cpp \
    $$PWD/triangulator.cpp
}

HEADERS += \
    $$PWD/normalssmoother.h

SOURCES += \
    $$PWD/normalssmoother.cpp
