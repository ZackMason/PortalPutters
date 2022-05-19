cmake_minimum_required(VERSION 3.15)
project(my_project C CXX)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # or games

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

file(GLOB src_files
	"src/main.cpp"
)

if (EMSCRIPTEN)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -O3 -Wall ..\\raylib\\raylib\\src\\emscripten_build\\libraylib.a -I. -I..\\src  -I. -I..\\raylib\\raylib\\src -L. -L..\\raylib\\raylib\\src\\emscripten_build\\libraylib.a -s USE_GLFW=3 -s ASYNCIFY -DPLATFORM_WEB  -s USE_GLFW=3 -s ALLOW_MEMORY_GROWTH=1 -s ASSERTIONS=1 -s SAFE_HEAP=1 -s WASM=1 -s ASYNCIFY -s USE_BULLET=1 --use-preload-plugins --preload-file assets/")
    #set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s USE_GLFW=3 -s ALLOW_MEMORY_GROWTH=1 -s ASSERTIONS=1 -s WASM=1 -s ASYNCIFY -s USE_BULLET=1")
    set(CMAKE_EXECUTABLE_SUFFIX ".html") # This line is used to set your executable to build with the emscripten html template so that you can directly open it.


else()
    #file(GLOB_RECURSE src_files ${PROJECT_SOURCE_DIR}/src/*.cpp)

endif ()

add_executable(${PROJECT_NAME} ${src_files})


add_subdirectory(raylib/raylib)

include_directories(raylib/raylib/src)
include_directories(src)


target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
target_link_libraries(${PROJECT_NAME} PUBLIC ${CONAN_LIBS})
target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/")

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)


file(COPY "assets/" DESTINATION "${CMAKE_BINARY_DIR}/assets/")


