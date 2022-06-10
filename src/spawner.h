// Copyright (c) Topaz Centuallas 2022.

#pragma once


namespace Game {
    void spawn_prey(struct GameState& state, int x, int y);

    void spawn_snake(struct GameState& state, int x, int y);

    void spawn_segment(struct GameState& state, struct Entity& parent);

    void spawn_prey_random(struct GameState& state);

    void on_entity_death(struct GameState& state, const struct Entity& entity);
}
