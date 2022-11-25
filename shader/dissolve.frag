#version 450 core

uniform sampler2D sprite;
uniform float dissolveTime;

in vec4 vertColor; in vec2 vertUV;
out vec4 resColor;

vec4 mod289(vec4 x) {
  return x - floor(x * (1 / 289.0)) * 289;
}
vec4 perm(vec4 x) {
  return mod289(((x * 34) + 1) * x);
}
//simple version of Perlin 3D Noise 
float noise3(vec3 p) {
  vec3 a = floor(p);
  vec3 d = p - a;
  d = d * d * (3 - 2 * d);

  vec4 b = a.xxyy + vec4(0, 1, 0, 1);
  vec4 x1 = perm(b.xyxy);
  vec4 x2 = perm(x1.xyxy + b.zzww);
  vec4 c = x2 + a.zzzz;

  vec4 r = fract(perm(c + 1) / 41.0) * d.z +
    fract(perm(c) / 41.0) * (1 - d.z);
  vec2 res = r.yw * d.x + r.xz * (1 - d.x);

  return res.y * d.y + res.x * (1 - d.y);
}
void main() {

  float gain = 50;
  float noise = noise3(vec3(gain * vertUV.x, 6 * gain * vertUV.y, 0));
  float t = dissolveTime;
  vec4 fromColor = texture(sprite, vertUV);
  vec4 toColor = vec4(fromColor.rgb, 0);
  resColor = fromColor * smoothstep(t, t, noise) + toColor * smoothstep(1 - t, 1 - t, 1 - noise);
}