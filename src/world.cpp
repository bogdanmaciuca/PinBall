#include "world.hpp"

#include "globals.hpp"
#include "shader_sources.hpp"

namespace {
    constexpr float FLIP_ANGLE       = 0.6f;
    constexpr float FLIPPER_REST_TILT = 0.2;
    constexpr float ANGULAR_SPEED    = 8.0f;
}

World::World() {
    m_marble_shader.Load(MARBLE_VS, MARBLE_FS);
    m_border_shader.Load(BORDER_VS, BORDER_FS);

    std::array quad_vertices{
        Vertex{ .pos = glm::vec2(-0.5f, -0.5f) },
        Vertex{ .pos = glm::vec2(+0.5f, -0.5f) },
        Vertex{ .pos = glm::vec2(+0.5f, +0.5f) },
        Vertex{ .pos = glm::vec2(-0.5f, -0.5f) },
        Vertex{ .pos = glm::vec2(+0.5f, +0.5f) },
        Vertex{ .pos = glm::vec2(-0.5f, +0.5f) },
    };

    m_objects.reserve(1024);
    // Marble
    m_objects.emplace_back(&m_marble_shader, OPT_BALL);
    m_marble_id = m_objects.size() - 1;
    Marble().Load(quad_vertices);
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
    Border().size  = glm::vec2(700.0f);
    Border().color = glm::vec4(0.3f, 0.9f, 0.4f, 1.0f);

    // Flippers
    auto flipper_vertices = [](float direction) {
        return MakeLinePoly(std::array{
            Vertex{ .pos = glm::vec2(0.0f, 0.0f) },
            Vertex{ .pos = glm::vec2(0.20f * direction, 0.0f) },
        }, 0.04f);
    };

    m_objects.emplace_back(&m_border_shader, OPT_POLY);
    m_pad_left_id = m_objects.size() - 1;
    PadLeft().Load(flipper_vertices(1.0f));
    PadLeft().size   = Border().size;
    PadLeft().color  = glm::vec4(0.2f, 0.6f, 1.0f, 1.0f);
    PadLeft().pos    = glm::vec2(-0.23f, 0.42f) * 700.0f;
    PadLeft().center = glm::vec2(0.0f, 0.0f);
    PadLeft().angle  = FLIPPER_REST_TILT;

    m_objects.emplace_back(&m_border_shader, OPT_POLY);
    m_pad_right_id = m_objects.size() - 1;
    PadRight().Load(flipper_vertices(-1.0f));
    PadRight().size   = Border().size;
    PadRight().color  = glm::vec4(0.2f, 0.6f, 1.0f, 1.0f);
    PadRight().pos    = glm::vec2(0.23f, 0.42f) * 700.0f;
    PadRight().center = glm::vec2(0.0f, 0.0f);
    PadRight().angle  = -FLIPPER_REST_TILT;

    // Obstacle balls
    std::array obstacle_positions{
        glm::vec2(0.0f, -120.0f),
        glm::vec2(-100.0f, 40.0f),
        glm::vec2(100.0f, 40.0f),
    };

    for (const glm::vec2& position : obstacle_positions) {
        Object& obstacle = m_objects.emplace_back(&m_marble_shader, OPT_BALL);
        obstacle.Load(quad_vertices);
        obstacle.pos = position;
        obstacle.size = glm::vec2(50.0f);
        obstacle.color = glm::vec4(0.9f, 0.1f, 0.0f, 1.0f);
    }
}

void World::UpdateModels() {
    for (Object& obj : m_objects) {
        obj.UpdateModel();
    }
}

void World::UpdatePads() {
    auto move_toward = [](Object& pad, float target) {
        float old_angle = pad.angle;
        float step = ANGULAR_SPEED * TIMESTEP;
        float diff = target - pad.angle;
        if (glm::abs(diff) <= step) {
            pad.angle = target;
        } else {
            pad.angle += glm::sign(diff) * step;
        }
        pad.angular_velocity = (pad.angle - old_angle) / TIMESTEP;
    };

    bool left_pressed = glfwGetKey(g_window, GLFW_KEY_A) == GLFW_PRESS
        || glfwGetKey(g_window, GLFW_KEY_LEFT) == GLFW_PRESS;
    bool right_pressed = glfwGetKey(g_window, GLFW_KEY_D) == GLFW_PRESS
        || glfwGetKey(g_window, GLFW_KEY_RIGHT) == GLFW_PRESS;

    move_toward(PadLeft(),  left_pressed  ? -FLIP_ANGLE :  FLIPPER_REST_TILT);
    move_toward(PadRight(), right_pressed ?  FLIP_ANGLE : -FLIPPER_REST_TILT);
}

void World::Render() {
    for (Object& obj : m_objects) {
        obj.Draw();
    }
    Marble().Draw();
}

