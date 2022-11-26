
#version 450 core
uniform mat4 projMatrix;
uniform mat4 modelMatrix;
uniform vec4 color;


in vec2 point;
in vec2 uv;

out vec4 vertColor;
out vec2 vertUV;

void main()
{
    gl_Position = projMatrix * modelMatrix * vec4(point, 0.0, 1.0);
    vertColor = color;
    vertUV = uv;
   
}