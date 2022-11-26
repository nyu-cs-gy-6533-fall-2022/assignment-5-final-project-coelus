#version 450 core

uniform sampler2D sprite;
uniform vec2 resolution;

uniform int lightLen;
//pos+dir
uniform vec4 lights[15];

in vec4 vertColor;
in vec2 vertUV;


out vec4 resColor;
void main()
{
    vec2 pixelPos = resolution*vertUV;
    vec4 pixelColor = vertColor* texture(sprite,vertUV);
    vec4 lightColor=vec4(1,1,0,1);
    float gain = 3.0;
    float totalBrightness=0.0;

    //point light
    for(int i = 0; i < lightLen; i++){
        vec2 lightPos = vec2(lights[i].x,lights[i].y);
        vec2 lightDir = vec2(lights[i].z,lights[i].w);
        vec2 lightDis = lightPos -pixelPos;
        float brightness = clamp(dot(normalize(lightDis), lightDir), 0.0, 1.0);
        brightness *= clamp(1 - (length(lightDis) / 1200.0), 0.0, 1.0);
        brightness=clamp(brightness*gain, 0.0, gain);
        totalBrightness+=brightness;
    }
    totalBrightness=clamp(totalBrightness, 0.7, gain);

    resColor = pixelColor*totalBrightness;
}