#define PHYSAC_IMPLEMENTATION
#define RAYGUI_IMPLEMENTATION
#define PHYSAC_AVOID_TIMMING_SYSTEM
#include "Core.h"


#include "UnityBuild.cpp"


int main()
{
#if __EMSCRIPTEN__
    std::cout << "Built with Emscripten!" << std::endl;
#endif


    SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    Window window;
    window.name = "Grbll Golf";
    window.init();

    SetExitKey(0);
    SetTargetFPS(60);
    
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
        }
        #endif
    }
    CloseWindow();
    
    return 0;
}
