#version 450 core
#define MAXLEN 15


struct Light{
  int type;
  vec2 pos;
  vec2 dir;
};

uniform sampler2D sprite;
uniform vec2 resolution;
uniform float time;


uniform int lightLen;
//pos+dir
uniform Light lights[MAXLEN];

in vec4 vertColor; 
in vec2 vertUV;

out vec4 resColor;
float round(float v){
  return v>0.7?1.0:0;
}
void main() {
  vec2 pixelPos = resolution * vertUV;
  vec4 pixelColor = vertColor * texture(sprite, vertUV);
  vec4 lightColor = vec4(1, 1, 0, 1);
  float gain = 3.0;
  float totalBrightness = 0.0;
  
  //point light
  for (int i = 0; i < lightLen; i++) {
    vec2 lightPos = lights[i].pos;
    vec2 lightDir = lights[i].dir;
    vec2 lightDis = lightPos - pixelPos;
    float brightness = 0.0;
    //direct light
    if(lights[i].type==0){
      float blink = 1;
      //brink
      if(sin(time+i)>=0.5){
          blink = (-cos(time+i) + 1) / 2.0 ;
      }
      brightness = clamp(dot(normalize(lightDis), lightDir), 0.0, 1.0);
      brightness *= clamp(1 - (length(lightDis) / 1200.0), 0.0, 1.0);
      brightness = clamp(brightness * gain , 0.0, gain)* blink;
     
    }else{//point light
      //0.5 to 1.0
      float blink = (-cos(time+i) + 3) / 4.0;
      brightness = 1.0 / length(lightDis)*100*blink;
    }
    totalBrightness += brightness;
  }
  totalBrightness = clamp(totalBrightness, 0.7, gain);

  resColor=pixelColor * totalBrightness;
  if(resColor.a>0){
    resColor.a=1;
  }
 
}