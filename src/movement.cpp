// Copyright (c) Topaz Centuallas 2022.

#include "movement.h"
#include "game_state.h"


namespace Game {
    void update_snake_position(GameState& state, Entity& snake, int x, int y) {
        const Segment& segment = snake.segment.value();

        if (segment.segment_count > 0) {
            for (size_t i = segment.segment_count - 1; i > 0; --i) {
                Entity& previous = state.entities[segment.segments[i]];
                Entity& next = state.entities[segment.segments[i - 1]];

                previous.transform.x = next.transform.x;
                previous.transform.y = next.transform.y;
            }

            Entity& first_segment_entity = state.entities[segment.segments[0]];
            first_segment_entity.transform.x = snake.transform.x;
            first_segment_entity.transform.y = snake.transform.y;
        }

        snake.transform.x = x;
        snake.transform.y = y;
    }

    void do_movement(GameState& state) {
        Entity& snake = state.player();
        int new_x = snake.transform.x;
        int new_y = snake.transform.y;

        switch (snake.transform.direction) {
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

        update_snake_position(state, snake, new_x, new_y);
    }
}
