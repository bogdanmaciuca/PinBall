#include "globals.hpp"
#include "object.hpp"
#include "world.hpp"

namespace {
    void CheckBallPolygonCollision(Object &marble, const Object &terrain){
        int vertice_count = terrain.GetWorldVertices().size();
        for (int i = 0; i < vertice_count; i++){
            glm::vec2 closest_pt = glm::closestPointOnLine(marble.pos, terrain.GetWorldVertices()[i], terrain.GetWorldVertices()[(i+1) % vertice_count]);
            if(marble.size[0] > glm::distance(marble.pos, closest_pt)){
                ResolveCollision(marble, closest_pt);
            }
        }
    }

    void CheckBallBallCollision(Object &marble, const Object &terrain){
        for (int i = 0; i < terrain.GetWorldVertices().size(); i++){
            if(marble.size[0] > glm::distance(marble.pos, terrain.pos)){
                ResolveCollision(marble, terrain.pos);
            }
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

    void ResolveCollision(Object& marble, glm::vec2 collision_point){
        glm::vec2 normal_vector = glm::normalize(marble.pos - collision_point);
        glm::vec2 force_modifier = 2 * glm::dot(marble.force, normal_vector) * normal_vector;
        marble.new_force -= force_modifier;
    }

    void Integrate(Object &marble){
        marble.force = marble.new_force;

        marble.pos += marble.force * TIMESTEP;
    }
}

void World::UpdatePhysics(){
    for (int i = 0; i < m_objects.size(); i++) {
        if (i == m_marble_id) continue;
        CheckCollision(Marble(), m_objects[i]);
    }

    Integrate(Marble());
}
