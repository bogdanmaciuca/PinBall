#pragma once

void InitFont();
void ShutdownFont();
void DrawText(glm::vec2 pos, glm::vec4 color, const char* fmt, ...);

