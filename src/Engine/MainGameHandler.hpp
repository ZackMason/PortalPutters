#pragma once

#include "MainMenuHandler.hpp"
#include "Game/Entity.hpp"
#include "Game/GolfLevel.hpp"
#include "box2d/box2d.h"

struct MainGameHandler : GameEventHandler
{
    explicit MainGameHandler(json& save_file)
    {
        camera.target = {0.0f, 0.0f};
        camera.offset = {0.0f, 0.0f};
        camera.rotation = 0.0f;
        camera.zoom = 1.0f;
        golf_level.load_ldtk(save_file);

        background = LoadTexture((S_ASSETS_PATH + "Sprites/checker.png").c_str());
    }
    explicit MainGameHandler(const std::string& file_name = "")
    {
        if (file_name != "")
        {
            std::cout << "loading " << file_name << std::endl;
            load(file_name);
        }
        else
        {
            json config;
            std::ifstream config_file(S_ASSETS_PATH + "Data/config.json");
            config_file >> config;
            load(config["level"].get<std::string>());
        }
    }
    virtual ~MainGameHandler()
    {
        json j;
        golf_level.save(j);

        std::ofstream file(S_ASSETS_PATH + "Data/Saves/save.json");
        file << std::setw(4) << j << std::endl;

        std::cout << "File saved: " << golf_level.entities.size() << " entities, " << (file.is_open() ? "opened" : "closed") << std::endl;
        file.close();
    }
    
    Texture2D background;
    
    Ref<GameEventHandler> tick(f32 dt) override;
	Ref<GameEventHandler> draw(f32 dt) override;
    
    
    Camera2D camera;
    mutable GolfLevel golf_level{camera};

    bool level_complete = false;
    bool ball_in_hole = false;

    bool load(const std::string& filename);
    
	void clear() const override { ClearBackground(GREEN); };
    bool should_quit() const override { 
        return IsKeyPressed(KEY_ESCAPE) || 
        IsMouseButtonDown(MOUSE_BUTTON_MIDDLE) ||
        level_complete;
        //Vector2Distance(golf_level.entities["player"].get()->position, golf_level.entities["hole"].get()->position) < 12.0f; 
    };
};