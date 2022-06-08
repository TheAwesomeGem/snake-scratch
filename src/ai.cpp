// Copyright (c) Topaz Centuallas 2022.

#include "ai.h"
#include "game_state.h"


namespace Game {
    std::vector<Direction> get_available_direction(const GameState& state, const Entity& entity) {
        std::vector<Direction> directions{Direction::WEST, Direction::SOUTH, Direction::EAST, Direction::NORTH};
        int x = entity.transform.x;
        int y = entity.transform.y;

        if (x < 1) {
            directions.erase(std::find(directions.begin(), directions.end(), Direction::WEST));
        }

        if (y < 1) {
            directions.erase(std::find(directions.begin(), directions.end(), Direction::SOUTH));
        }


        if (x >= state.level.cell_count_x - 1) {
            directions.erase(std::find(directions.begin(), directions.end(), Direction::EAST));
        }

        if (y >= state.level.cell_count_y - 1) {
            directions.erase(std::find(directions.begin(), directions.end(), Direction::NORTH));
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
                    prey_movement_ai(state, entity);
                    break;
                }

                default: {
                    printf("Unhandled AI.");
                }
            }
        }
    }
}
