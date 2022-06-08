// Copyright (c) Topaz Centuallas 2022.

#include "movement.h"
#include "game_state.h"


namespace Game {
    void update_snake_position(GameState& state, Entity& snake, int x, int y) {
        Segment& segment = snake.segment.value();
        clear_invalid_entities(state, segment.segments, segment.segment_count);

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
        for (auto& [entity_id, entity]: state.entities) {
            int new_x = entity.transform.x;
            int new_y = entity.transform.y;

            switch (entity.transform.direction) {
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

            if (entity.segment.has_value()) {
                update_snake_position(state, entity, new_x, new_y);
            }
            else {
                entity.transform.x = new_x;
                entity.transform.y = new_y;
            }
        }
    }
}
