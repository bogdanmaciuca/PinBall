#include "physics.hpp"

Physics::Physics(
    Shader &marble_shader, Shader &paddle_shader, 
    Shader &border_shader, Shader &ball_shader,
    Shader &triangle_shader, int ball_count, int triangle_count
) : marble(marble_shader), paddle_left(paddle_shader), 
paddle_right(paddle_shader), border(border_shader){

    while(ball_count){
        balls.emplace_back(ball_shader);
        ball_count--;
    }

    while(triangle_count){
        triangles.emplace_back(triangle_shader);
        triangle_count--;
    }
}

void CheckImpact();
void ClaculateImpact();
void Integrate();


// void PhysicsUpdate(World &world);