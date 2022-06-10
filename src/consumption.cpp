// Copyright (c) Topaz Centuallas 2022.

#include "consumption.h"
#include "game_state.h"
#include "spawner.h"


namespace Game {
    void do_consumption(GameState& state) {
        for (auto& [entity_id, entity]: state.entities) {
            if (!entity.consumption.has_value()) {
                continue;
            }

            Consumption& consumption = entity.consumption.value();

            if (consumption.eaten.is_nil()) {
                continue;
            }

            if (entity.segment.has_value()) {
                spawn_segment(state, entity);
            }

            state.entities[consumption.eaten].is_alive = false;
            consumption.eaten = uuids::uuid{};
        }
    }
}
