#include "text.hpp"

#include <cstdarg>
#include <cstdio>

#include "object.hpp"
#include "shader_sources.hpp"
#include "stb_truetype.h"

constexpr const char* FONT_PATH  = "res/font.ttf";
constexpr float       FONT_SIZE  = 32.0f;
constexpr int         ATLAS_W    = 512;
constexpr int         ATLAS_H    = 512;
constexpr int         FIRST_CHAR = 32;
constexpr int         CHAR_COUNT = 95;

constexpr float LINE_HEIGHT    = 40.0f;
constexpr float LETTER_SPACING = 2.0f;

namespace {
    std::vector<unsigned char>              s_font_data;
    std::vector<unsigned char>              s_atlas;
    std::array<stbtt_bakedchar, CHAR_COUNT> s_baked_chars;
    GLuint s_atlas_texture = 0;

    Shader s_text_shader;
    Object s_glyph_object;
}

void InitFont() {
    std::ifstream file(FONT_PATH, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::println(stderr, "ERROR::FONT::FAILED_TO_OPEN {}", FONT_PATH);
        return;
    }

    auto size = file.tellg();
    file.seekg(0, std::ios::beg);
    s_font_data.resize(size);
    file.read(reinterpret_cast<char*>(s_font_data.data()), size);

    s_atlas.resize(ATLAS_W * ATLAS_H);
    stbtt_BakeFontBitmap(
        s_font_data.data(), 0,
        FONT_SIZE,
        s_atlas.data(), ATLAS_W, ATLAS_H,
        FIRST_CHAR, CHAR_COUNT,
        s_baked_chars.data()
    );

    glGenTextures(1, &s_atlas_texture);
    glBindTexture(GL_TEXTURE_2D, s_atlas_texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, ATLAS_W, ATLAS_H, 0, GL_RED, GL_UNSIGNED_BYTE, s_atlas.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);

    s_text_shader.Load(TEXT_VS, TEXT_FS);
    s_glyph_object = Object(&s_text_shader);
    s_glyph_object.size   = glm::vec2(1.0f);
    s_glyph_object.pos    = glm::vec2(0.0f);
    s_glyph_object.center = glm::vec2(0.0f);
    s_glyph_object.angle  = 0.0f;
}

void ShutdownFont() {
    s_glyph_object.Cleanup();
    if (s_atlas_texture != 0) {
        glDeleteTextures(1, &s_atlas_texture);
        s_atlas_texture = 0;
    }
    s_text_shader.Cleanup();
}

void DrawText(glm::vec2 pos, glm::vec4 color, const char* fmt, ...) {
    static char buffer[1024];

    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, s_atlas_texture);

    float pen_x = pos.x;
    float pen_y = pos.y;

    for (const char* c = buffer; *c != '\0'; c++) {
        if (*c == '\n') {
            pen_x = pos.x;
            pen_y += LINE_HEIGHT;
            continue;
        }
        if (*c < FIRST_CHAR || *c >= FIRST_CHAR + CHAR_COUNT) {
            continue;
        }

        const stbtt_bakedchar& b = s_baked_chars[*c - FIRST_CHAR];

        float width  = b.x1 - b.x0;
        float height = b.y1 - b.y0;

        float left   = pen_x + b.xoff;
        float top    = pen_y + b.yoff;
        float right  = left + width;
        float bottom = top + height;

        float u0 = b.x0 / (float)ATLAS_W;
        float u1 = b.x1 / (float)ATLAS_W;
        float v0 = b.y0 / (float)ATLAS_H;
        float v1 = b.y1 / (float)ATLAS_H;

        std::array<Vertex, 6> quad{{
            { glm::vec2(left,  top),    color, glm::vec2(u0, v0) },
            { glm::vec2(right, top),    color, glm::vec2(u1, v0) },
            { glm::vec2(right, bottom), color, glm::vec2(u1, v1) },
            { glm::vec2(left,  top),    color, glm::vec2(u0, v0) },
            { glm::vec2(right, bottom), color, glm::vec2(u1, v1) },
            { glm::vec2(left,  bottom), color, glm::vec2(u0, v1) },
        }};

        s_glyph_object.Load(quad);
        s_glyph_object.Draw();

        pen_x += b.xadvance + LETTER_SPACING;
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}

