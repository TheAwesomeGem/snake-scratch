// Copyright (c) Topaz Centuallas 2022.

#pragma once

#include "core.h"
#include "renderer.h"


namespace Game {
    enum class InputType {
        KEY_DOWN,
        KEY_UP
    };

    enum class CommandType {
        TURN_LEFT,
        TURN_RIGHT,
        TURN_UP,
        TURN_DOWN
    };


    bool init();

    void input(InputType input, CommandType command);

    void update(double fps_delta);

    void render(Renderer* renderer);
}
