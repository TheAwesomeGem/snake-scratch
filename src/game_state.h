// Copyright (c) Topaz Centuallas 2022.

#pragma once

#include "renderer.h"
#include <cassert>
#include <optional>
#include <uuid.h>
#include <unordered_map>
#include <deque>
#include "random.h"


namespace Game {
    using EntityId = uuids::uuid;

    enum class Direction {
        NORTH,
        SOUTH,
        EAST,
        WEST,

        MAX
    };

    struct AI {
        enum class Type {
            PREY,
            PREDATOR
        };

        Type type;
    };

    struct Transform {
        int x = 0;
        int y = 0;
        Direction direction = Direction::WEST;
    };

    struct Movement {
        uint16_t ticks_per_step;
        uint16_t accumulated_ticks;

        bool just_moved() const {
            return accumulated_ticks == 0;
        }
    };

    struct Consumption {
        EntityId eaten;
    };

    struct Segment {
        static constexpr const size_t MAX_SEGMENT_COUNT = 12;

        size_t segment_count = 0;
        EntityId segments[MAX_SEGMENT_COUNT];
    };

    struct Render {
        Color color;
    };

    struct Respawn {
        enum class Type {
            PREY,
            SNAKE
        };

        Type type;
    };

    struct Eatable {

    };

    struct Killable {

    };

    struct Entity {
        Transform transform;
        Render render;
        std::optional<Movement> movement;
        std::optional<Consumption> consumption;
        std::optional<Segment> segment;
        std::optional<AI> ai;
        std::optional<Respawn> respawn;
        std::optional<Eatable> eatable;
        std::optional<Killable> killable;
        bool is_alive = false;
    };

    struct Level {
        int cell_count_x;
        int cell_count_y;
        float cell_padding;
        float cell_size;

        [[nodiscard]] float total_width() const {
            return (float) cell_count_x * (cell_size + cell_padding);
        }

        [[nodiscard]] float total_height() const {
            return (float) cell_count_y * (cell_size + cell_padding);
        }

        [[nodiscard]] float from_cell_x(int x) const {
            return (float) x * (cell_size + cell_padding);
        }

        [[nodiscard]] float from_cell_y(int y) const {
            return (float) y * (cell_size + cell_padding);
        }
    };

    struct GameState {
        Level level;
        std::unordered_map<EntityId, Entity> entities;
        std::deque<CommandType> input_commands;
        double time_since_last_tick;
        EntityId player_id;

        GameState()
                : level{}, entities{}, input_commands{}, time_since_last_tick{0.0}, player_id{} {

        }

        [[nodiscard]] Entity& player() {
            return entities.at(player_id);
        }
    };

    // TODO: Move this to an entity module

    void clear_invalid_entities(GameState& state, EntityId* storage, size_t& count);

    Entity* get_entity_or_remove(GameState& state, EntityId id_to_find, EntityId* storage, size_t& count);

    void init_level(GameState& state);

    void on_player_death(GameState& state);
}
