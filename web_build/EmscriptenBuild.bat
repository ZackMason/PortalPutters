em++ -std=c++20 -O2 ../src/main.cpp ^
..\..\box2d\src\common\b2_block_allocator.cpp ^
..\..\box2d\src\common\b2_draw.cpp ^
..\..\box2d\src\common\b2_math.cpp ^
..\..\box2d\src\common\b2_settings.cpp ^
..\..\box2d\src\common\b2_stack_allocator.cpp ^
..\..\box2d\src\common\b2_timer.cpp ^
..\..\box2d\src\collision\b2_broad_phase.cpp ^
..\..\box2d\src\collision\b2_chain_shape.cpp ^
..\..\box2d\src\collision\b2_circle_shape.cpp ^
..\..\box2d\src\collision\b2_collide_circle.cpp ^
..\..\box2d\src\collision\b2_collide_edge.cpp ^
..\..\box2d\src\collision\b2_collide_polygon.cpp ^
..\..\box2d\src\collision\b2_collision.cpp ^
..\..\box2d\src\collision\b2_distance.cpp ^
..\..\box2d\src\collision\b2_dynamic_tree.cpp ^
..\..\box2d\src\collision\b2_edge_shape.cpp ^
..\..\box2d\src\collision\b2_polygon_shape.cpp ^
..\..\box2d\src\collision\b2_time_of_impact.cpp ^
..\..\box2d\src\dynamics\b2_body.cpp ^
..\..\box2d\src\dynamics\b2_chain_circle_contact.cpp ^
..\..\box2d\src\dynamics\b2_chain_polygon_contact.cpp ^
..\..\box2d\src\dynamics\b2_circle_contact.cpp ^
..\..\box2d\src\dynamics\b2_contact.cpp ^
..\..\box2d\src\dynamics\b2_contact_manager.cpp ^
..\..\box2d\src\dynamics\b2_contact_solver.cpp ^
..\..\box2d\src\dynamics\b2_distance_joint.cpp ^
..\..\box2d\src\dynamics\b2_edge_circle_contact.cpp ^
..\..\box2d\src\dynamics\b2_edge_polygon_contact.cpp ^
..\..\box2d\src\dynamics\b2_fixture.cpp ^
..\..\box2d\src\dynamics\b2_friction_joint.cpp ^
..\..\box2d\src\dynamics\b2_gear_joint.cpp ^
..\..\box2d\src\dynamics\b2_island.cpp ^
..\..\box2d\src\dynamics\b2_joint.cpp ^
..\..\box2d\src\dynamics\b2_motor_joint.cpp ^
..\..\box2d\src\dynamics\b2_mouse_joint.cpp ^
..\..\box2d\src\dynamics\b2_polygon_circle_contact.cpp ^
..\..\box2d\src\dynamics\b2_polygon_contact.cpp ^
..\..\box2d\src\dynamics\b2_prismatic_joint.cpp ^
..\..\box2d\src\dynamics\b2_pulley_joint.cpp ^
..\..\box2d\src\dynamics\b2_revolute_joint.cpp ^
..\..\box2d\src\dynamics\b2_weld_joint.cpp ^
..\..\box2d\src\dynamics\b2_wheel_joint.cpp ^
..\..\box2d\src\dynamics\b2_world.cpp ^
..\..\box2d\src\dynamics\b2_world_callbacks.cpp ^
..\..\box2d\src\rope\b2_rope.cpp ^
..\raylib\raylib\src\emscripten_build\libraylib.a ^
-I. -I..\src -I..\..\box2d\include -I..\raylib\raylib\src ^
-s USE_GLFW=3 -s ASYNCIFY -DPLATFORM_WEB -s ALLOW_MEMORY_GROWTH=1 ^
-s ASSERTIONS=1 -s SAFE_HEAP=1 -s WASM=1 ^
-s ERROR_ON_UNDEFINED_SYMBOLS=0 -s NO_DISABLE_EXCEPTION_CATCHING ^
-DASSETS_PATH=\"./assets/\" --use-preload-plugins --preload-file assets/ ^
-o game.html

:: ../../box2d/src/collision/b2_circle_shape.cpp ../../box2d/src/collision/b2_polygon_shape.cpp
