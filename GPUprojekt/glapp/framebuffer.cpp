#include "framebuffer.h"
#include "gldebug.h"
#include <QOpenGLFunctions>


GLuint Framebuffer::DefaultFramebuffer = 0;

Framebuffer::Framebuffer()
{
    m_fbo = 0;
    m_rbo = 0;
    m_depthTex = 0;
    depthTexture = nullptr;
}


void Framebuffer::initialize(int w, int h)
{
    m_width = w;
    m_height = h;

    initializeOpenGLFunctions();

    if(m_fbo)
        release();

    glGenFramebuffers(1, &m_fbo); //generujemy framebuffer, 1 zapisujemy do m_fbo
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
   //glBindFramebuffer(GL_FRAMEBUFFER, 0);

    PRINT_GL_ERRORS("FrameBuffer::create()");
}

void Framebuffer::release() {
    if(m_fbo) glDeleteFramebuffers(1, &m_fbo);
    m_fbo = 0;

    if(m_rbo) glDeleteRenderbuffers(1, &m_rbo);
    m_rbo = 0;

    textures.clear();
}

bool Framebuffer::isValid() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    GLenum stat = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    if (stat == GL_FRAMEBUFFER_COMPLETE) return true;
    return false;
}

void Framebuffer::bind()
{
    //mamy framebuffer o indeksie m_fbo
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo); //bindujemy indeks do aktywnego framebuffera
   //mamy tablice tekstur, zeby framebuffer generowal cokolwiek do tej tekstury to nalezy mu ta teksture ustawic
    std::vector<GLenum> buffs(textures.size());
    for(std::pair<int, Texture*> tx : textures)
        buffs[tx.first] = GL_COLOR_ATTACHMENT0+tx.first;//wybieramy sobie teksture

    glDrawBuffers(textures.size(), buffs.data());

    GLenum stat = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if( stat !=  GL_FRAMEBUFFER_COMPLETE)
        qDebug() << "Framebuffer is not complete.";

    PRINT_GL_ERRORS("FrameBuffer::bind()");
}

void Framebuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, Framebuffer::DefaultFramebuffer);
}

void Framebuffer::id_bind(GLuint index)
{
    glBindFramebuffer(GL_FRAMEBUFFER,index);
}

void Framebuffer::bindDefault()
{
    QOpenGLContext::currentContext()->functions()->glBindFramebuffer(GL_FRAMEBUFFER, Framebuffer::DefaultFramebuffer); //zaladowanie statycznego framebuffera
}

void Framebuffer::setTextureAsColorAttachement(int unit, Texture* tex)
{
    if(m_fbo)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
        if(tex->width != m_width || tex->height != m_height) {
            tex->create(m_width, m_height);

        }
        textures[unit] = tex; //zaladowanie odpowiedniej tekstury
        textures[unit]->bind(0); //zbindowanie jej
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+unit, GL_TEXTURE_2D, textures[unit]->handle, 0);//zapisanie do tablicy tekstur teksutre o konrkretynm id
        textures[unit]->unbind(0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    PRINT_GL_ERRORS("FrameBuffer::setTexture();");
}

void Framebuffer::setTextureAsDepthAttachement(Texture *tex)
{
    if(m_fbo)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

        if(tex->width != m_width || tex->height != m_height)//sprawdzenie rozmiaru
            tex->createDepth(m_width, m_height);//tworzenie

        depthTexture = tex;
        depthTexture->bind(0);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture->handle, 0); //ladujemy

        depthTexture->unbind(0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    PRINT_GL_ERRORS("FrameBuffer::setTexture();");
}

