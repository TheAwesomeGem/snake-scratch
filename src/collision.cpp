// Copyright (c) Topaz Centuallas 2022.

#include "collision.h"
#include "game_state.h"


namespace Game {
    void do_collision(GameState& state) {
        Snake& snake = state.player();
        int snake_x = snake.head().x;
        int snake_y = snake.head().y;

        if (snake_x < 1 && snake.move_direction == Direction::WEST) {
            snake.move_direction = Direction::SOUTH;
        }

        if (snake_y < 1 && snake.move_direction == Direction::SOUTH) {
            snake.move_direction = Direction::EAST;
        }


        if (snake_x >= state.level.cell_count_x - 1 && snake.move_direction == Direction::EAST) {
            snake.move_direction = Direction::NORTH;
        }

        if (snake_y >= state.level.cell_count_y - 1 && snake.move_direction == Direction::NORTH) {
            snake.move_direction = Direction::WEST;
        }

        size_t prey_count = state.prey_count;

        for (size_t i = 0; i < prey_count; ++i) {
            Prey& prey = state.preys[i];
            if (snake_x == prey.x && snake_y == prey.y) {
                state.broadcast_killed_by(prey, snake);
            }
        }
    }
}
