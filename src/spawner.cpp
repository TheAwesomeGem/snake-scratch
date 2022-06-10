// Copyright (c) Topaz Centuallas 2022.

#include "spawner.h"
#include "game_state.h"


namespace Game {
    void spawn_prey(GameState& state, int x, int y) {
        Entity entity{
                Transform{x, y, Direction::WEST},
                Render{Color{0.0F, 1.0F, 0.0F, 1.0F}},
                Movement{5, 0},
                std::nullopt,
                std::nullopt,
                AI{AI::Type::PREY},
                Respawn{Respawn::Type::PREY},
                true
        };

        state.entities.emplace(uuid_gen(), entity);
    }

    void spawn_snake(GameState& state, int x, int y) {
        Entity entity{
                Transform{x, y, Direction::WEST},
                Render{Color{0.0F, 0.0F, 1.0F, 1.0F}},
                Movement{3, 0},
                std::optional(Consumption{}),
                std::optional(Segment{0, EntityId{}}),
                AI{AI::Type::PREDATOR},
                Respawn{Respawn::Type::SNAKE},
                true
        };

        EntityId id = uuid_gen();

        if (state.player_id.is_nil()) {
            state.player_id = id;
            entity.ai = std::nullopt;
        }

        state.entities.emplace(id, entity);
    }

    void spawn_segment(GameState& state, struct Entity& parent) {
        if (!parent.segment.has_value()) {
            return;
        }

        Segment& segment = parent.segment.value();

        assert(segment.segment_count < Segment::MAX_SEGMENT_COUNT);

        Transform transform = Transform{parent.transform.x, parent.transform.y, Direction::WEST};

        if (segment.segment_count > 0) {
            const Entity& prev_segment_entity = state.entities[segment.segments[segment.segment_count - 1]];
            transform.x = prev_segment_entity.transform.x;
            transform.y = prev_segment_entity.transform.y;
        }

        EntityId segment_entity_id = uuid_gen();
        state.entities.emplace(segment_entity_id, Entity{
                transform, parent.render.color, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, true
        });

        segment.segments[segment.segment_count++] = segment_entity_id;
    }

    void spawn_prey_random(GameState& state) {
        auto dist_x = std::uniform_int_distribution{0, state.level.cell_count_x - 1};
        auto dist_y = std::uniform_int_distribution{0, state.level.cell_count_y - 1};
        spawn_prey(state, dist_x(rng_gen), dist_y(rng_gen));
    }

    void on_entity_death(GameState& state, const Entity& entity) {
        if (!entity.respawn.has_value()) {
            return;
        }

        switch (entity.respawn->type) {
            case Respawn::Type::PREY: {
                spawn_prey_random(state);

                break;
            }

            default: {
                printf("Unhandled respawn\n");
            }
        }
    }
}
