#pragma once

constexpr const char* MARBLE_VS = R"GLSL(
    #version 450

    layout (location = 0) in vec2 aPos;
    layout (location = 1) in vec4 aColor;

    uniform mat4 uProj;
    uniform mat4 uModel;

    out vec2 FragPos;

    void main() {
        gl_Position = uProj * uModel * vec4(aPos, 0.0, 1.0);
        FragPos = aPos;
    }
)GLSL";

constexpr const char* MARBLE_FS = R"GLSL(
    #version 450

    in vec2 FragPos;

    uniform vec4 uColor;

    out vec4 oFragColor;

    void main() {
        // This requires that the vertices do not go over +-0.5f!
        if (length(FragPos) >= 0.5) discard;
        float color_coef = 1 - pow(length(1 * FragPos), 2);
        oFragColor = vec4(uColor.xyz * color_coef, 1.0);
    }
)GLSL";

constexpr const char* BORDER_VS = R"GLSL(
    #version 450

    layout (location = 0) in vec2 aPos;
    layout (location = 1) in vec4 aColor;

    uniform mat4 uProj;
    uniform mat4 uModel;

    void main() {
        gl_Position = uProj * uModel * vec4(aPos, 0.0, 1.0);
    }
)GLSL";

constexpr const char* BORDER_FS = R"GLSL(
    #version 450

    uniform vec4 uColor;

    out vec4 oFragColor;

    void main() {
        oFragColor = uColor;
    }
)GLSL";

