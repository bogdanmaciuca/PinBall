#include "world.hpp"

#include "object.hpp"

namespace {
    bool CheckBallPolygonCollision(Object &marble, const Object &terrain){
        for (int i = 0; i < terrain.GetWorldVertices().size(); i++){
            glm::vec2 closest_pt = glm::closestPointOnLine(marble.pos, terrain.GetWorldVertices()[i], terrain.GetWorldVertices()[i+1]);
            if(marble.size[0] > glm::distance(marble.pos, closest_pt)){
                return true;
            }
        }
        return false;
    }

    bool CheckBallBallCollision(Object &marble, const Object &terrain){
        for (int i = 0; i < terrain.GetWorldVertices().size(); i++){
            glm::vec2 closest_pt = glm::closestPointOnLine(marble.pos, terrain.GetWorldVertices()[i], terrain.GetWorldVertices()[i+1]);
            if(marble.size[0] > glm::distance(marble.pos, closest_pt)){
                return true;
            }
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
    }

    void ResolveBallPolygonCollision(Object &marble, const Object &terrain){

    }

    void ResolveBallBallCollision(Object &marble, const Object &terrain){

    }

    void ResolveCollision(Object& o1, Object& o2) {
        if (o1.physics_type == OPT_BALL && o2.physics_type == OPT_BALL) {
            ResolveBallBallCollision(o1, o2);
        }
        else if (o1.physics_type == OPT_BALL && o2.physics_type == OPT_POLY) {
            ResolveBallPolygonCollision(o1, o2);
        }
    }

    void Integrate(Object &marble){

    }
}

void World::UpdatePhysics(){
    for (int i = 0; i < m_objects.size(); i++) {
        if (i == m_marble_id) continue;
        ResolveCollision(Marble(), m_objects[i]);
    }

    Integrate(Marble());
}
