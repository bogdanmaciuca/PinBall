#pragma once

constexpr const char* BALL_VS = R"GLSL(
    #version 450

    layout (location = 0) in vec2 aPos;
    layout (location = 1) in vec4 aColor;

    uniform mat4 uProj;
    uniform mat4 uModel;

    void main() {
        gl_Position = uProj * uModel * vec4(aPos, 0.0, 1.0);
    }
)GLSL";

constexpr const char* BALL_FS = R"GLSL(
    #version 450

    uniform vec4 uColor;

    out vec4 oFragColor;

    void main() {
        oFragColor = uColor;
    }
)GLSL";

