#pragma once

#include "object.hpp"

struct World {
    Object marble;
    Object paddle_left;
    Object paddle_right;
    Object border;
    std::vector<Object> balls;
    std::vector<Object> triangles;
};

