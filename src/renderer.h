// Copyright (c) Topaz Centuallas 2022.

#pragma once

#include "core.h"
#include <glad/glad.h>


namespace Game {
    class Renderer {
    public:
        Renderer()
                : drawable{0}, program{0} {
        }

        ~Renderer() {
            deinit();
        }

        void init();

        void deinit();

        void render_rectangle(float x, float y, float width, float height);

    private:
        GLuint drawable;
        GLuint program;
    };
}
