#include "MainMenuHandler.hpp"
#include "raylib.h"
//#include "extras/raygui.h"

#include "rlgl.h"
#include "raymath.h"
#include "AssetManager.hpp"
#include "Window.hpp"

#include "MainGameHandler.hpp"
#include "LevelsScreenHandler.hpp"
#include "LevelEditorHandler.hpp"

Ref<GameEventHandler> MainMenuEventHandler::draw(f32 dt)
{
	auto button_width = window->width * 0.35f;
	auto button_height = button_width * 0.33f;

	if (GuiButton({ window->width / 2 - button_width/2, window->height / 3.f , button_width, button_height }, "Play"))
		return CreateRef<MainGameHandler>("");
	if (GuiButton({ window->width / 2 - button_width/2, window->height / 3 + 15.0f + button_height, button_width, button_height }, "Levels"))
		return CreateRef<LevelsScreenHandler>();
	if (GuiButton({ window->width / 2 - button_width/2, window->height / 3 + 30.0f + button_height * 2, button_width, button_height }, "Level Editor"))
		return CreateRef<LevelEditorHandler>();

	DrawText("Grbll Golf", window->width/2 - MeasureText("Grbll Golf", 26)/2, window->height/4, 26, RAYWHITE);
    
	return nullptr;
}

Ref<GameEventHandler> MainMenuEventHandler::tick(f32 dt)
{
	if (parent) parent = nullptr;
	EnableCursor();
    
	return nullptr;
}