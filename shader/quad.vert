#version 450 core
//full screen vert shader
out vec2 UV;

void main()
{
    vec2 pos[] = vec2[](vec2(-1,-1),vec2(3,-1),vec2(-1, 3));
    gl_Position = vec4(pos[gl_VertexID],0,1);
    UV =  gl_Position.xy * 0.5 + vec2(0.5);
}
