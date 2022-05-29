#include "MainMenuHandler.hpp"
#include "raylib.h"
//#include "extras/raygui.h"

#include "rlgl.h"
#include "raymath.h"
#include "AssetManager.hpp"
#include "Window.hpp"

#include "MainGameHandler.hpp"
#include "LevelsScreenHandler.hpp"

#include "Util/RayUtil.hpp"

Ref<GameEventHandler> MainMenuEventHandler::draw(f32 dt)
{
	auto button_width = window->width * 0.35f;
	auto button_height = button_width * 0.33f;
	

	static Texture2D background = LoadTexture((S_ASSETS_PATH + "Sprites/checker.png").c_str());
    DrawTextureTiled(background, {0,0,128,128}, {0,0,(f32)window->width, (f32)window->height}, {0,0},0,1.0,GREEN);


	//if (GuiButton({ window->width / 2 - button_width/2, window->height / 3.f , button_width, button_height }, "Play"))
	//	return CreateRef<MainGameHandler>("");
	if (GuiButton({ window->width / 2 - button_width/2, window->height / 3 + 15.0f + button_height, button_width, button_height }, "Levels"))
		return CreateRef<LevelsScreenHandler>();

	//DrawText("Grbll Golf", window->width/2 - MeasureText("Grbll Golf", 26)/2, window->height/4, 26, RAYWHITE);
	RayUtil::DrawShadowText("Portal Putters", window->width/2, window->height/4, 2, 32, RAYWHITE, RayUtil::TextAlignment::CENTER);

	return nullptr;
}

Ref<GameEventHandler> MainMenuEventHandler::tick(f32 dt)
{
	if (parent) parent = nullptr;
	EnableCursor();
    
	return nullptr;
}