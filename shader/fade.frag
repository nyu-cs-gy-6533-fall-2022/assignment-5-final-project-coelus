#version 450 core

in vec2 UV;
out vec4 resColor;

uniform sampler2D textureData;
uniform float time;

void main() {
    vec4 color = texture(textureData,UV);
    resColor = vec4(color.rgb * abs(cos(time)),1);
}