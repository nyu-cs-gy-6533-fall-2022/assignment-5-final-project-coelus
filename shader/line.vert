
#version 450 core
uniform mat4 projMatrix;

in vec2 pos;

void main()
{
    gl_Position = projMatrix * vec4(pos, 0.0, 1.0);
}