#pragma once

#include "object.hpp"
#include "shader.hpp"

struct World {
    World();
    void UpdateModels();
    void UpdatePhysics();
    void Render();
private:
    Shader m_marble_shader;
    Shader m_border_shader;

    uint32_t m_marble_id;
    uint32_t m_pad_left_id;
    uint32_t m_pad_right_id;
    uint32_t m_border_id;
    std::vector<Object> m_objects;

    Object& Marble() { return m_objects[m_marble_id]; };
    Object& PadLeft() { return m_objects[m_pad_left_id]; };
    Object& PadRight() { return m_objects[m_pad_right_id]; };
    Object& Border() { return m_objects[m_border_id]; };
};

