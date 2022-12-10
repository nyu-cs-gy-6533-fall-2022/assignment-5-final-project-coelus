#version 450 core

in vec2 UV;
out vec4 resColor;

uniform sampler2D textureData;
uniform float time;

void main() {
    float ker[9] = float[](
        1, 1, 1,
        1, 1, 1,
        1, 1, 1
    );
    float err = time/450.0;
    vec2 errs[9] = vec2[](vec2(-err, err), vec2(0, err), vec2(err, err), vec2(-err, 0), vec2(0 , 0), vec2(err, 0), vec2(-err, -err), vec2(0, -err), vec2(err, -err));


    vec3 mixTex[9];
    for (int i = 0; i < 9; i++) {
        mixTex[i] = vec3(texture(textureData, UV + errs[i]));
    }
    vec3 res = vec3(0);
    for (int i = 0; i < 9; i++)
        res += mixTex[i] * ker[i]/9.0;

    resColor = vec4(res, 1.0);
}