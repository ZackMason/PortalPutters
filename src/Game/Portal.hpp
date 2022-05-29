#pragma once

#include "Core.h"
#include "Entity.hpp"
#include "Ball.hpp"
#include "Util/Timer.hpp"

struct PortalLink : Entity {

    struct Portal
    {
        Vector2 position = {0.0f, 0.0f};
        f32 radius = 8.0f;
        Color color = SKYBLUE;
        bool active = true;
    };

    std::array<Portal, 2> portals;

    void update_portals(f32 dt, Ball* player) {
        int i = 0;
        for (auto& portal : portals)
        {
            if (portal.active && CheckCollisionCircles(portal.position, portal.radius, player->position, player->radius))
            {
                player->body->SetTransform(convert(portals[1-i].position * WORLD_TO_BOX), 0.f);
                portals[1-i].active = false;
                CallbackTimer::create(1.0f, [this, i](){
                    portals[1-i].active = true;
                });
                return;
            }
            i += 1;
        }
    }

    void render(f32 dt) override {
        auto& [p1, p2] = portals;
        auto& [p1_pos, p1_radius, p1_color, p1_active] = p1;
        auto& [p2_pos, p2_radius, p2_color, p2_active] = p2;

        DrawCircleV(p1_pos, p1_radius, p1_active ? p1_color : LIGHTGRAY);
        DrawCircleV(p2_pos, p2_radius, p2_active ? p2_color : LIGHTGRAY);
    }

    void save(json& save_file) override {

    }
    void load(json& save_file) override {
        auto [px, py] = save_file["position"].get<std::array<float, 2>>();
        auto [dstx, dsty] = save_file["dest"].get<std::array<float, 2>>();
        portals[0].position = {px + 8.0f, py + 8.0f};
        portals[1].position = {dstx, dsty};
    }

    PortalLink() {
        portals[1].color = ORANGE;
    }
};