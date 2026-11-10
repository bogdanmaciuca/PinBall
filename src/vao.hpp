#pragma once

#include "shader.hpp"

struct Vertex {
    glm::vec2 pos;
    glm::vec4 color;
};

struct VAO {
    float     angle = 0.0f;
    glm::vec2 pos   = glm::vec2(0.0f);
    glm::vec2 size  = glm::vec2(0.0f);

    ~VAO();
    void Load(std::span<Vertex> vertices);
    void Draw();
private:
    Shader m_shader;
    GLuint m_vao       = 0;
    GLuint m_vbo       = 0;
    int m_vertex_count = 0;
};

