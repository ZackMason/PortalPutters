#pragma once

#include "Core.h"
#include "Entity.hpp"
#include "Ball.hpp"
#include "Wall.hpp"
#include "Hole.hpp"
#include "Coin.hpp"
#include "WorldText.hpp"
#include "Portal.hpp"
#include "PhysicsBox.hpp"

#include "box2d/box2d.h"

struct GolfLevel : Serializable
{
    Camera2D& camera;
    u32 level = 0;
    std::unordered_map<std::string, Unique<Entity>> entities;
    std::unordered_map<std::string, Unique<Wall>> walls;
    std::unordered_map<std::string, Unique<PortalLink>> portals;
    
    b2World world{{0.0f, 0.0f}};

    void render(f32 dt) {
        for (auto& [name, portal] : portals)
        {
            portal->render(dt);
        }
    }

    void update(f32 dt) {
        for (auto& [name, portal] : portals)
        {
            portal->update_portals(dt, (Ball*)entities["player"].get());
        }
    }

    void save(json& save_file)
    {
        save_file["level"] = level;
        for (const auto& [name, entity] : entities)
        {
            entity->save(save_file["entities"][name]);
        }
    }

    void load(json& save_file)
    {
        load_custom(save_file);
    }

    void load_ldtk(json& save_file)
    {
        for (auto& level_variables : save_file["fieldInstances"])
        {
            if (level_variables["__identifier"] == "gravity")
                world.SetGravity({0, level_variables["__value"].get<f32>()});
            if (level_variables["__identifier"] == "start_zoom")
                camera.zoom = level_variables["__value"].get<f32>();
        }

        auto layers = save_file["layerInstances"];
        for (auto& layer : layers)
        {
            if (layer["__identifier"] == "Entities")
            {
			    auto layer_entities = layer["entityInstances"];
                for (auto& entity : layer_entities)
                {
                    json j;
                    j["position"] = entity["px"].get<std::array<f32, 2>>();

                    if (entity["__identifier"] == "Player")
                    {
                        entities["player"] =(CreateUnique<Ball>(camera, world));
                        entities["player"]->load(j);
                    }
                    else if (entity["__identifier"] == "Wall")
                    {
                        std::string name = std::to_string(walls.size());
                        walls[name] =(CreateUnique<Wall>(world));
                        j["size"] = std::array<f32,2>{entity["width"].get<f32>(), entity["height"].get<f32>()};
                        j["position"][0] = j["position"][0].get<f32>() + j["size"][0].get<f32>()/2;
                        j["position"][1] = j["position"][1].get<f32>() + j["size"][1].get<f32>()/2;
                        walls[name]->load(j);
                    }
                    else if (entity["__identifier"] == "PhysicsBox")
                    {
                        static int box_count = 0;
                        std::string name = "box" + std::to_string(box_count++);
                        entities[name] =(CreateUnique<PhysicsBox>(world));
                        j["size"] = std::array<f32,2>{entity["width"].get<f32>() - 2, entity["height"].get<f32>() - 2};
                        j["position"][0] = j["position"][0].get<f32>() + j["size"][0].get<f32>()/2;
                        j["position"][1] = j["position"][1].get<f32>() + j["size"][1].get<f32>()/2;
                        entities[name]->load(j);
                    }
                    else if (entity["__identifier"] == "Hole")
                    {
                        entities["hole"] = (CreateUnique<Hole>());
                        entities["hole"]->load(j);
                    }
                    else if (entity["__identifier"] == "PortalLink")
                    {
                        static int portal_count = 0;
                        std::string name = "portal" + std::to_string(portal_count++);
                        portals[name] = (CreateUnique<PortalLink>());
                        for (auto field : entity["fieldInstances"])
                        {
                            if (field["__identifier"] == "dest")
                            {
                                j["dest"][0] = field["__value"]["cx"].get<float>() * 16.0f + 8.0f;
                                j["dest"][1] = field["__value"]["cy"].get<float>() * 16.0f + 8.0f;
                            }
                        }
                        portals[name]->load(j);
                    }
                    //else if (entity["__identifier"] == "Coin")
                    //{
                    //    entities[name] =(CreateUnique<Coin>());
                    //    entities[name]->load(j);
                    //}
                    else if (entity["__identifier"] == "WorldText")
                    {
                        std::string name;
                        for (auto field : entity["fieldInstances"])
                        {
                            if (field["__identifier"] == "name")
                                name = field["__value"].get<std::string>();
                            else if (field["__identifier"] == "text")
                                j["text"] = field["__value"].get<std::string>();
                            else if (field["__identifier"] == "font_size")
                                j["font_size"] = field["__value"].get<int>();
                            else if (field["__identifier"] == "offset")
                                j["offset"] = field["__value"].get<int>();
                        }
                        entities[name] =(CreateUnique<WorldText>());
                        entities[name]->load(j);
                    }
                }
            }
        }

        using namespace std::string_literals;
        for (const auto& name : {"player"s, "hole"s})
        {
            if (entities.count(name) == 0)
            {
                throw std::runtime_error("Level does not contain: " + name);
            }
        }

    }

    void load_custom(json& save_file)
    {
        level = save_file["level"];

        if(save_file.contains("gravity"))
        {
            auto [gx, gy] = save_file["gravity"].get<std::array<f32, 2>>();
            world.SetGravity({gx, gy});
        }

        for (auto& [name,j] : save_file["entities"].items())
        {
            std::cout << name << ": " << j << std::endl;
            if (j["class"] == "Ball")
            {
                entities[name] =(CreateUnique<Ball>(camera, world));
                entities[name]->load(j);
            }
            else if (j["class"] == "Wall")
            {
                walls[name] =(CreateUnique<Wall>(world));
                walls[name]->load(j);
            }
            else if (j["class"] == "Hole")
            {
                entities[name] =(CreateUnique<Hole>());
                entities[name]->load(j);
            }
            else if (j["class"] == "Coin")
            {
                entities[name] =(CreateUnique<Coin>());
                entities[name]->load(j);
            }
            else if (j["class"] == "WorldText")
            {
                entities[name] =(CreateUnique<WorldText>());
                entities[name]->load(j);
            }
        }
        std::cout << "Loaded " << entities.size() << " entities" << std::endl;
    }

    GolfLevel(Camera2D& p_camera) : camera(p_camera) {};
    virtual ~GolfLevel()
    {
        
    }
};