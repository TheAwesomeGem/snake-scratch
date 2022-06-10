// Copyright (c) Topaz Centuallas 2022.

#include "game.h"
#include "renderer.h"
#include "spawner.h"
#include "ai.h"
#include "input.h"
#include "collision.h"
#include "consumption.h"
#include "movement.h"


namespace Game {
    GameApp::~GameApp() {
        deinit();
    }

    bool GameApp::init() {
        init_level(state);

        return true;
    }

    void GameApp::deinit() {
        state.entities.clear();
    }

    void GameApp::input(InputType input, CommandType command) {
        if (input == InputType::KEY_UP) {
            state.input_commands.push_back(command);
        }
    }

    void GameApp::clean_up() {
        for (auto& [entity_id, entity]: state.entities) {
            bool is_dead = !entity.is_alive;

            if (is_dead) {
                if (&entity == &state.player()) {
                    Game::on_player_death(state);
                    break;
                } else {
                    Game::on_entity_death(state, entity);
                }
            }
        }

        std::erase_if(state.entities, [&](const auto& item) {
            return !item.second.is_alive;
        });
    }

    void GameApp::tick() {
        do_ai(state);
        do_collision(state);
        do_consumption(state);
        do_movement(state);
        clean_up();
    }

    static constexpr const double TICK_FREQUENCY = 0.15;

    void GameApp::update(double fps_delta) {
        do_input(state);

        state.time_since_last_tick += fps_delta;

        if (state.time_since_last_tick >= TICK_FREQUENCY) {
            tick();

            state.time_since_last_tick = 0;
        }
    }

    void GameApp::render_cell(Renderer* renderer, int x, int y, Color color) const {
        float offset_x = renderer->width() * 0.5F - (state.level.total_width() * 0.5F);
        float offset_y = renderer->height() * 0.5F - (state.level.total_height() * 0.5F);
        float pixel_x = state.level.from_cell_x(x);
        float pixel_y = state.level.from_cell_y(y);

        renderer->render_rectangle(offset_x + pixel_x, offset_y + pixel_y, state.level.cell_size, state.level.cell_size, color);
    }

    void GameApp::render(Renderer* renderer) {
        for (const auto& [entity_id, entity]: state.entities) {
            if (entity.is_alive) {
                render_cell(renderer, entity.transform.x, entity.transform.y, entity.render.color);
            }
        }

        float offset_x = renderer->width() * 0.5F - (state.level.total_width() * 0.5F);
        float offset_y = renderer->height() * 0.5F - (state.level.total_height() * 0.5F);

        renderer->render_rectangle_border(offset_x, offset_y, state.level.total_width(), state.level.total_height(), Color{1.0F, 0.1F, 0.1F, 1.0F}, 2.0F);
    }
}
