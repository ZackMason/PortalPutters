#pragma once

#include "Core.h"

#include "box2d/box2d.h"

struct Entity : Serializable
{
    Vector2 position = Vector2{0.0f, 0.0f};

    virtual void on_destroy() {};
    virtual void render(f32 dt) {};
    // return true on kill
    virtual bool update(f32 dt) { return false; };

    virtual ~Entity() = default;
};

struct PhysicsEntity : Entity
{
    b2Body* body = nullptr;

    virtual bool update(f32 dt) 
    { 
        assert(body);
        position = convert(body->GetPosition()) * BOX_TO_WORLD;
        return false; 
    };


    virtual ~PhysicsEntity()
    {
    }
    
};