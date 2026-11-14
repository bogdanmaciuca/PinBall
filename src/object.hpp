#pragma once

#include "shader.hpp"

struct Vertex {
    glm::vec2 pos;
    glm::vec4 color;
};

enum ObjectPhysicsType {
    OPT_BALL,
    OPT_POLY,
};

struct Object {
    float     angle  = 0.0f;            // Radians
    glm::vec2 pos    = glm::vec2(0.0f); // In pixels
    glm::vec2 size   = glm::vec2(0.0f); // In pixels
    glm::vec2 center = glm::vec2(0.0f); // Relative to the vertices without scaling
    glm::vec4 color  = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
    ObjectPhysicsType physics_type = OPT_POLY;

    glm::vec2 velocity = glm::vec2(0.0f);

    Object() = default;
    Object(Shader* shader, ObjectPhysicsType opt = OPT_POLY) : physics_type(opt), m_shader(shader) {}
    ~Object();
    void Load(std::span<const Vertex> vertices);
    void Draw();
    void UpdateModel();

    std::span<const glm::vec2> GetWorldVertices() const { return m_world_vertices; }
private:
    Shader*   m_shader = nullptr;
    GLuint    m_vao    = 0;
    GLuint    m_vbo    = 0;
    glm::mat4 m_model  = glm::mat4(1.0f);
    std::vector<Vertex>    m_vertices;
    std::vector<glm::vec2> m_world_vertices;
};

std::vector<Vertex> MakeLinePoly(std::span<const Vertex> vertices, float line_width);

