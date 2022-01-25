#ifndef WIDGET_H
#define WIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>

#include "mesh.h"
#include "glslprogram.h"
#include "camera.h"
#include "texture.h"
#include "skybox.h"
#include "framebuffer.h"

#include <QTimer>
#include <set>
#include "simpleemitter.h"

class Widget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT

    QTimer *_qtimer;

    //tworzymy framebuffer do ktorego wyrenderowana zawrtosc bedziemy mogli zapisywac
    Framebuffer* framebuffer1,*framebuffer2,*framebuffer3,*framebuffer4,*framebuffer5;
    //mesh na ktorym ta zawartosc sobie wyswietlimy
    Mesh *fbOutputMesh;
    //fbOutputTexture - texutra na ktorej na ktorej framebuffer bedzie zapisywal wygenerowany obraz (kolor bez informacji bez glebokosci)
    Texture *fbDepthTexture;
    Texture *fbOutputTexture;
    //fbDepthTexture -> tekstura glebokosci

    Camera *camera1,*camera5;
    Mesh  *monkeyMesh;

    Skybox *skybox;
    GLSLProgram *program, *skyboxProgram;
    //musimy to obudowac w program
    GLSLProgram *fbProgram, *fire;
    vec3 lightPosition;

    SimpleEmitter* emitter;

    mat4 projMat;

    QPoint refPoint;

    std::set<int> keys;
    float dax, day;
    int mouse;

    void processCamera();
    void render(mat4 c); //do rendera wrzucimy to co bylo w paintglu
    void render();

public slots:
  virtual void animate();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

public:
    ~Widget();

};
#endif // WIDGET_H
