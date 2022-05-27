cmake_minimum_required(VERSION 3.22)
project(snake_scratch)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(SDL)
add_subdirectory(glad)
add_subdirectory(SDL_image)
add_subdirectory(glm)

add_executable(snake_scratch src/main.cpp src/game.cpp src/game.h src/core.h src/renderer.cpp src/renderer.h)

target_link_libraries(snake_scratch PRIVATE SDL2::SDL2main)
target_link_libraries(snake_scratch PRIVATE SDL2::SDL2)
target_link_libraries(snake_scratch PRIVATE glad)
target_link_libraries(snake_scratch PRIVATE SDL2_image)
target_link_libraries(snake_scratch PRIVATE glm::glm)
