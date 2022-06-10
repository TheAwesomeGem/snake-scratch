// Copyright (c) Topaz Centuallas 2022.

#include "game_state.h"
#include "spawner.h"


namespace Game {
    void init_level(GameState& state) {
        state.level = Level{40, 20, 2.0F, 24.0F};
        spawn_snake(state, 1, 1);
        auto prey_spawn_dist = std::uniform_int_distribution<size_t>(8, 12);
        auto snake_spawn_dist = std::uniform_int_distribution<size_t>(2, 2);

        for (size_t i = 0; i < snake_spawn_dist(rng_gen); ++i) {
            spawn_snake_random(state);
        }

        for (size_t i = 0; i < prey_spawn_dist(rng_gen); ++i) {
            spawn_prey_random(state);
        }
    }

    void on_player_death(GameState& state) {
        state = GameState{};
        init_level(state);
    }

    void clear_invalid_entities(GameState& state, EntityId* storage, size_t& count) {
        for (size_t i = 0; i < count; ++i) {
            EntityId& id = storage[i];

            if (state.entities.find(id) == state.entities.end()) {
                std::swap(id, storage[count - 1]);
                --count;
            }
        }
    }

    Entity* get_entity_or_remove(GameState& state, EntityId id_to_find, EntityId* storage, size_t& count) {
        clear_invalid_entities(state, storage, count);

        for (size_t i = 0; i < count; ++i) {
            EntityId& id = storage[i];

            if (id_to_find != id) {
                continue;
            }

            if (state.entities.find(id) == state.entities.end()) {
                return nullptr;
            }

            return &state.entities[id];
        }

        return nullptr;
    }
}
