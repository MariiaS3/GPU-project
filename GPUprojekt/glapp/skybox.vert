#version 330

uniform mat4 ModelMat;
uniform mat4 ViewMat;
uniform mat4 ProjMat;

layout (location=0) in vec3 VertexPosition;
layout (location=3) in vec3 VertexNormals;
layout (location=4) in vec3 VertexUV;

out vec3 UV;

void main()
{
    vec4 pos = vec4(VertexPosition, 1.0);
    mat4 mv = ViewMat * ModelMat;
    mv[3][0] = 0.0;
    mv[3][1] = 0.0;
    mv[3][2] = 0.0;
    gl_Position = ProjMat * mv * pos;
    UV = VertexUV;
}
