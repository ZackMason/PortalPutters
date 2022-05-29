#define RAYGUI_IMPLEMENTATION
#include "Core.h"

#include "UnityBuild.cpp"

#include "Util/Timer.hpp"

decltype(CallbackTimer::inst) CallbackTimer::inst;

int main()
{
#if __EMSCRIPTEN__
    std::cout << "Built with Emscripten!" << std::endl;
#endif

    json config;
    std::ifstream(S_ASSETS_PATH + "Data/config.json") >> config;


    SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    Window window;
    window.name = "Portal Putters";
    window.width = config["width"];
    window.height = config["height"];
    window.init();

    
    SetExitKey(0);
    SetTargetFPS(60);

    GuiSetStyle(0, 16, 0x00000016); // set text size
    GuiSetStyle(0, 19, 0x00000000); // set background color to transparent

    
    Ref<GameEventHandler> handler = CreateRef<MainMenuEventHandler>();
    
    while (WindowShouldClose() == false)
    {
        Ref<GameEventHandler> new_handler = nullptr;
        #if !__EMSCRIPTEN__||1
        try {
        #endif
            window.update();
            handler->window = &window;
            
           
            const auto dt = std::min(1.0f, GetFrameTime());
            CallbackTimer::update_timers(dt);

            if (handler->should_quit() && handler->parent)
            {
                handler = handler->parent;
            }
            
            BeginDrawing();
            handler->clear();
            new_handler = handler->tick(dt);
            
            new_handler = new_handler ? new_handler : handler->draw(dt);
            EndDrawing();
            
            if (new_handler && handler->parent != new_handler)
            {
                new_handler->parent = handler;
            }
            handler = new_handler ? new_handler : handler;
        #if !__EMSCRIPTEN__||1
        }
        catch (std::exception & e)
        {
            std::cout << e.what() << std::endl;
            handler = handler->parent;
        }
        #endif
    }
    CloseWindow();
    
    config["width"] = window.width;
    config["height"] = window.height;

    std::ofstream(S_ASSETS_PATH + "Data/config.json") << config;

    return 0;
}
