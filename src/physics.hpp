#pragma once

#include "object.hpp"

struct Physics{
    Object marble;
    Object paddle_left;
    Object paddle_right;
    Object border;
    std::vector<Object> balls;
    std::vector<Object> triangles;

    Physics(
        Shader &marble_shader, Shader &paddle_shader, Shader &border_shader, 
        Shader &ball_shader, Shader &triangle_shader, int ball_count, int triangle_count
    );
};

// void PhysicsUpdate(World &world);