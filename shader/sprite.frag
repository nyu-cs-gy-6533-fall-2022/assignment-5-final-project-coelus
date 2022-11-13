#version 450 core

uniform sampler2D sprite;
in vec4 vertColor;
in vec2 vertUV;
out vec4 resColor;
void main()
{
    resColor = vertColor * texture(sprite,vertUV);
}