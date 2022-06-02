// Copyright (c) Topaz Centuallas 2022.

#pragma once

#include <random>
#include <chrono>
#include <uuid.h>


namespace Game {
    static std::mt19937 rng_gen{static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count())};
    static uuids::uuid_random_generator uuid_gen{rng_gen};
}
