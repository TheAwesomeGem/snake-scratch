// Copyright (c) Topaz Centuallas 2022.

#include "consumption.h"
#include "game_state.h"


namespace Game {
    void init_consumption(GameState& state) {
        state.add_killed_by_listener([](GameState& inner_state, Prey& victim, Snake& attacker) {
            --inner_state.prey_count;
            attacker.add_segment();
        });
    }
}
