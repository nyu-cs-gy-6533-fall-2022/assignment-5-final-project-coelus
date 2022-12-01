#version 150 core

out vec2 UV;

void main()
{
    vec4 pos[] = vec4[](vec4(1, 1, 0, 1),vec4(-1, 1, 0, 1),vec4(-1, -1, 0, 1),vec4(-1, -1, 0, 1),vec4(1, -1, 0, 1),vec4(1, 1, 0, 1));
    gl_Position = pos[gl_VertexID];
    UV =  gl_Position.xy * 0.5 + vec2(0.5);
}
