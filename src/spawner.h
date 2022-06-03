// Copyright (c) Topaz Centuallas 2022.

#pragma once


namespace Game {
    void spawn_prey(struct GameState& state, int x, int y);

    void spawn_snake(struct GameState& state, int x, int y);

    void spawn_segment(struct GameState& state, struct Entity& parent);

    void prey_death(struct GameState& state);
}
