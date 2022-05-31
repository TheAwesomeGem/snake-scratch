// Copyright (c) Topaz Centuallas 2022.

#include <cassert>
#include "game.h"
#include "renderer.h"
#include "collision.h"
#include "movement.h"
#include "game_state.h"
#include "consumption.h"


namespace Game {
    static GameState state; // TODO: Figure out how to get rid of this global state

    bool init() {
        state.level = Level{40, 20, 2.0F, 24.0F};
        state.add_prey(Prey{2, 2});
        state.add_snake(Snake{1, 1});

        init_consumption(state);

        return true;
    }

    void input(InputType input, CommandType command) {
        if (input == InputType::KEY_DOWN) {
            switch (command) {
                case CommandType::TURN_LEFT:
                    state.player().move_direction = Direction::WEST;
                    break;
                case CommandType::TURN_RIGHT:
                    state.player().move_direction = Direction::EAST;
                    break;
                case CommandType::TURN_UP:
                    state.player().move_direction = Direction::NORTH;
                    break;
                case CommandType::TURN_DOWN:
                    state.player().move_direction = Direction::SOUTH;
                    break;
            }
        }
    }

    static constexpr const double TICK_FREQUENCY = 0.5;

    void tick() {
        do_collision(state);
        do_movement(state);
    }

    void update(double fps_delta) {
        state.accumulated_tick += fps_delta;

        if (state.accumulated_tick >= TICK_FREQUENCY) {
            tick();

            state.accumulated_tick = 0;
        }
    }

    void render_cell(Renderer* renderer, int x, int y, Color color) {
        float offset_x = renderer->width() * 0.5F - (state.level.total_width() * 0.5F);
        float offset_y = renderer->height() * 0.5F - (state.level.total_height() * 0.5F);
        float pixel_x = state.level.from_cell_x(x);
        float pixel_y = state.level.from_cell_y(y);

        renderer->render_rectangle(offset_x + pixel_x, offset_y + pixel_y, state.level.cell_size, state.level.cell_size, color);
    }

    void render(Renderer* renderer) {
        for (size_t i = 0; i < state.prey_count; ++i) {
            const Prey& prey = state.preys[i];
            render_cell(renderer, prey.x, prey.y, Prey::COLOR);
        }

        for (size_t i = 0; i < state.snake_count; ++i) {
            const Snake& snake = state.snakes[i];

            // TODO: Submit a queue of segment to be used here to render instead of this garbage slow loop
            for (int y = 0; y < state.level.cell_count_y; ++y) {
                for (int x = 0; x < state.level.cell_count_x; ++x) {
                    if (snake.is_segment(x, y)) {
                        render_cell(renderer, x, y, Snake::COLOR);
                    }
                }
            }
        }

        float offset_x = renderer->width() * 0.5F - (state.level.total_width() * 0.5F);
        float offset_y = renderer->height() * 0.5F - (state.level.total_height() * 0.5F);

        renderer->render_rectangle_border(offset_x, offset_y, state.level.total_width(), state.level.total_height(), Color{1.0F, 0.1F, 0.1F, 1.0F}, 2.0F);
    }
}
