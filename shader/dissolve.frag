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
float noiser(vec3 p) {
  vec3 fl = floor(p);
  vec3 dif = p - fl;
  dif = dif * dif * (3 - (2 * dif));

  vec4 a = fl.xxyy + vec4(0, 1, 0, 1);
  vec4 x1 = perm(a.xyxy);
  vec4 x2 = perm(x1.xyxy + a.zzww);
  vec4 b = x2 + fl.zzzz;

  vec4 r = fract(perm(b + 1) / 41.0) * dif.z +
    fract(perm(b) / 41.0) * (1 - dif.z);
  vec2 res = r.yw * dif.x + r.xz * (1 - dif.x);

  return res.y * dif.y + res.x * (1 - dif.y);
}
void main() {

  float gain = 50;
  float noise = noiser(vec3(gain * vertUV.x, 6 * gain * vertUV.y, 0));
  float t = dissolveTime;
  vec4 fromColor = texture(sprite, vertUV);
  vec4 toColor = vec4(fromColor.rgb, 0);
  resColor = fromColor * smoothstep(t, t, noise) + toColor * smoothstep(1 - t, 1 - t, 1 - noise);
}