#pragma once

struct Shader {
    Shader() = default;
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;
    ~Shader();
    bool Load(const char* vertex_shader, const char* fragment_shader);
    void Bind();
    void Cleanup();
    void Set(const char* name, const glm::vec2& value);
    void Set(const char* name, const glm::vec4& value);
    void Set(const char* name, const glm::mat4& value);
private:
    GLuint m_id = 0;
};

