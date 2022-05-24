#pragma once

#include "Entity.hpp"

struct PhysicsBox : PhysicsEntity
{
    Vector2 size = {100.0f, 100.0f};
    f32 rotation = 0.f;
    Color color = RAYWHITE;// Color{0xaa, 0xaa, 0xff, 0xff};
    b2World& world;

    void render(f32 dt) override {
        Rectangle rec;
        rec.x = position.x;
        rec.y = position.y;
        rec.width = size.x;
        rec.height = size.y;

        DrawRectanglePro(rec, {rec.width/2, rec.height/2}, body->GetAngle() * 180.0f / 3.1415f, color);
        return;

        //rec.y += 5.0f;
        //DrawRectangleRounded(rec, 0.25f, 8, BLACK);
        //rec.y -= 5.0f;
        DrawRectangleRounded(rec, 0.25f, 8, color);
        
    }
    void save(json& save_file) override {
        save_file["position"] = { position.x , position.y };
        save_file["size"] = { size.x , size.y };
    }
    void load(json& save_file) override {
        auto [px, py] = save_file["position"].get<std::array<float, 2>>();
        auto [sx, sy] = save_file["size"].get<std::array<float, 2>>();
        position = {px,py};
        size = {sx,sy};
        create_physics_body(world);
    }

    void create_physics_body(b2World& world)
    {
        b2BodyDef bodyDef;
        bodyDef.position.Set(position.x * WORLD_TO_BOX, position.y * WORLD_TO_BOX);
        bodyDef.type = b2_dynamicBody;
        body = world.CreateBody(&bodyDef);

        b2PolygonShape shape;
        shape.SetAsBox(size.x * WORLD_TO_BOX / 2.0f, size.y * WORLD_TO_BOX / 2.0f);

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &shape;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.96f;
        fixtureDef.restitution = 0.5f;

        body->CreateFixture(&fixtureDef);
    }

    PhysicsBox(b2World& p_world)
        : world(p_world)
    {
        
    }
};