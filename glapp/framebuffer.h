#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <QOpenGLFunctions_3_3_Core>
#include "texture.h"

class Framebuffer : protected QOpenGLFunctions_3_3_Core
{

    GLuint m_fbo,m_fbo2;
    GLuint m_rbo;
    GLuint m_depthTex;

public:
    int n;

    static GLuint DefaultFramebuffer; //tutaj zapiszemy informacje gdzie jest ten framebuffer do ktorego chcemy pisac
    static void bindDefault();

    int m_width;
    int m_height;
    int width() { return m_width; }
    int height() { return m_height; }

    std::map<int, Texture*> textures;
    Texture *depthTexture;

    Framebuffer();
    Framebuffer(GLuint index);

    void initialize(int w, int h);
    void id_bind(GLuint index);
    void release();
    bool isValid();

    void bind();
    void unbind();

    void setTextureAsColorAttachement(int unit, Texture* tex);
    void setTextureAsDepthAttachement(Texture *tex);


};

#endif // FRAMEBUFFER_H
