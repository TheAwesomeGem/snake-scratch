// Copyright (c) Topaz Centuallas 2022.

#pragma once

#include "renderer.h"
#include <cassert>


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

        Direction move_direction;
        size_t segment_count;
        Segment segments[MAX_SEGMENT_COUNT];

    public:
        Snake()
                : move_direction{Direction::EAST}, segment_count{0}, segments{} {

        }

        Snake(int x, int y)
                : Snake() {
            segments[segment_count++] = Segment{x, y};
        }

        void add_segment() {
            assert(segment_count < MAX_SEGMENT_COUNT - 1 || segment_count > 0);

            const Segment& prev_segment = segments[segment_count - 1];
            Segment segment{prev_segment.x, prev_segment.y};
            segments[segment_count++] = segment;
        }

        Segment& head() {
            return segments[0];
        }

        // TODO: This is bad. Remove this and send a render queue instead.
        [[nodiscard]] bool is_segment(int in_x, int in_y) const {
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

        [[nodiscard]] float total_width() const {
            return (float) cell_count_x * (cell_size + cell_padding);
        }

        [[nodiscard]] float total_height() const {
            return (float) cell_count_y * (cell_size + cell_padding);
        }

        [[nodiscard]] float from_cell_x(int x) const {
            return (float) x * (cell_size + cell_padding);
        }

        [[nodiscard]] float from_cell_y(int y) const {
            return (float) y * (cell_size + cell_padding);
        }
    };

    struct GameState {
        static constexpr const size_t MAX_PREY_COUNT = 4;
        static constexpr const size_t MAX_SNAKE_COUNT = 1;
        static constexpr const size_t MAX_KILLED_BY_LISTENERS = 4;

        Level level;
        Prey preys[MAX_PREY_COUNT];
        size_t prey_count;
        Snake snakes[MAX_SNAKE_COUNT];
        size_t snake_count;
        double accumulated_tick;
        int killed_by_listener_count;
        // TODO: Should GameState really be a part of the event args??
        void (* killed_by_event[MAX_KILLED_BY_LISTENERS])(GameState& inner_state, Prey& victim, Snake& attacker);

        GameState()
                : level{}, preys{}, prey_count{0}, snakes{}, snake_count{0}, accumulated_tick{0.0}, killed_by_listener_count{0}, killed_by_event{} {

        }

        void add_prey(Prey prey) {
            preys[prey_count++] = prey;
        }

        void add_snake(Snake snake) {
            snakes[snake_count++] = snake;
        }

        // TODO: This should be a pointer once we start handling 'death'
        [[nodiscard]] Snake& player() {
            return snakes[0];
        }

        void add_killed_by_listener(void(* listener)(GameState& inner_state, Prey& victim, Snake& killer)) {
            killed_by_event[killed_by_listener_count++] = listener;
        }

        void broadcast_killed_by(Prey& victim, Snake& killer) {
            for (size_t i = 0; i < killed_by_listener_count; ++i) {
                killed_by_event[i](*this, victim, killer);
            }
        }
    };
}
