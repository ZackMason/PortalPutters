#pragma once

#include "MainMenuHandler.hpp"

struct LevelsScreenHandler : public GameEventHandler
{
    json world_file;
    const std::filesystem::path levels_directory{S_ASSETS_PATH + "Data/Levels/"};
    std::vector<bool> played_level;

    LevelsScreenHandler()
    {
        std::ifstream file(S_ASSETS_PATH + "Data/Levels/ldtk_grbll_golf.ldtk");
        file >> world_file;
        played_level = std::vector<bool>(world_file["levels"].size(), false);
    }
    virtual ~LevelsScreenHandler()
    {
    }

    bool should_quit() const override
    {
        return IsKeyPressed(KEY_ESCAPE) || IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE);
    }
    
    Ref<GameEventHandler> tick(f32 dt) override
    {   
        return nullptr;
    }
	Ref<GameEventHandler> draw(f32 dt) override
    {
        static Texture2D background = LoadTexture((S_ASSETS_PATH + "Sprites/checker.png").c_str());
        DrawTextureTiled(background, {0,0,128,128}, {0,0,(f32)window->width, (f32)window->height}, {0,0},0,1.0,GREEN);

        const auto screen_width = window->width - 6;
        const auto screen_height = window->height;
        
        const auto columns = std::max(4, screen_width/128);
        const auto margin = 6;

        const f32 button_width = screen_width/columns - margin * 2;
        const f32 button_height = 100 - margin * 2;

        Rectangle window{0,0,(f32)screen_width + 6, (f32)screen_height};
        Rectangle content{10, 10, (f32)screen_width - 20 + 6, screen_height*2.0f};
        static Vector2 scroll;
        auto view = GuiScrollPanel(window, content, &scroll);

        int idx = 0;

        for (auto level : world_file["levels"])
        { 
            int i = idx % columns;
            int j = idx / columns;

            if (played_level[idx])
            {
                GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, ColorToInt(SKYBLUE));
                GuiSetStyle(DEFAULT, BASE_COLOR_FOCUSED, ColorToInt(SKYBLUE));
            }
            else{
                GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, ColorToInt(WHITE));
                GuiSetStyle(DEFAULT, BASE_COLOR_FOCUSED, ColorToInt(WHITE));
            }

            if(GuiButton({
                i * button_width + margin + (i * margin*2), 
                j * button_height + margin + (j * margin*2) + scroll.y, 
                button_width, 
                button_height}, level["identifier"].get<std::string>().c_str()))
                {
                    auto level_handler = std::make_shared<MainGameHandler>(level);
                    level_handler->golf_level.level = idx + 1;
                    played_level[idx] = true;
                    return level_handler;
                }

            ++idx;
            
        }
        GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, ColorToInt(WHITE));
        GuiSetStyle(DEFAULT, BASE_COLOR_FOCUSED, ColorToInt(WHITE));

        return nullptr;


        for (auto const& dir_entry : std::filesystem::directory_iterator{levels_directory})
        {
            int i = idx % columns;
            int j = idx / columns;
            auto filename = dir_entry.path().filename().string();

            if(GuiButton({
                i * button_width + margin + (i * margin*2), 
                j * button_height + margin + (j * margin*2) + scroll.y, 
                button_width, 
                button_height}, filename.c_str()))
                {
                    return std::make_shared<MainGameHandler>(filename);
                    return CreateRef<MainGameHandler>(filename);
                }

            ++idx;
        }

        return nullptr;
    }
    
    
	void clear() const override { ClearBackground(GREEN); };
};