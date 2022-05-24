#include "MainGameHandler.hpp"

#include "Game/Ball.hpp"
#include "Game/Hole.hpp"
#include "Game/Wall.hpp"

#include "Util/Timer.hpp"
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

    for (auto i = 0; i < 3; i++)
        golf_level.world.Step(1.0f / 60.0f / 3.0f, 8, 1);

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


        CallbackTimer::create(2.0f, [this](){
            this->level_complete = true;
        });
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

    //DrawTextureTiled(background, {0,0,128,128}, {0, 0,(f32)window->width, (f32)window->height}, {0,0},0,1.0,GREEN);

    BeginMode2D(camera);

    
    auto bx = camera.target.x - window->width/2.0f;
    auto by = camera.target.y - window->height/2.0f;
    bx = std::round(bx / 128.0f) * 128.0f;
    by = std::round(by / 128.0f) * 128.0f;

    DrawTextureTiled(background, {0,0,128,128}, {bx, by,(f32)window->width, (f32)window->height}, {0,0},0,1.0,GREEN);
    
    std::vector<Wall*> walls;
    for (const auto& [name, entity] : golf_level.walls)
        walls.push_back(entity.get());
    std::sort(walls.begin(), walls.end(),
            [](Wall* a, Wall* b) { 
                return a->position.y + a->size.y < b->position.y + a->size.y;
            });
    for (const auto& wall : walls)
        wall->render(dt);
    for (const auto& [name, entity] : golf_level.entities)
        entity->render(dt);

    EndMode2D();

    DrawFPS(0,0);
    DrawText(ss.str().c_str(), 0, 25, 24, BLACK);
    
    const auto level_text_width = MeasureText(TextFormat("Level: %d", golf_level.level), 28);
    DrawText(TextFormat("Level: %d", golf_level.level), window->width - level_text_width - 16,0,28, RAYWHITE);

    DrawText(TextFormat("TouchCount: %d", GetTouchPointCount()), 0, 53, 25, RAYWHITE);

    return nullptr;
}

