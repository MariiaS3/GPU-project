#include "simpleemitter.h"
#include <GLFW/glfw3.h>
#include <iostream>

SimpleEmitter::SimpleEmitter()
{
    // create 100 particles
    particles.resize( 500 );
    for ( uint i = 0 ; i < particles.size() ; ++i )
    {
        // give every particle a random position
        particles[i].position =rand_Vec3Range(-1.0, 1.0);
        particles[i].lifetime = rand_FloatRange(1.0,2.0);
        particles[i].r = 1.0;
        particles[i].g = 0.0;
        particles[i].b = 0.0;
    }

       initializeOpenGLFunctions();
    // create a vertex and position buffer
     glGenVertexArrays(1,&vao);
     glBindVertexArray(vao);
     glGenBuffers(1,&positionBuffer);
     glGenBuffers(1, &colorbuffer);
     // Initialize with empty (NULL) buffer : it will be updated later, each frame.
    // fill the position buffer
    glBindBuffer( GL_ARRAY_BUFFER , this->positionBuffer );
    glBufferData( GL_ARRAY_BUFFER , particles.size()*4*sizeof(float) , NULL , GL_STATIC_DRAW );

    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, particles.size() * 4 * sizeof(float), NULL, GL_STATIC_DRAW);
    glBindBuffer( GL_ARRAY_BUFFER , 0);
    glBindVertexArray(0);
}







void SimpleEmitter::update( const float dt )
{
    for ( uint i = 0 ; i < particles.size() ; ++i )
    {
        // subtract from the particles lifetime
        particles[i].lifetime -= dt;

        // if the lifetime is below 0 respawn the particle
        if ( particles[i].lifetime <= 0.0f )
        {
            particles[i].position =rand_Vec3Range(-1.0, 1.0);
            particles[i].lifetime = rand_FloatRange(1.0,6.0);

            particles[i].r = 1;
            particles[i].g = 0.5f;
            particles[i].b = 0;


        }

        // move the particle down depending on the delta time
        particles[i].position =particles[i].position - vec3{ 0.0f ,  dt*(-0.5f), 0.0f };

        // update the position buffer
        positions[i*4+0] = particles[i].position.x;
        positions[i*4+1] = particles[i].position.y;
        positions[i*4+2] = particles[i].position.z;
        positions[i*4+3] = particles[i].lifetime;

        color[4*i+0] =  particles[i].r;
        color[4*i+1] =  particles[i].g;
        color[4*i+2] =  particles[i].b;
    }

    glBindVertexArray(vao);

    // update the position buffer
    glBindBuffer( GL_ARRAY_BUFFER , positionBuffer );
    glBufferSubData( GL_ARRAY_BUFFER , 0 , particles.size()*4*sizeof(float) , positions );

    glBindBuffer( GL_ARRAY_BUFFER , colorbuffer );
    glBufferSubData( GL_ARRAY_BUFFER , 0 , particles.size()*4*sizeof(float) , color );

    glBindBuffer( GL_ARRAY_BUFFER , 0 );
    glBindVertexArray(0);
}



void SimpleEmitter::draw()
{

    glBindVertexArray(vao);

    // update the position buffer
    glBindBuffer( GL_ARRAY_BUFFER , positionBuffer );
    glBufferSubData( GL_ARRAY_BUFFER , 0 , this->particles.size()*4*sizeof(float) , positions );

    glBindBuffer( GL_ARRAY_BUFFER , colorbuffer );
    glBufferSubData( GL_ARRAY_BUFFER , 0 , particles.size()*4*sizeof(float) , color );

    // position buffer
    glBindBuffer( GL_ARRAY_BUFFER , positionBuffer );
    glEnableVertexAttribArray( 4 );
    glVertexAttribPointer( 4 , 4 , GL_FLOAT , GL_FALSE , 0 , 0 );

    glBindBuffer( GL_ARRAY_BUFFER , colorbuffer );
    glEnableVertexAttribArray( 2 );
    glVertexAttribPointer( 2 , 4 , GL_FLOAT , GL_FALSE , 0 , 0 );

    // draw points
    glDrawArrays(GL_POINTS, 0, 500);

    glBindBuffer( GL_ARRAY_BUFFER , 0 );
    glBindVertexArray(0);
}

