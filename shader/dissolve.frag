#version 450 core

uniform sampler2D sprite;
uniform float dissolveTime;

in vec4 vertColor;
in vec2 vertUV;
out vec4 resColor;
void main()
{
    resColor = vec4(vertColor.r,vertColor.g,vertColor.b,dissolveTime/1.5f) * texture(sprite,vertUV);
}