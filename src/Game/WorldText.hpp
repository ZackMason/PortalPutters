#pragma once

#include "Entity.hpp"

struct WorldText : public Entity 
{
    std::string text = "<text>";
    int font_size = 12;
    int offset = 2;

    void render(f32 dt) override
    {
        DrawText(text.c_str(), position.x+offset, position.y+offset, font_size, BLACK);
        DrawText(text.c_str(), position.x, position.y, font_size, WHITE);
    }

    void load(json& save_file) override
    {
        //assert(save_file["class"].get<std::string>() == "WorldText");

        auto [px, py] = save_file["position"].get<std::array<float, 2>>();
        position = {px,py};

        text = save_file["text"].get<std::string>();
        font_size = save_file["font_size"].get<int>();
        offset = save_file["offset"].get<int>();
    }

    void save(json& save_file) override
    {
        save_file["class"] = "WorldText";
        save_file["position"] = {position.x, position.y};
        save_file["text"] = text;
        save_file["font_size"] = font_size;
        save_file["offset"] = offset;
    }
};