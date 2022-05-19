#pragma once

#include "Entity.hpp"

struct Ball : public PhysicsEntity
{
    Camera2D& camera;
    b2World& world;

    f32 radius = 6.0f;
    f32 drag = 0.5f;
    bool clicked = false;
    Vector2 click_point = {0,0};
    bool dragging = false;

    b2Vec2 getTrajectoryPoint(const b2Vec2& startingPosition,const b2Vec2& startingVelocity, float n )
    {
      //velocity and gravity are given per second but we want time step values here
      float t = 1 / 60.0f; // seconds per time step (at 60fps)
      b2Vec2 stepVelocity = t * startingVelocity; // m/s
      b2Vec2 stepGravity = t * t * world.GetGravity(); // m/s/s
  
      return startingPosition + n * stepVelocity + 0.5f * (n*n+n) * stepGravity;
    }

    b2Vec2 get_drag_strength()
    {
        auto mouse = GetScreenToWorld2D(GetMousePosition(), camera);

        auto power = Vector2Distance(mouse, click_point);
        auto dir = Vector2Normalize(click_point - mouse);
        return convert(dir * power * WORLD_TO_BOX);
    }

    void mouse_control () 
    {
        if (GetTouchPointCount() > 1){
            click_point = Vector2Zero();
            clicked = false;
            dragging = false;
            return;
        } 
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            if (!clicked)
            {
                auto mouse = GetScreenToWorld2D(GetMousePosition(), camera);
                
                clicked = true;
                click_point = mouse;
                dragging = true;
            }
        }
        else
        {
            if (dragging)
            {
                // hit the ball

                auto shot = get_drag_strength();
                body->ApplyLinearImpulseToCenter(shot, true);
            }

            click_point = Vector2Zero();
            clicked = false;
            dragging = false;
        }
    }

    bool update(f32 dt) override {
        //std::cout << "vel: " << body->velocity.x << ", " << body->velocity.y << std::endl;

        PhysicsEntity::update(dt);
        
        mouse_control();

        return false;

        //velocity.y += 9.81f * dt;

        //position = position + velocity * dt;


        
        return false;
    }

    void render(f32 dt) override {
        DrawCircle(position.x+1, position.y+1, radius, GRAY);
        if (dragging)
        {
            auto g_mouse = GetScreenToWorld2D(GetMousePosition(), camera);
            auto drag_delta = g_mouse - click_point;


            Vector2 one = {1.0f, 1.0f};
            DrawLineEx(position  + one, position + drag_delta + one, 4, BLACK);
            DrawCircleV(position + drag_delta + one, 2, BLACK);
            
            DrawLineEx(position, position + drag_delta , 4, WHITE);
            DrawCircleV(position + drag_delta, 2, WHITE);

            for (int i = 0; i < -180; i += 30)
            {
                b2Vec2 trajectoryPosition = getTrajectoryPoint( body->GetPosition(), body->GetLinearVelocity() + get_drag_strength(), i);
                DrawCircleV(convert(trajectoryPosition) * BOX_TO_WORLD, 4, WHITE);
            }
        }

        DrawCircle(position.x, position.y, radius, WHITE);

    }
    void save(json& save_file) override {
        save_file["class"] = "Ball";
        save_file["position"] = {position.x , position.y };
    }
    void load(json& save_file) override {
        auto [px, py] = save_file["position"].get<std::array<float, 2>>();
        position = {px,py};
        //body->position = position;
        body->SetTransform(convert(position * WORLD_TO_BOX), 0.0f);
    }
    
    Ball(Camera2D& p_camera, b2World& p_world) : PhysicsEntity(), camera(p_camera), world(p_world)
    {
        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.position.Set(position.x * WORLD_TO_BOX, position.y * WORLD_TO_BOX);
        body = world.CreateBody(&bodyDef);

        b2Shape* shape = new b2CircleShape();
        ((b2CircleShape*)shape)->m_radius = radius * WORLD_TO_BOX;

        b2FixtureDef fixtureDef;
        fixtureDef.density = 1.0f;
        fixtureDef.friction = 0.4f;
        fixtureDef.restitution = 0.60f;
        fixtureDef.shape = shape;

        body->CreateFixture(&fixtureDef);
        
        return;

        //body = CreatePhysicsBodyCircle(position, radius, 1.0f);
        //body->restitution = 1.0f;
        //body->freezeOrient = true;
        //body->staticFriction = 0.0f;
        //body->dynamicFriction = 0.0f;
    }
    virtual ~Ball() = default;
};