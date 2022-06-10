// Copyright (c) Topaz Centuallas 2022.

#include "collision.h"
#include "game_state.h"


namespace Game {
    void do_collision(GameState& state) {
        for (auto& [entity_id, entity]: state.entities) {
            int x = entity.transform.x;
            int y = entity.transform.y;

            if ((x < 1 && entity.transform.direction == Direction::WEST) ||
                (y < 1 && entity.transform.direction == Direction::SOUTH) ||
                (x >= state.level.cell_count_x - 1 && entity.transform.direction == Direction::EAST) ||
                (y >= state.level.cell_count_y - 1 && entity.transform.direction == Direction::NORTH)) {
                entity.is_alive = false;

                continue;
            }

            int new_x;
            int new_y;
            switch (entity.transform.direction) {
                case Direction::NORTH:
                    new_x = x;
                    new_y = y + 1;
                    break;
                case Direction::SOUTH:
                    new_x = x;
                    new_y = y - 1;
                    break;
                case Direction::EAST:
                    new_x = x + 1;
                    new_y = y;
                    break;
                case Direction::WEST:
                    new_x = x - 1;
                    new_y = y;
                    break;
                default:
                    new_x = x;
                    new_y = y;
                    break;
            }

            if (entity.consumption.has_value()) {
                for (const auto& [other_entity_id, other_entity]: state.entities) {
                    if (&other_entity == &entity) {
                        continue;
                    }

                    if (new_x == other_entity.transform.x && new_y == other_entity.transform.y) {
                        if (other_entity.eatable.has_value()) {
                            entity.consumption->eaten = other_entity_id;
                        }

                        if (other_entity.killable.has_value()) {
                            entity.is_alive = false;
                        }
                    }
                }
            }
        }
    }
}
