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
    constexpr float RESTITUTION = 0.7f;

    void ResolveCollision(Object& marble, glm::vec2 collision_point){
        glm::vec2 normal_vector = glm::normalize(marble.pos - collision_point);
        // glm::vec2 force_modifier = 2 * glm::dot(marble.velocity, normal_vector) * normal_vector;
        normal += normal_vector;
    }

    void CheckBallPolygonCollision(Object &marble, const Object &terrain){
        int vertice_count = terrain.GetWorldVertices().size();
        for (int i = 0; i < vertice_count; i++){
            glm::vec2 closest_pt = closestPointOnSegment(marble.pos, terrain.GetWorldVertices()[i], terrain.GetWorldVertices()[(i+1) % vertice_count]);
            if(marble.size.x / 2 > glm::distance(marble.pos, closest_pt)){

                glm::vec2 d = marble.pos - closest_pt;
                float dist = glm::length(d);
                marble.pos += d / dist * (marble.size.x / 2 - dist);

                ResolveCollision(marble, closest_pt);
                
                if(glm::distance(terrain.GetWorldVertices()[(i+1) % vertice_count], closest_pt) < 0.0001f){
                    i++;
                }
            }
        }
    }

    void CheckBallBallCollision(Object &marble, const Object &terrain){
        if(marble.size.x > glm::distance(marble.pos, terrain.pos)){
            glm::vec2 d = marble.pos - terrain.pos;
            float dist = glm::length(d);
            marble.pos += d / dist * (marble.size.x / 2 + terrain.size.x / 2 - dist);

            ResolveCollision(marble, terrain.pos);
        }
    }

    void CheckCollision(Object& o1, Object& o2) {
        if (o1.physics_type == OPT_BALL && o2.physics_type == OPT_BALL) {
            CheckBallBallCollision(o1, o2);
        }
        else if (o1.physics_type == OPT_BALL && o2.physics_type == OPT_POLY) {
            CheckBallPolygonCollision(o1, o2);
        }
    }

    void Integrate(Object &marble){
        float drag_coeficient = 1.0f;

        if(glm::length(normal) > 0.0001f){
            normal = glm::normalize(normal);
            // float vn = glm::dot(marble.velocity, normal);
            // if (vn < 0.0f) {                      // only if moving into the wall
            //     marble.velocity -= 2.0f * vn * normal;
            // }
        }

        marble.velocity -= (1.0f + RESTITUTION) * glm::dot(marble.velocity, normal) * normal; // new direction

        if (glm::length(marble.velocity) > 0.0001f) {
            marble.velocity -= glm::normalize(marble.velocity) * drag_coeficient * TIMESTEP;
        }

        marble.velocity += GRAVITY * TIMESTEP;

        marble.pos += marble.velocity * TIMESTEP;
    }
}

void World::UpdatePhysics(){
    normal = glm::vec2(0.0f);

    for (size_t i = 0; i < m_objects.size(); i++) {
        if (i == m_marble_id) continue;
        CheckCollision(Marble(), m_objects[i]);
    }

    Integrate(Marble());
}
