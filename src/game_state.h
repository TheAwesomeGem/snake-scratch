// Copyright (c) Topaz Centuallas 2022.

#pragma once

#include "renderer.h"
#include <cassert>
#include <optional>
#include <uuid.h>
#include <unordered_map>
#include "random.h"


namespace Game {
    using EntityId = uuids::uuid;

    enum class Direction {
        NORTH,
        SOUTH,
        EAST,
        WEST
    };

    struct Transform {
        int x = 0;
        int y = 0;
        Direction direction = Direction::WEST;
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

    struct Entity {
        Transform transform;
        Render render;
        std::optional<Consumption> consumption;
        std::optional<Segment> segment;
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
        double accumulated_tick;
        EntityId player_id;

        GameState()
                : level{}, entities{}, accumulated_tick{0.0} {

        }

        // TODO: Move all of these spawn inside it's own module/file and outside of the struct.

        void spawn_prey(int x, int y) {
            Entity entity{
                    Transform{x, y, Direction::WEST},
                    Render{Color{0.0F, 1.0F, 0.0F, 1.0F}},
                    std::nullopt,
                    std::nullopt,
                    true
            };

            entities.emplace(uuid_gen(), entity);
        }

        void spawn_snake(int x, int y) {
            Entity entity{
                    Transform{x, y, Direction::WEST},
                    Render{Color{0.0F, 0.0F, 1.0F, 1.0F}},
                    std::optional(Consumption{}),
                    std::optional(Segment{0, EntityId{}}),
                    true
            };

            EntityId id = uuid_gen();

            if (player_id.is_nil()) {
                player_id = id;
            }

            entities.emplace(id, entity);
        }

        void spawn_segment(Entity& entity) {
            if (!entity.segment.has_value()) {
                return;
            }

            Segment& segment = entity.segment.value();

            assert(segment.segment_count < Segment::MAX_SEGMENT_COUNT);

            Transform transform = Transform{entity.transform.x, entity.transform.y, Direction::WEST};

            if (segment.segment_count > 0) {
                const Entity& prev_segment_entity = entities[segment.segments[segment.segment_count - 1]];
                transform.x = prev_segment_entity.transform.x;
                transform.y = prev_segment_entity.transform.y;
            }

            EntityId segment_entity_id = uuid_gen();
            entities.emplace(segment_entity_id, Entity{
                    transform, entity.render.color, std::nullopt, std::nullopt, true
            });

            segment.segments[segment.segment_count++] = segment_entity_id;
        }

        // TODO: This should be a pointer once we start handling player 'death'
        [[nodiscard]] Entity& player() {
            return entities.at(player_id);
        }
    };

    // TODO: Move this to an entity module

    void clear_invalid_entities(GameState& state, EntityId* storage, size_t& count);

    Entity* get_entity_or_remove(GameState& state, EntityId id_to_find, EntityId* storage, size_t& count);
}
