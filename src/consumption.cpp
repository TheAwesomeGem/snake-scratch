// Copyright (c) Topaz Centuallas 2022.

#include "consumption.h"
#include "game_state.h"


namespace Game {
    void do_consumption(GameState& state) {
        for (EntityId i = 0; i < state.entity_count; ++i) {
            Entity& entity = state.entities[i];

            if (!entity.consumption.has_value()) {
                continue;
            }

            Consumption& consumption = entity.consumption.value();

            if (consumption.eaten == INVALID_ENTITY) {
                continue;
            }

            if (entity.segment.has_value()) {
                state.spawn_segment(entity);
            }

            state.entities[consumption.eaten].is_alive = false;
            consumption.eaten = INVALID_ENTITY;
        }
    }
}
