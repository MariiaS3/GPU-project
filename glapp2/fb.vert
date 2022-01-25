#version 330

uniform vec3 Color = vec3(1,1,1);

layout (location=0) in vec3 VertexPosition;
layout (location=4) in vec2 VertexUV;

out vec4 VColor;
out vec2 UV;

void main(void)
{
    gl_Position = vec4(VertexPosition, 1.0); //ten shader possluzy tylko do wyswietlania tekstury na plaskim ekranie
    VColor = vec4(Color,1); //vec4(VertexColor, 1.0);
    UV = VertexUV;
}
