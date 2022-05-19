#pragma once
#include "raylib.h"

#if __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

struct Window
{
	int width = 640;
	int height = 360;
	std::string name = "Raylib";

	Window& init() {
		InitWindow(width, height, name.c_str());

		#if __EMSCRIPTEN__
			emscripten_set_window_title(name.c_str());
		#endif

		return *this;
	}

	void update() {
		width = GetScreenWidth();
		height = GetScreenHeight();
	}
};