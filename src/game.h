// Copyright (c) Topaz Centuallas 2022.

#pragma once

#include "core.h"


namespace Game {
    bool init();

    void poll();

    void update(double fps_delta);

    void render();
}
