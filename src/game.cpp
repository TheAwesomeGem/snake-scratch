// Copyright (c) Topaz Centuallas 2022.

#include "game.h"
#include "renderer.h"


namespace Game {
    bool init() {
        return true;
    }

    void input(InputType input, CommandType command) {
    }

    void update(double fps_delta) {

    }

    void render(Renderer* renderer) {
        renderer->render_rectangle(0.5F, 0.5F, 0.25F, 0.25F);
    }
}
