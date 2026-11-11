#include "world.hpp"

#include "shader_sources.hpp"

World::World() {
    m_marble_shader.Load(MARBLE_VS, MARBLE_FS);

    m_objects.reserve(1024);
    m_objects.emplace_back(&m_marble_shader, OPT_BALL);
    m_marble_id = m_objects.size() - 1;
}

void World::UpdateModels() {
    for (Object& obj : m_objects) {
        obj.UpdateModel();
    }
}

void World::UpdatePhysics() {
}

void World::Render() {
    for (Object& obj : m_objects) {
        obj.Draw();
    }
}

