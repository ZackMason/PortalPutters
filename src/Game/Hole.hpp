#pragma once

#include "Entity.hpp"

struct Hole : public Entity
{
    bool update(f32 dt) override {
        
        return false;
    }
    void render(f32 dt) override {
        DrawCircle(position.x, position.y, 8, BLACK);
        DrawCircle(position.x, position.y, 6, DARKGRAY);
    }

    void save(json& save_file) override {
        save_file["class"] = "Hole";
        save_file["position"] = {position.x , position.y };

    }
    void load(json& save_file) override {
        auto [px, py] = save_file["position"].get<std::array<float, 2>>();
        position = {px,py};
    }

    Hole() = default;
    virtual ~Hole() = default;
};