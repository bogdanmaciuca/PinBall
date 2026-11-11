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

void Object::Load(std::span<const Vertex> vertices) {
    m_vertices.resize(vertices.size());
    std::copy(vertices.begin(), vertices.end(), m_vertices.begin());

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

void Object::UpdateModel() {
    using namespace glm;

    m_model =
        translate(mat4(1.0f), vec3(pos.x, pos.y, 0.0f)) *       // Translate
        rotate(mat4(1.0f), angle, vec3(0.0f, 0.0f, 1.0f)) *     // Rotate
        scale(mat4(1.0f), vec3(size.x, size.y, 0.0f)) *         // Scale
        translate(mat4(1.0f), -vec3(center.x, center.y, 0.0f)); // Center of rotation

    m_world_vertices.resize(m_vertices.size());
    for (uint32_t i = 0; i < m_vertices.size(); i++) {
        vec4 v = m_model * vec4(m_vertices[i].pos.x, m_vertices[i].pos.y, 0.0f, 1.0f);
        m_world_vertices[i] = vec2(v.x, v.y);
    }
}

void Object::Draw() {
    m_shader->Bind();

    m_shader->Set("uProj", g_proj);
    m_shader->Set("uModel", m_model);
    m_shader->Set("uColor", color);

    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
    glBindVertexArray(0);
}

// `points[last]` must equal `points[first]`
std::vector<Vertex> MakeLinePoly(std::span<const Vertex> points, float line_width) {
    assert(points.size() >= 2);
    std::vector<Vertex> vertices;

    float half_w = line_width / 2.0f;
    size_t segment_count = points.size() - 1;
    vertices.reserve(segment_count * 6); // 2 triangles per segment

    for (size_t i = 0; i < segment_count; i++) {
        const auto& p0 = points[i];
        const auto& p1 = points[i + 1];

        glm::vec2 tangent = glm::normalize(p1.pos - p0.pos);
        glm::vec2 normal = glm::vec2(-tangent.y, tangent.x) * half_w;

        Vertex A = { p0.pos + normal, p0.color };
        Vertex B = { p0.pos - normal, p0.color };
        Vertex C = { p1.pos + normal, p1.color };
        Vertex D = { p1.pos - normal, p1.color };

        // ABC
        vertices.push_back(A);
        vertices.push_back(B);
        vertices.push_back(C);
        // BDC
        vertices.push_back(B);
        vertices.push_back(D);
        vertices.push_back(C);
    }

    return vertices;
}

