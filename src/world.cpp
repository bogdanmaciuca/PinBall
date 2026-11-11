#include "world.hpp"

#include "shader_sources.hpp"

World::World() {
    m_marble_shader.Load(MARBLE_VS, MARBLE_FS);
    m_border_shader.Load(BORDER_VS, BORDER_FS);

    m_objects.reserve(1024);
    // Marble
    m_objects.emplace_back(&m_marble_shader, OPT_BALL);
    m_marble_id = m_objects.size() - 1;
    Marble().Load(std::array{
        Vertex{ .pos = glm::vec2(-0.5f, -0.5f) },
        Vertex{ .pos = glm::vec2(+0.5f, -0.5f) },
        Vertex{ .pos = glm::vec2(+0.5f, +0.5f) },
        Vertex{ .pos = glm::vec2(-0.5f, -0.5f) },
        Vertex{ .pos = glm::vec2(+0.5f, +0.5f) },
        Vertex{ .pos = glm::vec2(-0.5f, +0.5f) },
    });
    Marble().size     = glm::vec2(50.0f);
    Marble().color    = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    Marble().velocity = glm::vec2(250.0f, -350.0f);
    // Border
    m_objects.emplace_back(&m_border_shader, OPT_POLY);
    m_border_id = m_objects.size() - 1;
    Border().Load(MakeLinePoly(std::array{
        // Drain
        Vertex{ .pos = glm::vec2(-0.23f,  0.5f) },
        // Left wall
        Vertex{ .pos = glm::vec2(-0.37f,  0.19f) },
        Vertex{ .pos = glm::vec2(-0.37f, -0.21f) },
        // Arch
        Vertex{ .pos = glm::vec2(-0.26f, -0.41f) },
        Vertex{ .pos = glm::vec2( 0.0f,  -0.5f) },
        Vertex{ .pos = glm::vec2( 0.26f, -0.41f) },
        // Right wall
        Vertex{ .pos = glm::vec2( 0.37f, -0.21f) },
        Vertex{ .pos = glm::vec2( 0.37f,  0.19f) },
        // Closing back to start
        Vertex{ .pos = glm::vec2( 0.23f,  0.5f) },
        Vertex{ .pos = glm::vec2(-0.23f,  0.5f) }
    }, 0.005f));
    Border().size  = glm::vec2(500.0f);
    Border().color = glm::vec4(0.3f, 0.9f, 0.4f, 1.0f);
}

void World::UpdateModels() {
    for (Object& obj : m_objects) {
        obj.UpdateModel();
    }
}

void World::Render() {
    for (Object& obj : m_objects) {
        obj.Draw();
    }
    Marble().Draw();
}

