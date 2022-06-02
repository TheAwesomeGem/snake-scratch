// Copyright (c) Topaz Centuallas 2022.

#include "game_state.h"


namespace Game {
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
