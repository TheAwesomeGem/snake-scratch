// Copyright (c) Topaz Centuallas 2022.

#include "collision.h"
#include "game_state.h"


namespace Game {
    void do_collision(GameState& state) {
        Entity& snake = state.player();
        int snake_x = snake.transform.x;
        int snake_y = snake.transform.y;

        if (snake_x < 1 && snake.transform.direction == Direction::WEST) {
            snake.transform.direction = Direction::SOUTH;
        }

        if (snake_y < 1 && snake.transform.direction == Direction::SOUTH) {
            snake.transform.direction = Direction::EAST;
        }


        if (snake_x >= state.level.cell_count_x - 1 && snake.transform.direction == Direction::EAST) {
            snake.transform.direction = Direction::NORTH;
        }

        if (snake_y >= state.level.cell_count_y - 1 && snake.transform.direction == Direction::NORTH) {
            snake.transform.direction = Direction::WEST;
        }

        int new_snake_x;
        int new_snake_y;
        switch (snake.transform.direction) {
            case Direction::NORTH:
                new_snake_x = snake_x;
                new_snake_y = snake_y + 1;
                break;
            case Direction::SOUTH:
                new_snake_x = snake_x;
                new_snake_y = snake_y - 1;
                break;
            case Direction::EAST:
                new_snake_x = snake_x + 1;
                new_snake_y = snake_y;
                break;
            case Direction::WEST:
                new_snake_x = snake_x - 1;
                new_snake_y = snake_y;
                break;
            default:
                new_snake_x = snake_x;
                new_snake_y = snake_y;
                break;
        }

        for (EntityId i = 1; i < state.entity_count; ++i) {
            Entity& entity = state.entities[i];
            // TODO: Add tag here for segment and prey
            if (new_snake_x == entity.transform.x && new_snake_y == entity.transform.y) {
                snake.consumption.value().eaten = i;
            }
        }
    }
}
