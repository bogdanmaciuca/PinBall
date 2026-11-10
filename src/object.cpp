#include "object.hpp"

#include "globals.hpp"

Object::~Object() {
    if (m_vao != 0) {
        glDeleteVertexArrays(1, &m_vao);
    }
    if (m_vbo != 0) {
        glDeleteBuffers(1, &m_vbo);
    }
}

void Object::Load(std::span<Vertex> vertices) {
    m_vertex_count = static_cast<int>(vertices.size());

    if (m_vao == 0) {
        glGenVertexArrays(1, &m_vao);
    }
    if (m_vbo == 0) {
        glGenBuffers(1, &m_vbo);
    }

    glBindVertexArray(m_vao);

    // Load vertex data into VBO
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size_bytes(), vertices.data(), GL_STATIC_DRAW);

    // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, pos)
    );

    // Color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        4,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, color)
    );

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Object::Draw() {
    using namespace glm;

    mat4 model =
        translate(mat4(1.0f), vec3(pos.x, pos.y, 0.0f)) *       // Translate
        rotate(mat4(1.0f), angle, vec3(0.0f, 0.0f, 1.0f)) *     // Rotate
        scale(mat4(1.0f), vec3(size.x, size.y, 0.0f)) *         // Scale
        translate(mat4(1.0f), -vec3(center.x, center.y, 0.0f)); // Center of rotation
    m_shader.Bind();
    m_shader.Set("uModel", model);
    m_shader.Set("uProj", g_proj);

    // m_shader.Set("uModel", mat4(1));
    // m_shader.Set("uProj", mat4(1));
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, m_vertex_count);
    glBindVertexArray(0);
}

