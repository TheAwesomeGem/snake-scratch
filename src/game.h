// Copyright (c) Topaz Centuallas 2022.

#pragma once

#include "core.h"
#include "game_state.h"


namespace Game {
    struct GameApp {
        ~GameApp();

        bool init();

        void deinit();

        void input(InputType input, CommandType command);

        void update(double fps_delta);

        void render(struct Renderer* renderer);

    private:
        void clean_up();

        void tick();

        void render_cell(Renderer* renderer, int x, int y, Color color) const;

    private:
        GameState state;
    };
}
