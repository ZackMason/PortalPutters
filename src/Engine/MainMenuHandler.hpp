#pragma once
#include "Core.h"
#include "raylib.h"


struct Window;
struct GameEventHandler
{
	Window* window;
	Ref<GameEventHandler> parent = nullptr;
    
	virtual Ref<GameEventHandler> tick(f32 dt) = 0;
	virtual Ref<GameEventHandler> draw(f32 dt) = 0;
	virtual void clear() const { ClearBackground(BLACK); };
	virtual bool should_quit() const { return false; }
    
	virtual ~GameEventHandler() = default;
};


struct MainMenuEventHandler : GameEventHandler
{
	Ref<GameEventHandler> tick(f32 dt) override;
	Ref<GameEventHandler> draw(f32 dt) override;
};

