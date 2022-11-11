# Instructions
use vscode to run. Press z,x,c,v keys to switch different tasks.

# (1) Texturing a sphere

used vertex.glsl and fragment.glsl, pretty straight forward, just load the texture and calculate UV from sphere. and render it with shader.

![](figure/1.jpg)
```cpp
    vec4 textureColor = texture(textureData, UV);
    vec3 normal = normalize(n);
    vec3 lightDir = normalize(lightPos - pos);
    vec3 col = clamp( textureColor.rgb * lightParams.x + 
        textureColor.rgb * max(0.0, dot(normal, lightDir)) + 
        vec3(1.0) * pow(max(0.0, dot( normalize(camPos - pos), normalize( reflect(-lightDir, normal)))), lightParams.y),
        0.0, 1.0);
    
    outColor = vec4(col,1.0);
```

# (2) Render-to-Texture

Used 2 program, 2 set of shaders: vertex.glsl,fragment.glsl,vertex_quad.glsl and fragment_quad.glsl. 
Result almost looks the smae but the process is different.

![](figure/2.jpg)

1. init frame buffer with 1 attachment for full screen quad

```cpp
    glGenTextures(name, &texOb.tex);
    glBindTexture(GL_TEXTURE_2D, texOb.tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    // unbind RenderTex
    glBindTexture(GL_TEXTURE_2D, 0);

    // frame buffer
    glGenFramebuffers(name, &texOb.frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, texOb.frameBuffer);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texOb.tex, 0);

    // depth
    glGenRenderbuffers(name, &texOb.depth);
    glBindRenderbuffer(GL_RENDERBUFFER, texOb.depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WIDTH, HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, texOb.depth);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        cout << "Frame buffer fked" << endl;
        return;
    }

    GLenum drawBuffers[] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, drawBuffers);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
```

2. render to framebuffer

```cpp

    glBindFramebuffer(GL_FRAMEBUFFER, texOb.frameBuffer);
    glViewport(0, 0, WIDTH, HEIGHT);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_CLEAR_BIT);
    glDrawElements(GL_TRIANGLES, triSize * 3, GL_UNSIGNED_INT, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
```

3. framebuffer to full screen quad (you can see the difference if only deaw 1 full screen triangle)


```cpp

    glViewport(0, 0, WIDTH, HEIGHT);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_CLEAR_BIT);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texOb.tex);
    
    // change 6 to 3 to prove this is full screen quad rendering
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    
```
# (3) Deferred Shading

Used 2 program, 2 set of shaders: vertex_geom.glsl,fragment_geom.glsl,vertex_light.glsl and fragment_light.glsl. 
result almost looks the smae but the process is different.

![](figure/3.jpg)


1. init frame buffer with 3 attachments: gNormal,gPos,gColor 
```cpp
int name = 1;
    // frame buffer
    glGenFramebuffers(name, &DSOb.frameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, DSOb.frameBuffer);

    // color
    glGenTextures(name, &DSOb.color);
    glBindTexture(GL_TEXTURE_2D, DSOb.color);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, DSOb.color, 0);

    // pos
    glGenTextures(name, &DSOb.pos);
    glBindTexture(GL_TEXTURE_2D, DSOb.pos);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, 0);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, DSOb.pos, 0);

    // normal
    glGenTextures(name, &DSOb.normal);
    glBindTexture(GL_TEXTURE_2D, DSOb.normal);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, 0);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, DSOb.normal, 0);

    // depth
    glGenRenderbuffers(name, &DSOb.depthRB);
    glBindRenderbuffer(GL_RENDERBUFFER, DSOb.depthRB);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WIDTH, HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, DSOb.depthRB);
   
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        cout << "Frame buffer fked" << endl;
        return;
    }

    GLenum drawBuffers[] = {GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
    glDrawBuffers(3, drawBuffers);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
```

2. render to gNormal,gPos,gColor 

```cpp
  glBindFramebuffer(GL_FRAMEBUFFER, DSOb.frameBuffer);
  glClearColor(0.f, 0.f, 0.f, 0.0f);
  glClear(GL_CLEAR_BIT);
  glDrawElements(GL_TRIANGLES, triSize * 3, GL_UNSIGNED_INT, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
```

3. Use gNormal,gPos,gColor to calculate phong shading

```cpp
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_CLEAR_BIT);
    // Use gNormal,gPos,gColor to calculate phong shading
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, DSOb.color);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, DSOb.pos);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, DSOb.normal);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
```

The fragment shader is like this

```cpp
    vec3 pos = texture(gPos, UV).rgb;
    vec3 normal = normalize(texture(gNormal, UV).rgb);
    vec3 color = texture(gColor, UV).rgb;
    vec3 lightDir = normalize(lightPos - pos);
    vec3 col = clamp( color * lightParams.x + 
        color * max(0.0, dot(normal, lightDir)) + 
        vec3(1.0) * pow(max(0.0, dot( normalize(camPos - pos), normalize( reflect(-lightDir, normal)))), lightParams.y),
        0.0, 1.0);
    outColor = vec4(col,1.0);
```




