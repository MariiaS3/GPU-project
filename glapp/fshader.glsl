#version 330 core

in vec3 color;
out vec4 fragColor;

void main()
{
   fragColor = vec4(color,1);//vec4( 0,1.0,0,1.0 );
}

