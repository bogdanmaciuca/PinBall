#include "globals.hpp"
#include "object.hpp"
#include "world.hpp"

namespace {

    glm::vec2 closestPointOnSegment(const glm::vec2& point, const glm::vec2& a, const glm::vec2& b) {
        glm::vec2 ab = b - a;
        float t = glm::dot(point - a, ab) / glm::dot(ab, ab);
        t = glm::clamp(t, 0.0f, 1.0f); // Clamps to the [0, 1] segment range
        return a + t * ab;
    }

    glm::vec2 normal(0.0f);
    glm::vec2 surface_velocity(0.0f);
    int collision_count = 0;
    constexpr float RESTITUTION = 0.8f;

    void ResolveCollision(Object& marble, const Object& terrain, glm::vec2 collision_point){
        glm::vec2 normal_vector = glm::normalize(marble.pos - collision_point);
        // glm::vec2 force_modifier = 2 * glm::dot(marble.velocity, normal_vector) * normal_vector;
        normal += normal_vector;

        glm::vec2 r = collision_point - terrain.pos;

        glm::vec2 velocity(
            -terrain.angular_velocity * r.y,
            terrain.angular_velocity * r.x
        );

        surface_velocity += velocity;
        collision_count++;
    }

    bool CheckBallPolygonCollision(Object &marble, const Object &terrain){
        int vertice_count = terrain.GetWorldVertices().size();
        bool collides = false;

        for (int i = 0; i < vertice_count; i++){
            glm::vec2 closest_pt = closestPointOnSegment(marble.pos, terrain.GetWorldVertices()[i], terrain.GetWorldVertices()[(i+1) % vertice_count]);
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
        if((marble.size.x + terrain.size.x) / 2 > glm::distance(marble.pos, terrain.pos)){
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
        float drag_coeficient = 1.0f;

        if(glm::length(normal) > 0.0001f){
            normal = glm::normalize(normal);
            glm::vec2 avg_surface_velocity =
            surface_velocity / (float)collision_count;

            glm::vec2 relative_velocity =
                marble.velocity - avg_surface_velocity;

            float vn = glm::dot(relative_velocity, normal);

            if (vn < 0.0f) {
                marble.velocity -= (1.0f + RESTITUTION) * vn * normal;
            }
        }
        // marble.velocity -= glm::dot(marble.velocity, normal) * normal; // new direction

        if (glm::length(marble.velocity) > 0.0001f) {
            marble.velocity -= glm::normalize(marble.velocity) * drag_coeficient * TIMESTEP;
        }

        marble.velocity += GRAVITY * TIMESTEP;

        marble.pos += marble.velocity * TIMESTEP;
    }
}

void World::UpdatePhysics(){
    normal = glm::vec2(0.0f);
    surface_velocity = glm::vec2(0.0f);
    collision_count = 0;

    if (CheckCollision(Marble(), m_objects[m_drain_id])){
        reset_game = true;
        return;
    }

    for (size_t i = 0; i < m_objects.size(); i++) {
        if (i == m_marble_id || i == m_drain_id) continue;
        CheckCollision(Marble(), m_objects[i]);
    }

    Integrate(Marble());
}
