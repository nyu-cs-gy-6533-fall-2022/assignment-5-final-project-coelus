#version 450 core


in vec2 UV;
out vec4 resColor;
uniform sampler2D textureData;

void main(){
    resColor = texture( textureData, UV);
}
