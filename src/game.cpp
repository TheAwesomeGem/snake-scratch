// Copyright (c) Topaz Centuallas 2022.

#include <cassert>
#include "game.h"
#include "renderer.h"


// TODO: Large goal. Figure out how to separate data from logic. They are too encapsulated and making things harder to manage. This is the Core Architecture!

namespace Game {

    enum class Direction {
        NORTH,
        SOUTH,
        EAST,
        WEST
    };

    struct Segment {
        int x;
        int y;
    };

    struct Snake {
        static constexpr const Color COLOR{0.0F, 0.0F, 1.0F, 1.0F};
        static constexpr const size_t MAX_SEGMENT_COUNT = 12;

        Direction direction;
        size_t segment_count;
        int x;
        int y;
        Segment segments[MAX_SEGMENT_COUNT];

        void init(int new_x, int new_y) {
            x = new_x;
            y = new_y;
            segments[segment_count++] = Segment{new_x, new_y};
        }

        void add_segment() {
            assert(segment_count < MAX_SEGMENT_COUNT - 1 || segment_count > 0);

            const Segment& prev_segment = segments[segment_count - 1];
            Segment segment{prev_segment.x, prev_segment.y};
            segments[segment_count++] = segment;
        }

        void set_location() {
            for (size_t i = segment_count - 1; i > 0; --i) {
                Segment& previous = segments[i];
                Segment& next = segments[i - 1];

                previous.x = next.x;
                previous.y = next.y;
            }

            Segment& head = segments[0];
            head.x = x;
            head.y = y;
        }

        // TODO: This is bad. Remove this and send a render queue instead.
        bool is_segment(int in_x, int in_y) const {
            for (size_t i = 0; i < segment_count; ++i) {
                const Segment& segment = segments[i];

                if (segment.x == in_x && segment.y == in_y) {
                    return true;
                }
            }

            return false;
        }
    };

    struct Prey {
        // TODO: Later add movement for prey.
        // TODO: Later add segments for prey too and use the same decoupled logic as the snake. Perhaps componentialize all these data.
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
        static constexpr const size_t MAX_PREY_COUNT = 4;
        static constexpr const size_t MAX_SNAKE_COUNT = 1;

        Level level;
        Prey preys[MAX_PREY_COUNT];
        size_t prey_count;
        Snake snakes[MAX_SNAKE_COUNT];
        size_t snake_count;
        double accumulated_tick;

        void add_prey(Prey prey) {
            preys[prey_count++] = prey;
        }

        void add_snake(Snake snake, int x, int y) {
            snake.init(x, y);

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
        state.add_snake(Snake{Direction::EAST, 0}, 1, 1);

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

    void do_movement() {
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

    void do_collision() {
        Snake& snake = state.player();

        if (snake.x < 0) {
            snake.x = 0;
        }

        if (snake.x > state.level.cell_count_x - 1) {
            snake.x = state.level.cell_count_x - 1;
        }

        if (snake.y < 0) {
            snake.y = 0;
        }

        if (snake.y > state.level.cell_count_y - 1) {
            snake.y = state.level.cell_count_y - 1;
        }

        // TODO: This doesn't belong here. Figure out how to decouple future location based on past location and collision and update the segments appropriately
        snake.set_location();

        size_t prey_count = state.prey_count;

        for (size_t i = 0; i < prey_count; ++i) {
            const Prey& prey = state.preys[i];
            if (snake.x == prey.x && snake.y == prey.y) {
                --state.prey_count;
                snake.add_segment();
            }
        }
    }

    void tick() {
        do_movement();
        do_collision();
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
