// Copyright (c) Topaz Centuallas 2022.

#include "input.h"


namespace Game {
    void do_input(GameState& state) {
        if (state.input_commands.empty()) {
            return;
        }

        Entity& player = state.player();

        switch (state.input_commands.front()) {
            case CommandType::TURN_LEFT:
                if (player.transform.direction != Direction::EAST) {
                    player.transform.direction = Direction::WEST;
                }
                break;
            case CommandType::TURN_RIGHT:
                if (player.transform.direction != Direction::WEST) {
                    player.transform.direction = Direction::EAST;
                }
                break;
            case CommandType::TURN_UP:
                if (player.transform.direction != Direction::SOUTH) {
                    player.transform.direction = Direction::NORTH;
                }
                break;
            case CommandType::TURN_DOWN:
                if (player.transform.direction != Direction::NORTH) {
                    player.transform.direction = Direction::SOUTH;
                }
                break;
        }

        state.input_commands.pop_front();
    }
}
