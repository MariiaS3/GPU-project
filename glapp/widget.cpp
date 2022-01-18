#include "widget.h"
#include <QDebug>
#include <QKeyEvent>
#include <QMouseEvent>
#include <algorithm>
#include <iostream>

Widget::~Widget()
{
}

void Widget::initializeGL() {
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);

    dax = day = 0;
    mouse =0;
    lightPosition = {10, 5, 0};

    framebuffer1 = new Framebuffer();
    framebuffer2 = new Framebuffer();
    framebuffer3 = new Framebuffer();
    framebuffer4 = new Framebuffer();
    framebuffer5 = new Framebuffer();

    fbOutputTexture = new Texture();

    fbDepthTexture = new Texture();

    fbOutputMesh = Mesh::createPlane({1,1});

       emitter = new SimpleEmitter();

    camera1 = new Camera();
    camera2 = new Camera();//do testow
    camera3 = new Camera();
    camera4 = new Camera();
    camera5 = new Camera();

    camera1->pos = {2,0.2,1.2};
    camera2->pos = {2,0.5,-3.5};
    camera3->pos = { 2,1.8,0.8};
    camera4->pos = {0.3,0,-1.7};
    camera5->pos = {2,0.2,1.2};

    camera1->forward = {0.1,-0.2,-3.2};
    camera2->forward = {0.4,-0.2,3.2};
    camera3->forward = {0.1, -1.2,-1.4};
    camera4->forward = {2.8,0.5,1.5};
     camera5->forward = {-0.5,0.5,-1};


    monkeyMesh = Mesh::loadFromObj(":/monkey.obj");
    monkeyMesh->pos = {2, 0, -1};


    monkeyMesh->material.ambient = vec3{ 0.329412f, 0.223529f, 0.027451f };
    monkeyMesh->material.diffuse = vec3{ 0.780392f, 0.568627f, 0.113725f };
    monkeyMesh->material.specular = vec3{ 0.992157f, 0.941176f, 0.807843f };
    monkeyMesh->material.shiness = 27.8974f;

    skybox = new Skybox();
    skybox->loadFromImage("skybox/sky");

    gourardProgram = new GLSLProgram();
    gourardProgram->compileShaderFromFile(":/vshader.vert", GL_VERTEX_SHADER);
    gourardProgram->compileShaderFromFile(":/fshader.fsh", GL_FRAGMENT_SHADER);
    gourardProgram->link();

    skyboxProgram = new GLSLProgram();
    skyboxProgram->compileShaderFromFile("skybox.vert", GL_VERTEX_SHADER);
    skyboxProgram->compileShaderFromFile("skybox.frag", GL_FRAGMENT_SHADER);
    skyboxProgram->link();

    fbProgram = new GLSLProgram();
    fbProgram->compileShaderFromFile("fb.vert", GL_VERTEX_SHADER);
    fbProgram->compileShaderFromFile("fb.frag", GL_FRAGMENT_SHADER);
    fbProgram->link();

    fire = new GLSLProgram();
          fire->compileShaderFromFile("vshader.glsl", GL_VERTEX_SHADER);
          fire->compileShaderFromFile("fshader.glsl", GL_FRAGMENT_SHADER);
    fire->link();


    program =gourardProgram;


    _qtimer = new QTimer(this);
                connect(_qtimer, SIGNAL(timeout()), this, SLOT(animate()));
                connect(_qtimer, SIGNAL(timeout()), this, SLOT(update()));
       _qtimer->start(200);
}

void Widget::animate() {
    emitter->update(5);

   update();
}

void Widget::render(mat4 c) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    glCullFace(GL_FRONT);
    glDepthFunc(GL_LEQUAL);
    skyboxProgram->use();
    skyboxProgram->setUniform("ProjMat", projMat);
    skyboxProgram->setUniform("ViewMat", c);
    skyboxProgram->setUniform("ModelMat", skybox->mesh()->matrix());
    skyboxProgram->setUniform("ColorTexture", 0);
    skybox->bind(0);
    skybox->mesh()->render();

    program->use();
    program->setUniform("ViewMat", c);
    program->setUniform("ProjMat", projMat);
    program->setUniform("ModelMat", monkeyMesh->matrix());
    program->setUniform("MaterialAmbient", monkeyMesh->material.ambient);
    program->setUniform("MaterialDiffuse", monkeyMesh->material.diffuse);
    program->setUniform("MaterialSpecular", monkeyMesh->material.specular);
    program->setUniform("MaterialShiness", monkeyMesh->material.shiness);
    monkeyMesh->render();


}
void Widget::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPointSize(8.0);

    fire->use();
    fire->setUniform("M_v", camera5->matrix());
    fire->setUniform("M_p",projMat);

    emitter->draw();


}
void Widget::paintGL() { //tu bedziemy bidnowac framebuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, (GLint*)&Framebuffer::DefaultFramebuffer); //do czego on zostal zbindowany
    Widget::processCamera();


    //1 dol lewo
    framebuffer1->bind();
    glViewport(0, 0,width(), height());
    render(camera1->matrix());
    framebuffer1->unbind();
    glViewport(0, 0,width()/2, height()/2);
    fbOutputTexture->bind(0);//te 2 rzeczy sa ze soba powiazane, ta tekstura jest widziana jako sampletext
    fbProgram->use();
    fbProgram->setUniform("SamplerTex", 0);//wybieramy teksutre
    fbOutputMesh->render();
    fbOutputTexture->unbind(0);


    framebuffer5->bind();
    glViewport(0, 0,width(), height());
    render();
    framebuffer5->unbind();
    glViewport(0, 0,width()/5, height()/5);
    fbOutputTexture->bind(0);//te 2 rzeczy sa ze soba powiazane, ta tekstura jest widziana jako sampletext
    fbProgram->use();
    fbProgram->setUniform("SamplerTex", 0);//wybieramy teksutre
    fbOutputMesh->render();
    fbOutputTexture->unbind(0);

    //2 dol prawo
    framebuffer2->bind();
    glViewport(0,0, width(), height());
    render(camera2->matrix());
    framebuffer2->unbind();
    glViewport(width()/2,0, width()/2, height()/2);
    fbOutputTexture->bind(0);//te 2 rzeczy sa ze soba powiazane, ta tekstura jest widziana jako sampletext
    fbProgram->use();
    fbProgram->setUniform("SamplerTex", 0);//wybieramy teksutre
    fbOutputMesh->render();
    fbOutputTexture->unbind(0);


    //3 gora lewo
    framebuffer3->bind();
    glViewport(0,0, width(), height());
    render(camera3->matrix());
    framebuffer3->unbind();
    glViewport(0,  height()/2, width()/2, height()/2);
    fbProgram->use();
    fbOutputTexture->bind(0);//te 2 rzeczy sa ze soba powiazane, ta tekstura jest widziana jako sampletext
    fbProgram->setUniform("SamplerTex", 0);//wybieramy teksutre
    fbOutputMesh->render();
    fbOutputTexture->unbind(0);

    //4 gora prawo
    framebuffer4->bind();
    glViewport(0,0, width(), height());
    render(camera4->matrix());
    framebuffer4->unbind();
    glViewport(width()/2, height()/2, width()/2, height()/2);
    fbProgram->use();
    fbOutputTexture->bind(0);//te 2 rzeczy sa ze soba powiazane, ta tekstura jest widziana jako sampletext
    fbProgram->setUniform("SamplerTex", 0);//wybieramy teksutre
    fbOutputMesh->render();
    fbOutputTexture->unbind(0);


}
void Widget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    projMat = perspectiveMat(50.0f, (float)w/(float)h, 0.1f, 500.0f); //tutaj ustawiamy skalowanie

    framebuffer1->initialize(w, h); //wxh -> rozmiar okna
    framebuffer2->initialize(w, h);
    framebuffer3->initialize(w, h);
    framebuffer4->initialize(w, h);
    framebuffer5->initialize(w, h);

    framebuffer1->setTextureAsColorAttachement(0, fbOutputTexture);
    framebuffer2->setTextureAsColorAttachement(0, fbOutputTexture);
    framebuffer3->setTextureAsColorAttachement(0, fbOutputTexture);
    framebuffer4->setTextureAsColorAttachement(0, fbOutputTexture);
    framebuffer5->setTextureAsColorAttachement(0, fbOutputTexture);

    framebuffer1->setTextureAsDepthAttachement(fbDepthTexture); //ustawienie teksutury glebokosci
    framebuffer2->setTextureAsDepthAttachement(fbDepthTexture);
    framebuffer3->setTextureAsDepthAttachement(fbDepthTexture);
    framebuffer4->setTextureAsDepthAttachement(fbDepthTexture);
    framebuffer5->setTextureAsDepthAttachement(fbDepthTexture);

}

void Widget::processCamera() {
    if(mouse==1){
    float dv=0.1f;
    if(keys.find(Qt::Key_S) != keys.end())
        camera1->pos = camera1->pos + camera1->forward * dv;
    else if(keys.find(Qt::Key_W) != keys.end())
        camera1->pos = camera1->pos - camera1->forward*dv;
    if(keys.find(Qt::Key_D) != keys.end())
        camera1->pos = camera1->pos - camera1->x()*dv;
    else if(keys.find(Qt::Key_A) != keys.end())
        camera1->pos = camera1->pos + camera1->x()*dv;
    if(keys.find(Qt::Key_Q) != keys.end())
        camera1->pos = camera1->pos + camera1->up*dv;
    else if(keys.find(Qt::Key_Z) != keys.end())
        camera1->pos = camera1->pos - camera1->up*dv;

    camera1->forward = {0.1,-0.2,-3.2};
    camera1->forward = camera1->forward * rotationMat(dax, 0, 1, 0);
    vec3 cx = normal(camera1->x());
    camera1->forward = camera1->forward * rotationMat(std::clamp<float>(day, -89.999, 89.999), cx.x, cx.y, cx.z);


}
    if(mouse==2){
    float dv=0.1f;
    if(keys.find(Qt::Key_S) != keys.end())
        camera2->pos = camera2->pos + camera2->forward * dv;
    else if(keys.find(Qt::Key_W) != keys.end())
        camera2->pos = camera2->pos - camera2->forward*dv;
    if(keys.find(Qt::Key_D) != keys.end())
        camera2->pos = camera2->pos - camera2->x()*dv;
    else if(keys.find(Qt::Key_A) != keys.end())
        camera2->pos = camera2->pos + camera2->x()*dv;
    if(keys.find(Qt::Key_Q) != keys.end())
        camera2->pos = camera2->pos + camera2->up*dv;
    else if(keys.find(Qt::Key_Z) != keys.end())
        camera2->pos = camera2->pos - camera2->up*dv;

    camera2->forward = {0.4,-0.2,3.2};
    camera2->forward = camera2->forward * rotationMat(dax, 0, 1, 0);
    vec3 cx = normal(camera2->x());
    camera2->forward = camera2->forward * rotationMat(std::clamp<float>(day, -89.999, 89.999), cx.x, cx.y, cx.z);
   }
    if(mouse==3){
    float dv=0.1f;
    if(keys.find(Qt::Key_S) != keys.end())
        camera3->pos = camera3->pos + camera3->forward * dv;
    else if(keys.find(Qt::Key_W) != keys.end())
        camera3->pos = camera3->pos - camera3->forward*dv;
    if(keys.find(Qt::Key_D) != keys.end())
        camera3->pos = camera3->pos - camera3->x()*dv;
    else if(keys.find(Qt::Key_A) != keys.end())
        camera3->pos = camera3->pos + camera3->x()*dv;
    if(keys.find(Qt::Key_Q) != keys.end())
        camera3->pos = camera3->pos + camera3->up*dv;
    else if(keys.find(Qt::Key_Z) != keys.end())
        camera3->pos = camera3->pos - camera3->up*dv;

    camera3->forward = {0.1, -1.2,-1.4};
    camera3->forward = camera3->forward * rotationMat(dax, 0, 1, 0);
    vec3 cx = normal(camera3->x());
    camera3->forward = camera3->forward * rotationMat(std::clamp<float>(day, -89.999, 89.999), cx.x, cx.y, cx.z);

    }
    if(mouse==4){
    float dv=0.1f;
    if(keys.find(Qt::Key_S) != keys.end())
        camera4->pos = camera4->pos + camera4->forward * dv;
    else if(keys.find(Qt::Key_W) != keys.end())
        camera4->pos = camera4->pos - camera4->forward*dv;
    if(keys.find(Qt::Key_D) != keys.end())
        camera4->pos = camera4->pos - camera4->x()*dv;
    else if(keys.find(Qt::Key_A) != keys.end())
        camera4->pos = camera4->pos + camera4->x()*dv;
    if(keys.find(Qt::Key_Q) != keys.end())
        camera4->pos = camera4->pos + camera4->up*dv;
    else if(keys.find(Qt::Key_Z) != keys.end())
        camera4->pos = camera4->pos - camera4->up*dv;

    camera4->forward = {2.8,0.5,1.5};
    camera4->forward = camera4->forward * rotationMat(dax, 0, 1, 0);
    vec3 cx = normal(camera4->x());
    camera4->forward = camera4->forward * rotationMat(std::clamp<float>(day, -89.999, 89.999), cx.x, cx.y, cx.z);
}
    if(mouse==5){
    float dv=0.1f;
    if(keys.find(Qt::Key_D) != keys.end())
        camera5->pos = camera5->pos - camera5->x()*dv*3;
    else if(keys.find(Qt::Key_A) != keys.end())
        camera5->pos = camera5->pos + camera5->x()*dv*3;

    camera5->forward = {-0.5,0.5,-1};
    camera5->forward = camera5->forward * rotationMat(dax, 0, 1, 0);
    vec3 cx = normal(camera5->x());
    camera5->forward = camera5->forward * rotationMat(std::clamp<float>(day, -89.999, 89.999), cx.x, cx.y, cx.z);
}

}

void Widget::keyPressEvent(QKeyEvent *event) {

       keys.insert(event->key());

}


void Widget::keyReleaseEvent(QKeyEvent *event) {
    keys.erase(event->key());
}

void Widget::mousePressEvent(QMouseEvent *event) {
      if(event->x()>=0 && event->x()<=width()/5 && event->y()<=height() && event->y()>=height()-height()/5){
           mouse = 5;
      } else if(event->x()>=0 && event->x()<=width()/2){
        if(event->y()>=0 && event->y()<=height()/2){
            mouse = 3;
        } else{
            mouse = 1;
        }

    }else{
        if(event->y()>=0 && event->y()<=height()/2){
            mouse = 4;
        }else{
            mouse = 2;
        }
    }
    refPoint = event->pos();
}

void Widget::mouseMoveEvent(QMouseEvent *event) {
    dax += event->x() - refPoint.x();
    day += event->y() - refPoint.y();
    refPoint = event->pos();
    update();
}
