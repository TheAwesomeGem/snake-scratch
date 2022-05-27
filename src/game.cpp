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
        renderer->render_rectangle(1280.0F - 100.0F, 720.0F - 100.0F, 100.F, 100.F, Color { 1.0F, 0.5F, 0.5F, 1.0F });
        renderer->render_rectangle(0.0F, 0.0F, 100.F, 100.F, Color { 0.0F, 0.25F, 1.0F, 1.0F });
    }
}
