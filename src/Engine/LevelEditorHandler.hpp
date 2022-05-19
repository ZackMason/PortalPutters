#pragma once

#include "MainMenuHandler.hpp"

struct LevelEditorHandler : public GameEventHandler
{
    const std::filesystem::path levels_directory{S_ASSETS_PATH + "Data/Levels/"};

    LevelEditorHandler()
    {
    }
    virtual ~LevelEditorHandler()
    {
    }

    bool should_quit() const override
    {
        return IsKeyPressed(KEY_ESCAPE);
    }
    
    Ref<GameEventHandler> tick(f32 dt) override
    {   
        return nullptr;
    }
	Ref<GameEventHandler> draw(f32 dt) override
    {
        const auto screen_width = window->width - 6;
        const auto screen_height = window->height;
        
        const auto columns = std::max(4, screen_width/128);
        const auto margin = 6;

        const f32 button_width = screen_width/columns - margin * 2;
        const f32 button_height = 100 - margin * 2;

        static Vector2 scroll;


        return nullptr;
    }
    
	void clear() const override { ClearBackground(GREEN); };
};