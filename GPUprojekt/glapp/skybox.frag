#version 330

uniform samplerCube ColorTexture;

in vec3 UV;

out vec4 fragColor;

void main()
{
    fragColor = texture(ColorTexture, UV);
}
