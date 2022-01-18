QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

CONFIG += console debug c++17
LIBS +=  -lGLU

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    camera.cpp \
    frame.cpp \
    framebuffer.cpp \
    glslprogram.cpp \
    main.cpp \
    mesh.cpp \
    simpleemitter.cpp \
    skybox.cpp \
    texture.cpp \
    widget.cpp

HEADERS += \
    camera.h \
    frame.h \
    framebuffer.h \
    gldebug.h \
    glmath.h \
    glslprogram.h \
    mesh.h \
    simpleemitter.h \
    skybox.h \
    texture.h \
    widget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    fb.frag \
    fb.vert \
    fb_2.frag \
    fb_blur.frag \
    fshader.fsh \
    fshader.glsl \
    phong.fsh \
    phong.vert \
    reflection.frag \
    reflection.vert \
    skybox.frag \
    skybox.vert \
    terrain.frag \
    terrain.vert \
    vshader.glsl \
    vshader.vert

RESOURCES += \
    shaders.qrc
