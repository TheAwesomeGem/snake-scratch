// Copyright (c) Topaz Centuallas 2022.

#include "movement.h"
#include "game_state.h"


namespace Game {
    void update_snake_position(Snake& snake, int x, int y) {
        for (size_t i = snake.segment_count - 1; i > 0; --i) {
            Segment& previous = snake.segments[i];
            Segment& next = snake.segments[i - 1];

            previous.x = next.x;
            previous.y = next.y;
        }

        snake.head().x = x;
        snake.head().y = y;
    }

    void do_movement(GameState& state) {
        Snake& snake = state.player();
        int new_x = snake.head().x;
        int new_y = snake.head().y;

        switch (snake.move_direction) {
            case Direction::NORTH:
                new_y += 1;
                break;
            case Direction::SOUTH:
                new_y -= 1;
                break;
            case Direction::EAST:
                new_x += 1;
                break;
            case Direction::WEST:
                new_x -= 1;
                break;
        }

        update_snake_position(snake, new_x, new_y);
    }
}
