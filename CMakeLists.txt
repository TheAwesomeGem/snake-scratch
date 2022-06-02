cmake_minimum_required(VERSION 3.22)
project(snake_scratch C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(SDL)
add_subdirectory(glad)
add_subdirectory(SDL_image)
add_subdirectory(glm)
add_subdirectory(stduuid)

add_executable(snake_scratch
        src/main.cpp
        src/game.cpp
        src/game.h
        src/core.h
        src/renderer.cpp
        src/renderer.h
        src/movement.h
        src/movement.cpp
        src/collision.cpp
        src/collision.h
        src/game_state.cpp
        src/game_state.h
        src/consumption.cpp
        src/consumption.h
        src/random.h
        )

target_link_libraries(snake_scratch PRIVATE SDL2::SDL2main)
target_link_libraries(snake_scratch PRIVATE SDL2::SDL2)
target_link_libraries(snake_scratch PRIVATE glad)
target_link_libraries(snake_scratch PRIVATE SDL2_image)
target_link_libraries(snake_scratch PRIVATE glm::glm)
target_link_libraries(snake_scratch PRIVATE stduuid)

#set(SOME_VAR "Howdy how are you?")
#set(FIND_STR "how")
#set(REPLACE_STR "who")
#
#string(REPLACE ${FIND_STR} ${REPLACE_STR}  OUT_VAR ${SOME_VAR})
#
#message("String: ${OUT_VAR}")

