

#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLEW_STATIC
#include <GL/glew.h>

#include <iostream>
#include <vector>
#include "Shader.h"

using namespace std;
using namespace glm;
class Line
{
public:
    Line(vector<Shader *> &s)
        : shaders(s)
    {

        vector<vec2> vertex = {};

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * vertex.size(), vertex.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    };
    ~Line()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    };
    void Draw(const vector<vec2> &vertex)
    {
        shaders[3]->Use();
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vec2) * vertex.size(), vertex.data(), GL_DYNAMIC_DRAW);
        glBindVertexArray(VAO);
        glLineWidth(3.f);
        glDrawArrays(GL_LINE_LOOP, 0, 2);
    };

private:
    vector<Shader *> &shaders;
    unsigned int VAO, VBO;
};