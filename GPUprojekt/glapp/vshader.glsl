#version 330 core

layout ( location = 2 ) in vec3 vColor;
layout ( location = 4 ) in vec4 position;

uniform mat4 M_v;
uniform mat4 M_p;

out vec3 color;

void main()
{

    gl_Position    = M_p * M_v * vec4( position.xyz , 1 );
    color = vColor;
}
