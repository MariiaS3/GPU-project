#version 330
//najprostszy shader / tutaj dostajemy teksutre kolorowa

uniform sampler2D SamplerTex;

in vec4 VColor;
in vec2 UV;

out vec4 FragColor;


void main()
{

    FragColor = texture(SamplerTex, UV); //shader

}
