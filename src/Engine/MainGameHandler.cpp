#include "MainGameHandler.hpp"

#include "Game/Ball.hpp"
#include "Game/Hole.hpp"
#include "Game/Wall.hpp"

#include <thread>

bool MainGameHandler::load(const std::string& filename)
{
    ClearBackground(BLACK);
    
        DrawText("Loading", 200, 200, 22, WHITE);

    EndDrawing();

    camera.target = {0.0f, 0.0f};
    camera.offset = {0.0f, 0.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    json j;
    std::ifstream file(S_ASSETS_PATH + "Data/Levels/" + filename);
    file >> j;
    golf_level.load(j);

    return true;
}

void camera_zoom(Camera2D& camera)
{
    std::stringstream ss;

    auto scroll = GetMouseWheelMove();
    camera.zoom = std::max(1.0f, camera.zoom - scroll);
    //std::cout << "scroll: " << scroll << std::endl;

    //return;
    const auto pinch = GetGesturePinchVector();
    static auto last_pinch = pinch; 
    if (last_pinch.x == 0.0f && last_pinch.y == 0.0f) last_pinch = pinch;

    const auto pinch_delta = Vector2Length(last_pinch) - Vector2Length(pinch);
    //ss << "Pinch delta: " << pinch_delta;
    
    if (pinch.x != 0.0f && pinch.y != 0.0f)
    {
        camera.zoom = std::max(1.0f, camera.zoom - pinch_delta/100.0f);
    }

    //DrawText(ss.str().c_str(), 0, 25, 24, WHITE);

    last_pinch = pinch;
}

Ref<GameEventHandler> MainGameHandler::tick(f32 dt)
{


    golf_level.world.Step(1.0f / 60.0f, 6, 2);

    for (const auto& [name, entity] : golf_level.walls)
        entity->update(dt);
    for (const auto& [name, entity] : golf_level.entities)
        entity->update(dt);

    camera_zoom(camera);
    
    if (!ball_in_hole && Vector2Distance(golf_level.entities["player"].get()->position, golf_level.entities["hole"].get()->position) < 12.0f)
    {
        ball_in_hole = true;
        Ball* player = reinterpret_cast<Ball*>(golf_level.entities["player"].get());
        player->body->SetLinearVelocity({0,0});

    #if !__EMSCRIPTEN__
        delay_callback(2.0f, [this](){
    #endif
            this->level_complete = true;
    #if !__EMSCRIPTEN__
        });
    #endif
    }

    return nullptr;
}

Ref<GameEventHandler> MainGameHandler::draw(f32 dt)
{
    const auto player_global = golf_level.entities["player"]->position;
    camera.target = lerp_dt(camera.target, player_global, 0.15f, dt);
    camera.offset = Vector2{window->width/2.0f, window->height/2.0f};
    camera.rotation = 0.0f;


    std::stringstream ss;
    ss << "Camera: < " << camera.target.x << ", " << camera.target.y << " >";

    constexpr int bg_sq_size = 64;
    constexpr int bg_sq_count = 13;
    for (int i = 0; i < bg_sq_count; i += 1)
    {
        for (int j = 0; j < bg_sq_count; j += 1)
        {
            auto x = (2*i + (j%2)) * bg_sq_size;
            auto y = j * bg_sq_size;
            DrawRectangle(
                x, y, 
                bg_sq_size, bg_sq_size, 
                Color{0x22, 0xbb, 0x22, 0xff});
        }
    }
    
    BeginMode2D(camera);

    
    std::vector<Wall*> walls;
    for (const auto& [name, entity] : golf_level.walls)
        walls.push_back(entity.get());
    std::sort(walls.begin(),
            walls.end(),
            [](Wall* a, Wall* b) { 
                return a->position.y + a->size.y < b->position.y + a->size.y;
            });
    for (const auto& wall : walls)
        wall->render(dt);
    for (const auto& [name, entity] : golf_level.entities)
        entity->render(dt);

#if 1
    int bodiesCount = GetPhysicsBodiesCount();
    for (int i = 0; i < bodiesCount; i++)
    {
        PhysicsBody body = GetPhysicsBody(i);

        int vertexCount = GetPhysicsShapeVerticesCount(i);
        for (int j = 0; j < vertexCount; j++)
        {
            // Get physics bodies shape vertices to draw lines
            // Note: GetPhysicsShapeVertex() already calculates rotation transformations
            Vector2 vertexA = GetPhysicsShapeVertex(body, j);

            int jj = (((j + 1) < vertexCount) ? (j + 1) : 0);   // Get next vertex or first to close the shape
            Vector2 vertexB = GetPhysicsShapeVertex(body, jj);

            DrawLineV(vertexA, vertexB, GREEN);     // Draw a line between two vertex positions
        }
    }
#endif

    EndMode2D();

    DrawFPS(0,0);
    DrawText(ss.str().c_str(), 0, 25, 24, BLACK);
    
    const auto level_text_width = MeasureText(TextFormat("Level: %d", golf_level.level), 28);
    DrawText(TextFormat("Level: %d", golf_level.level), window->width - level_text_width - 16,0,28, RAYWHITE);

    DrawText(TextFormat("TouchCount: %d", GetTouchPointCount()), 0, 53, 25, RAYWHITE);

    return nullptr;
}
