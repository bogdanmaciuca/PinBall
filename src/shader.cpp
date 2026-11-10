#include "shader.hpp"

Shader::~Shader() {
    if (m_id != 0) {
        glDeleteProgram(m_id);
    }
}

bool Shader::Load(const char* vertex_source, const char* fragment_source) {
    GLuint vertex, fragment;
    int success;
    char info_log[512];

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertex_source, nullptr);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, nullptr, info_log);
        std::println(stderr, "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n{}", info_log);
        glDeleteShader(vertex);
        return false;
    }

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragment_source, nullptr);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, nullptr, info_log);
        std::println(stderr, "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n{}", info_log);
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        return false;
    }

    m_id = glCreateProgram();
    glAttachShader(m_id, vertex);
    glAttachShader(m_id, fragment);
    glLinkProgram(m_id);

    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_id, 512, nullptr, info_log);
        std::println(stderr, "ERROR::SHADER::PROGRAM::LINKING_FAILED\n{}", info_log);
        glDeleteShader(vertex);
        glDeleteShader(fragment);
        glDeleteProgram(m_id);
        m_id = 0;
        return false;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return true;
}

void Shader::Bind() {
    glUseProgram(m_id);
}

void Shader::Set(const char* name, const glm::vec2& value) {
    glUniform2fv(glGetUniformLocation(m_id, name), 1, &value[0]);
}

void Shader::Set(const char* name, const glm::vec4& value) {
    glUniform4fv(glGetUniformLocation(m_id, name), 1, &value[0]);
}

void Shader::Set(const char* name, const glm::mat4& value) {
    glUniformMatrix4fv(glGetUniformLocation(m_id, name), 1, GL_FALSE, &value[0][0]);
}
