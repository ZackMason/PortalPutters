#pragma once

#include "Entity.hpp"

struct Coin : public Entity
{
    void render(f32 dt) override {
        DrawCircle(position.x, position.y, 8, YELLOW);
    }
    void save(json& save_file) override {
        save_file["class"] = "Coin";
        save_file["position"] = {position.x , position.y };
    }
    void load(json& save_file) override {
        auto [px, py] = save_file["position"].get<std::array<float, 2>>();
        position = {px,py};
    }
};