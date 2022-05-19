#pragma once
#include "Core.h"

struct AssetManager
{
	using ModelCache = std::unordered_map<std::string, Model>;
	using TextureCache = std::unordered_map<std::string, Texture>;
	using SoundCache = std::unordered_map<std::string, Sound>;
	using ShaderCache = std::unordered_map<std::string, Shader>;
	


	static inline ModelCache model_cache;
	static inline TextureCache texture_cache;
	static inline SoundCache sound_cache;
	static inline ShaderCache shader_cache;

	static Model& get_model(const std::string& filename)
	{
		if (model_cache.count(filename))
			return model_cache[filename];

		std::stringstream fs; fs << ASSETS_PATH << "Models/" << filename;

		std::cout << "Loading: " << fs.str() << std::endl;

		model_cache[filename] = LoadModel(fs.str().c_str());
		return model_cache[filename];
	}
	
	static Texture& get_texture(const std::string& filename)
	{
		if (texture_cache.count(filename))
			return texture_cache[filename];
		std::stringstream fs; fs << ASSETS_PATH << "Textures/" << filename;

		return texture_cache[filename] = LoadTexture(fs.str().c_str());
	}
	
	static Sound& get_sound(const std::string& filename)
	{
		if (sound_cache.count(filename))
			return sound_cache[filename];

		std::stringstream fs; fs << ASSETS_PATH << filename;

		return sound_cache[filename] = LoadSound(fs.str().c_str());
	}
	static Shader& get_shader(const std::string& filename)
	{
		if (shader_cache.count(filename))
			return shader_cache[filename];

		std::stringstream fs; fs << ASSETS_PATH << "Shaders/" << filename << ".fs";
		std::stringstream vs; vs << ASSETS_PATH << "Shaders/" << filename << ".vs";

		return shader_cache[filename] = LoadShader(vs.str().c_str(), fs.str().c_str());
	}
};
