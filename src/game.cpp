// Copyright (c) Topaz Centuallas 2022.

#include "game.h"
#include "renderer.h"


namespace Game {

    enum class Direction {
        NORTH,
        SOUTH,
        EAST,
        WEST
    };

    struct Segment {
        // TODO: Implement Segment for Snakes.
    };

    struct Snake {
        static constexpr const Color COLOR{0.0F, 0.0F, 1.0F, 1.0F};

        int x;
        int y;

        Direction direction;
    };

    struct Prey {
        // TODO: Later add movement for prey.

        static constexpr const Color COLOR{0.0F, 1.0F, 0.0F, 1.0F};

        int x;
        int y;
    };

    struct Level {
        int cell_count_x;
        int cell_count_y;
        float cell_padding;
        float cell_size;

        float total_width() {
            return (float) cell_count_x * (cell_size + cell_padding);
        }

        float total_height() {
            return (float) cell_count_y * (cell_size + cell_padding);
        }

        float from_cell_x(int x) {
            return (float) x * (cell_size + cell_padding);
        }

        float from_cell_y(int y) {
            return (float) y * (cell_size + cell_padding);
        }
    };

    struct GameState {
        static constexpr const int MAX_PREY_COUNT = 4;
        static constexpr const int MAX_SNAKE_COUNT = 1;

        Level level;
        Prey preys[MAX_PREY_COUNT];
        size_t prey_count;
        Snake snakes[MAX_SNAKE_COUNT];
        size_t snake_count;
        double accumulated_tick;

        void add_prey(Prey prey) {
            preys[prey_count++] = prey;
        }

        void add_snake(Snake snake) {
            snakes[snake_count++] = snake;
        }

        [[nodiscard]] Snake& player() {
            return snakes[0];
        }
    };

    static GameState state;

    bool init() {
        state.level = Level{40, 20, 2.0F, 24.0F};
        state.add_prey(Prey{2, 2});
        state.add_snake(Snake{1, 1, Direction::EAST});

        return true;
    }

    void input(InputType input, CommandType command) {
        if (input == InputType::KEY_DOWN) {
            switch (command) {
                case CommandType::TURN_LEFT:
                    state.player().direction = Direction::WEST;
                    break;
                case CommandType::TURN_RIGHT:
                    state.player().direction = Direction::EAST;
                    break;
                case CommandType::TURN_UP:
                    state.player().direction = Direction::NORTH;
                    break;
                case CommandType::TURN_DOWN:
                    state.player().direction = Direction::SOUTH;
                    break;
            }
        }
    }

    static constexpr const double TICK_FREQUENCY = 0.5;

    void tick() {
        // TODO: Check wall collision
        // TODO: Check prey collision

        Snake& snake = state.player();

        switch (snake.direction) {
            case Direction::NORTH:
                snake.y += 1;
                break;
            case Direction::SOUTH:
                snake.y -= 1;
                break;
            case Direction::EAST:
                snake.x += 1;
                break;
            case Direction::WEST:
                snake.x -= 1;
                break;
        }
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
            render_cell(renderer, snake.x, snake.y, Snake::COLOR);
        }

        float offset_x = renderer->width() * 0.5F - (state.level.total_width() * 0.5F);
        float offset_y = renderer->height() * 0.5F - (state.level.total_height() * 0.5F);

        renderer->render_rectangle_border(offset_x, offset_y, state.level.total_width(), state.level.total_height(), Color{1.0F, 0.1F, 0.1F, 1.0F}, 5.0F);
    }
}
