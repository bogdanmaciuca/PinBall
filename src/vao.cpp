#include "vao.hpp"

VAO::~VAO() {
    if (m_vao != 0) {
        glDeleteVertexArrays(1, &m_vao);
    }
    if (m_vbo != 0) {
        glDeleteBuffers(1, &m_vbo);
    }
}

void VAO::Load(std::span<Vertex> vertices) {
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

void VAO::Draw() {
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, m_vertex_count);
    glBindVertexArray(0);
}
