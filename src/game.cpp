// Copyright (c) Topaz Centuallas 2022.

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
        state.spawn_snake(1, 1);
        state.spawn_prey(2, 2);
        state.spawn_prey(4, 5);

        return true;
    }

    void input(InputType input, CommandType command) {
        if (input == InputType::KEY_DOWN) {
            switch (command) {
                case CommandType::TURN_LEFT:
                    state.player().transform.direction = Direction::WEST;
                    break;
                case CommandType::TURN_RIGHT:
                    state.player().transform.direction = Direction::EAST;
                    break;
                case CommandType::TURN_UP:
                    state.player().transform.direction = Direction::NORTH;
                    break;
                case CommandType::TURN_DOWN:
                    state.player().transform.direction = Direction::SOUTH;
                    break;
            }
        }
    }

    static constexpr const double TICK_FREQUENCY = 0.5;

    void tick() {
        do_collision(state);
        do_consumption(state);
        do_movement(state);

        // TODO: Clean up all the dead entities here.
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
        for (EntityId i = 0; i < state.entity_count; ++i) {
            const Entity& entity = state.entities[i];

            if (entity.is_alive) {
                render_cell(renderer, entity.transform.x, entity.transform.y, entity.render.color);
            }
        }

        float offset_x = renderer->width() * 0.5F - (state.level.total_width() * 0.5F);
        float offset_y = renderer->height() * 0.5F - (state.level.total_height() * 0.5F);

        renderer->render_rectangle_border(offset_x, offset_y, state.level.total_width(), state.level.total_height(), Color{1.0F, 0.1F, 0.1F, 1.0F}, 2.0F);
    }
}
