#include "globals.hpp"
#include "object.hpp"
#include "world.hpp"

constexpr float RESTITUTION = 0.8f;
constexpr float DRAG_COEF  = 0.9f;

namespace {

    glm::vec2 ClosestPointOnSegment(const glm::vec2& point, const glm::vec2& a, const glm::vec2& b) {
        glm::vec2 ab = b - a;
        float t = glm::dot(point - a, ab) / glm::dot(ab, ab);
        t = glm::clamp(t, 0.0f, 1.0f); // Clamps to the [0, 1] segment range
        return a + t * ab;
    }

    // Specifically for marble
    glm::vec2 s_normal(0.0f);
    glm::vec2 s_surface_velocity(0.0f);
    int       s_collision_count = 0;

    void ResolveCollision(Object& marble, const Object& terrain, glm::vec2 collision_point){
        glm::vec2 normal_vector = glm::normalize(marble.pos - collision_point);
        // glm::vec2 force_modifier = 2 * glm::dot(marble.velocity, normal_vector) * normal_vector;
        s_normal += normal_vector;

        glm::vec2 r = collision_point - terrain.pos;

        glm::vec2 velocity(
            -terrain.angular_velocity * r.y,
            terrain.angular_velocity * r.x
        );

        s_surface_velocity += velocity;
        s_collision_count++;
    }

    bool CheckBallPolygonCollision(Object &marble, const Object &terrain){
        int vertice_count = terrain.GetWorldVertices().size();
        bool collides = false;

        for (int i = 0; i < vertice_count; i++){
            // glm::closestPointOnLine() also works
            glm::vec2 closest_pt = ClosestPointOnSegment(marble.pos, terrain.GetWorldVertices()[i], terrain.GetWorldVertices()[(i+1) % vertice_count]);
            if (marble.size.x / 2 > glm::distance(marble.pos, closest_pt)){

                glm::vec2 d = marble.pos - closest_pt;
                float dist = glm::length(d);
                marble.pos += d / dist * (marble.size.x / 2 - dist);

                ResolveCollision(marble, terrain, closest_pt);

                if(glm::distance(terrain.GetWorldVertices()[(i+1) % vertice_count], closest_pt) < 0.0001f){
                    i++;
                }

                collides = true;
            }
        }
        return collides;
    }

    bool CheckBallBallCollision(Object &marble, const Object &terrain){
        if ((marble.size.x + terrain.size.x) / 2 > glm::distance(marble.pos, terrain.pos)){
            glm::vec2 d = marble.pos - terrain.pos;
            float dist = glm::length(d);
            marble.pos += d / dist * (marble.size.x / 2 + terrain.size.x / 2 - dist);

            ResolveCollision(marble, terrain, terrain.pos);
            return true;
        }

        return false;
    }

    bool CheckCollision(Object& o1, Object& o2) {
        if (o1.physics_type == OPT_BALL && o2.physics_type == OPT_BALL) {
            return CheckBallBallCollision(o1, o2);
        }
        else if (o1.physics_type == OPT_BALL && o2.physics_type == OPT_POLY) {
            return CheckBallPolygonCollision(o1, o2);
        }
        return false;
    }

    void Integrate(Object &marble){
        if(glm::length(s_normal) > 0.0001f){
            s_normal = glm::normalize(s_normal);
            glm::vec2 avg_surface_velocity = s_surface_velocity / (float)s_collision_count;

            glm::vec2 relative_velocity = marble.velocity - avg_surface_velocity;

            float vn = glm::dot(relative_velocity, s_normal);

            if (vn < 0.0f) {
                marble.velocity -= (1.0f + RESTITUTION) * vn * s_normal;
            }
        }
        // marble.velocity -= glm::dot(marble.velocity, normal) * normal; // new direction

        if (glm::length(marble.velocity) > 0.0001f) {
            marble.velocity -= glm::normalize(marble.velocity) * DRAG_COEF * TIMESTEP;
        }

        marble.velocity += GRAVITY * TIMESTEP;

        marble.pos += marble.velocity * TIMESTEP;
    }
}

void World::UpdatePhysics(){
    s_normal = glm::vec2(0.0f);
    s_surface_velocity = glm::vec2(0.0f);
    s_collision_count = 0;

    if (CheckCollision(Marble(), Drain())){
        reset_game = true;
        return;
    }

    for (size_t i = 0; i < m_objects.size(); i++) {
        if (i == m_marble_id || i == m_drain_id) continue;
        if (CheckCollision(Marble(), m_objects[i]) && m_objects[i].physics_type == OPT_BALL) {
            g_score++;
        }
    }

    Integrate(Marble());
}
