#ifndef TEXTURE_H
#define TEXTURE_H

#include <QOpenGLFunctions_3_3_Core>
#include <glmath.h>

class Texture : protected QOpenGLFunctions_3_3_Core{

public:
    GLuint handle;

    int width, height; //fb


    Texture();
    bool loadFromFile(const char *filename);
    bool loadFromData(vec4* data, int w, int h);
    //fb
    bool create(int w, int h);
    bool createDepth(int w, int h);//funkcja tworzenia teksutry glebokosci
    bool createNormal(int w, int h);

    void bind(int unit);
    void unbind(int unit);
};

#endif // TEXTURE_H
