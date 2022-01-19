#include "texture.h"
#include <QImage>
#include <QGLWidget>
#include "gldebug.h"

Texture::Texture() {
    initializeOpenGLFunctions();
    glGenTextures(1, &handle);
}

bool Texture::loadFromFile(const char *filename) {
    QImage image(filename);
    if(!image.isNull()) {
        image = QGLWidget::convertToGLFormat(image);

        bind(0);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        unbind(0);
        return true;

    }
    else {
        PRINT_GL_ERRORS("Texture::loadFromFile");
        return false;
    }
}

bool Texture::loadFromData(vec4* data, int w, int h)
{
    width = w;
    height = h;
    bind(0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_FLOAT, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    unbind(0);
    PRINT_GL_ERRORS("Texture::loadFromData()");
    return true;
}

void Texture::bind(int unit) {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, handle);
}

bool Texture::create(int w, int h) {
    bind(0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,  GL_RGBA, GL_UNSIGNED_BYTE, nullptr);//fb
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    unbind(0);
    PRINT_GL_ERRORS("Texture::create()");
    return true;
}

bool Texture::createDepth(int w, int h) {
//    bind(0);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, w, h, 0,  GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    unbind(0);
//    PRINT_GL_ERRORS("Texture::createDepth()");
//    return true;
}

bool Texture::createNormal(int w, int h) {
    bind(0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, w, h, 0,  GL_RGB, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    unbind(0);
    PRINT_GL_ERRORS("Texture::createDepth()");
    return true;
}

void Texture::unbind(int unit) {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, 0);
}


