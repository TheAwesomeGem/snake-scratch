cmake_minimum_required(VERSION 3.22)
project(snake_scratch C CXX)

include(FetchContent)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

FetchContent_Declare(
        SDL
        GIT_REPOSITORY "https://github.com/libsdl-org/SDL"
        GIT_TAG "53dea9830964eee8b5c2a7ee0a65d6e268dc78a1" # 2.0.22
)

FetchContent_Declare(
        SDL_image
        GIT_REPOSITORY "https://github.com/libsdl-org/SDL_image"
        GIT_TAG "3396c84c703eda256e7ba4e797703e6827d36bb6" # latest
)

FetchContent_Declare(
        glm
        GIT_REPOSITORY "https://github.com/g-truc/glm"
        GIT_TAG "bf71a834948186f4097caa076cd2663c69a10e1e" # 0.9.9.8
)

set(UUID_USING_CXX20_SPAN ON CACHE BOOL "" FORCE)
FetchContent_Declare(
        stduuid
        GIT_REPOSITORY "https://github.com/mariusbancila/stduuid"
        GIT_TAG "3dca9d816cce4f142e82fe249d71c4f781bb7aee" # latest
)

FetchContent_MakeAvailable(SDL SDL_image glm stduuid)
add_subdirectory(thirdparty/glad)

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
        src/spawner.cpp
        src/spawner.h
        src/ai.cpp
        src/ai.h
        src/input.cpp
        src/input.h)

target_link_libraries(snake_scratch PRIVATE SDL2::SDL2main)
target_link_libraries(snake_scratch PRIVATE SDL2::SDL2)
target_link_libraries(snake_scratch PRIVATE glad)
target_link_libraries(snake_scratch PRIVATE SDL2_image)
target_link_libraries(snake_scratch PRIVATE glm::glm)
target_link_libraries(snake_scratch PRIVATE stduuid)

message("Info: $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/SDL2>")

#set(SOME_VAR "Howdy how are you?")
#set(FIND_STR "how")
#set(REPLACE_STR "who")
#
#string(REPLACE ${FIND_STR} ${REPLACE_STR}  OUT_VAR ${SOME_VAR})
#
#message("String: ${OUT_VAR}")

#set(SOME_LIST hello world stuff)
#list(LENGTH SOME_LIST len)
#list(APPEND SOME_LIST another)
#message("length = ${len}")
#list(GET SOME_LIST 0 1 2 3 SOME_WORDS)
#message("list = ${SOME_WORDS}")
#list(FIND SOME_LIST stuff INDEX)
#message("index = ${INDEX}")

#set(lol 4)
#set(lolo 4)
#
#if(lol LESS_EQUAL lolo)
#    message("It's a set!")
#else()
#    message("It's neither!")
#endif()

#set(SOME_LIST hey now brown cow)
#set(ANOTHER_LIST 1 2 3 4 5)
#list(APPEND SOME_LIST lol)
#list(APPEND ANOTHER_LIST 6)
#foreach(item1 item2 IN ZIP_LISTS SOME_LIST ANOTHER_LIST)
#    message("Item = ${item1}: ${item2}")
#endforeach()
