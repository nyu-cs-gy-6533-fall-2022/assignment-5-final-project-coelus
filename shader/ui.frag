#version 450 core


in vec2 UV;
out vec4 resColor;
uniform sampler2D textureData;

uniform float scale;
uniform float err;

void main(){
    resColor = texture( textureData, vec2(UV.x*scale+err,1-UV.y));
}
