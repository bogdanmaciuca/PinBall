#pragma once

struct Shader {
    ~Shader();
    bool Load(const char* vertex_shader, const char* fragment_shader);
    void Bind();
    void Set(const char* name, const glm::vec2& value);
    void Set(const char* name, const glm::vec4& value);
    void Set(const char* name, const glm::mat4& value);
private:
    GLuint m_id = 0;
};

