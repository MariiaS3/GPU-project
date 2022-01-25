#ifndef SIMPLEEMITTER_H
#define SIMPLEEMITTER_H

#include <QOpenGLFunctions_3_3_Core>
#include "glmath.h"

#include "glslprogram.h"
#include "camera.h"

#define MAX_PARTICLES 1024 * 50

class SimpleEmitter : public Frame, protected QOpenGLFunctions_3_3_Core
{
public:
    SimpleEmitter();


        void update( const float dt );

        void update( Camera *camera,  double currentTime);

        void draw();
        void render();
        int FindUnusedParticle();
    private:
        struct Particle
        {
            vec3 position;
            float lifetime;
            float r,g,b,a; // Color

        };
        std::vector< Particle > particles;
        float positions[2000];
        float color[2000];

        GLuint vertexBuffer,vao;
        GLuint positionBuffer;
        GLuint colorbuffer;
};


#endif // SIMPLEEMITTER_H
