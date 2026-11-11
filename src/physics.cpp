#include "globals.hpp"
#include "object.hpp"
#include "world.hpp"

namespace {
    constexpr float RESTITUTION = 1.1f; // Bounciness (1.0 = perfectly elastic)
    constexpr float DRAG        = 0.1f; // Air/rolling resistance

    float BallRadius(const Object& ball) {
        return ball.size.x * 0.5f;
    }

    glm::vec2 ClosestPointOnSegment(glm::vec2 p, glm::vec2 a, glm::vec2 b) {
        glm::vec2 ab = b - a;
        float length_sq = glm::dot(ab, ab);
        if (length_sq < 1e-12f) {
            return a;
        }
        float t = glm::clamp(glm::dot(p - a, ab) / length_sq, 0.0f, 1.0f);
        return a + t * ab;
    }

    // Reflect the ball's velocity off a surface pointing along `normal`,
    // only when the ball is actually moving into it.
    void Reflect(Object& ball, glm::vec2 normal, float restitution) {
        float vn = glm::dot(ball.velocity, normal);
        if (vn < 0.0f) {
            ball.velocity -= (1.0f + restitution) * vn * normal;
        }
    }

    void ResolveBallPolygon(Object& ball, const Object& poly) {
        float radius = BallRadius(ball);
        auto verts = poly.GetWorldVertices();

        for (size_t i = 0; i < verts.size(); i++) {
            glm::vec2 a = verts[i];
            glm::vec2 b = verts[(i + 1) % verts.size()];

            glm::vec2 closest = ClosestPointOnSegment(ball.pos, a, b);
            glm::vec2 delta = ball.pos - closest;
            float dist = glm::length(delta);

            if (dist >= radius) {
                continue;
            }

            glm::vec2 normal = (dist > 1e-6f) ? delta / dist : glm::vec2(0.0f, -1.0f);

            ball.pos = closest + normal * radius;
            Reflect(ball, normal, RESTITUTION);
        }
    }

    void ResolveBallBall(Object& ball, Object& other) {
        float radius_sum = BallRadius(ball) + BallRadius(other);
        glm::vec2 delta = ball.pos - other.pos;
        float dist = glm::length(delta);

        if (dist >= radius_sum || dist < 1e-6f) {
            return;
        }

        glm::vec2 normal = delta / dist;
        ball.pos = other.pos + normal * radius_sum;
        Reflect(ball, normal, RESTITUTION);
    }

    void Integrate(Object& ball) {
        ball.velocity += GRAVITY * TIMESTEP;
        ball.velocity *= (1.0f - DRAG * TIMESTEP);
        ball.pos += ball.velocity * TIMESTEP;
    }
}

void World::UpdatePhysics() {
    Object& ball = Marble();

    Integrate(ball);

    for (Object& obj : m_objects) {
        if (&obj == &ball) {
            continue;
        }
        switch (obj.physics_type) {
            case OPT_BALL: ResolveBallBall(ball, obj);      break;
            case OPT_POLY: ResolveBallPolygon(ball, obj);   break;
        }
    }
}
