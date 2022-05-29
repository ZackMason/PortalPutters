#pragma once

#include "Core.h"

namespace RayUtil {

    enum class TextAlignment{
        LEFT, RIGHT, CENTER
    };

    void DrawShadowText(const char* text, 
        int x, int y, int offset, 
        int font_size, const Color& color, 
        TextAlignment alignment = TextAlignment::CENTER, const Color& bg_color = BLACK)
    {
        int text_offset_x = 0;
        switch (alignment)
        {
        case TextAlignment::RIGHT:
            text_offset_x = -MeasureText(text, font_size);
            break;
        case TextAlignment::CENTER:
            text_offset_x = -MeasureText(text, font_size) / 2;
            break;
        default:
            break;
        }
        DrawText(text, x + text_offset_x + offset, y + offset, font_size, bg_color);
        DrawText(text, x + text_offset_x, y, font_size, color);
    }
};

