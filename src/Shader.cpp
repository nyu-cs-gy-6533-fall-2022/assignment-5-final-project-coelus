#include "Shader.h"

// from ../shader/
Shader::Shader(const string vertShaderName, const string fragShaderName)
{
    // read file
    ifstream vertShaderFile("../shader/" + vertShaderName);
    stringstream vertRaw;
    vertRaw << vertShaderFile.rdbuf();
    ifstream fragShaderFile("../shader/" + fragShaderName);
    stringstream fragRaw;
    fragRaw << fragShaderFile.rdbuf();

    string vertShaderStr = vertRaw.str();
    string fragShaderString = fragRaw.str();

    unsigned int vert, frag;
    const char *vertShader = vertShaderStr.c_str();
    const char *fragShader = fragShaderString.c_str();

    id = glCreateProgram();

    vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &vertShader, NULL);
    glCompileShader(vert);
    glAttachShader(id, vert);

    frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &fragShader, NULL);
    glCompileShader(frag);
    glAttachShader(id, frag);

    glLinkProgram(id);

    glDeleteShader(vert);
    glDeleteShader(frag);

    Use();
    SetVec4("color", vec4(1, 1, 1, 1));
}

Shader::~Shader()
{
    glDeleteShader(id);
}

void Shader::Use()
{
    glUseProgram(id);
}

void Shader::SetFloat(const char *name, float val)
{
    GLfloat *glval = new GLfloat[1];
    glval[0] = val;
    glUniform1fv(glGetUniformLocation(id, name), 1, glval);
}

void Shader::SetMat(const char *name, mat4 mat)
{
    glUniformMatrix4fv(glGetUniformLocation(id, name), 1, GL_FALSE, value_ptr(mat));
}
void Shader::SetVec4(const char *name, vec4 vec)
{
    glUniform4fv(glGetUniformLocation(id, name), 1, value_ptr(vec));
}
