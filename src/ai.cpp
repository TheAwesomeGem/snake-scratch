// Copyright (c) Topaz Centuallas 2022.

#include "ai.h"
#include "game_state.h"


namespace Game {
    void safe_erase(std::vector<Direction>& directions, Direction direction) {
        auto it = std::find(directions.begin(), directions.end(), direction);

        if (it == directions.end()) {
            return;
        }

        directions.erase(it);
    }

    std::vector<Direction> get_available_direction(const GameState& state, const Entity& entity) {
        std::vector<Direction> directions{Direction::WEST, Direction::SOUTH, Direction::EAST, Direction::NORTH};
        int x = entity.transform.x;
        int y = entity.transform.y;

        if (entity.segment.has_value()) {
            switch (entity.transform.direction) {
                case Direction::NORTH:
                    safe_erase(directions, Direction::SOUTH);
                    break;
                case Direction::SOUTH:
                    safe_erase(directions, Direction::NORTH);
                    break;
                case Direction::EAST:
                    safe_erase(directions, Direction::WEST);
                    break;
                case Direction::WEST:
                    safe_erase(directions, Direction::EAST);
                    break;
                case Direction::MAX:
                    break;
            }
        }

        if (x < 1) {
            safe_erase(directions, Direction::WEST);
        }

        if (y < 1) {
            safe_erase(directions, Direction::SOUTH);
        }


        if (x >= state.level.cell_count_x - 1) {
            safe_erase(directions, Direction::EAST);
        }

        if (y >= state.level.cell_count_y - 1) {
            safe_erase(directions, Direction::NORTH);
        }

        return directions;
    }

    void prey_movement_ai(const GameState& state, Entity& entity) {
        std::vector<Direction> directions = get_available_direction(state, entity);
        auto dist = std::uniform_int_distribution<size_t>(0, directions.size() - 1);
        Direction direction = directions[dist(rng_gen)];

        entity.transform.direction = direction;
    }

    void do_ai(GameState& state) {
        for (auto& [entity_id, entity]: state.entities) {
            if (!entity.ai.has_value()) {
                continue;
            }

            switch (entity.ai->type) {
                case AI::Type::PREY: {
                    if (entity.movement.has_value() && entity.movement->just_moved()) {
                        prey_movement_ai(state, entity);
                    }
                    break;
                }

                case AI::Type::PREDATOR: {
                    if (entity.movement.has_value() && entity.movement->just_moved()) {
                        prey_movement_ai(state, entity);
                    }

                    break;
                }

                default: {
                    printf("Unhandled AI.");
                }
            }
        }
    }
}
