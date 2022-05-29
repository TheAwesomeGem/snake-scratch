// Copyright (c) Topaz Centuallas 2022.

#pragma once

#include "core.h"
#include <glad/glad.h>


namespace Game {
    struct Color {
        float r;
        float g;
        float b;
        float a;
    };

    class Renderer {
    public:
        Renderer(float viewport_width_, float viewport_height_)
                : drawable{0}, program{0}, viewport_width{viewport_width_}, viewport_height{viewport_height_} {
        }

        ~Renderer() {
            deinit();
        }

        void init();

        void deinit();

        void render_rectangle(float x, float y, float width, float height, Color color) const;

        void render_rectangle_border(float x, float y, float width, float height, Color color, float thickness) const;

        float width() const {
            return viewport_width;
        }

        float height() const {
            return viewport_height;
        }

    private:
        GLuint drawable;
        GLuint program;
        float viewport_width;
        float viewport_height;
    };
}
