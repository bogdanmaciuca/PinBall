#include "physics.hpp"

namespace {
    bool CheckImpactBallToPoligon(Object &marble, const Object &terrain){
        for (int i = 0; i < terrain.GetWorldVertices().size(); i++){
            glm::vec2 closest_pt = glm::closestPointOnLine(marble.pos, terrain.GetWorldVertices()[i], terrain.GetWorldVertices()[i+1]);
            if(marble.size[0] > glm::distance(marble.pos, closest_pt)){
                return true;
            }
        }
        return false;
    }
    
    bool CheckImpactBallToBall(Object &marble, const Object &terrain){
        for (int i = 0; i < terrain.GetWorldVertices().size(); i++){
            glm::vec2 closest_pt = glm::closestPointOnLine(marble.pos, terrain.GetWorldVertices()[i], terrain.GetWorldVertices()[i+1]);
            if(marble.size[0] > glm::distance(marble.pos, closest_pt)){
                return true;
            }
        }
        return false;
    }
    
    void CalculateImpactBallToPoligon(Object &marble, const Object &terrain){

    }

    void CalculateImpactBallToBall(Object &marble, const Object &terrain){

    }

    void Integrate(Object &marble){

    }
}

void PhysicsUpdate(World &world){
    if (CheckImpactBallToPoligon(world.marble, world.border)){
        CalculateImpactBallToPoligon(world.marble, world.border);
    }


    if (CheckImpactBallToPoligon(world.marble, world.paddle_left)){
        CalculateImpactBallToPoligon(world.marble, world.paddle_left);
    }
    

    if (CheckImpactBallToPoligon(world.marble, world.paddle_right)){
        CalculateImpactBallToPoligon(world.marble, world.paddle_right);
    }

    for (int i = 0; i < world.triangles.size(); i++){
        if(CheckImpactBallToPoligon(world.marble, world.triangles[i])){
            CalculateImpactBallToPoligon(world.marble, world.triangles[i]);
        }
    }

    for (int i = 0; i < world.balls.size(); i++){
        if(CheckImpactBallToBall(world.marble, world.balls[i])){
            CalculateImpactBallToBall(world.marble, world.balls[i]);
        }
    }

    Integrate(world.marble);
}