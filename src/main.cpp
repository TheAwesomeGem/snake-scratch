// Copyright (c) Topaz Centuallas 2022.

#include <glad/glad.h>
#include <SDL.h>
#include <SDL_image.h>
#include <cstdio>
#include "game.h"


// TODO: Separate platform/SDL code from game code.
// TODO: Add logger.

bool init_submodules() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Error: %s\n", SDL_GetError());

        return false;
    }

    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        printf("Error: %s\n", IMG_GetError());

        return false;
    }

    return true;
}

SDL_Window* init_window() {
    SDL_Window* window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED, 1280, 720,
                                          SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    if (window == nullptr) {
        printf("Error: Window couldn't be created due to %s\n", SDL_GetError());
    }

    return window;
}

SDL_Renderer* init_renderer(SDL_Window* window) {
    SDL_Renderer* renderer =
            SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (renderer == nullptr) {
        printf("Error: Renderer couldn't be created due to %s\n", SDL_GetError());
    }

    return renderer;
}

bool init_glad(SDL_Window* window) {
    // Load GL extensions using glad
    if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
        printf("Error: Failed to initialize the OpenGL context.");

        return false;
    }

    // Loaded OpenGL successfully.
    printf("INFO: OpenGL version loaded: %d.%d\n", GLVersion.major, GLVersion.minor);
    printf("INFO: Vendor:   %s\n", glGetString(GL_VENDOR));
    printf("INFO: Renderer: %s\n", glGetString(GL_RENDERER));
    printf("INFO: Version:  %s\n", glGetString(GL_VERSION));

    return true;
}

void destroy(SDL_Window* window, SDL_Renderer* renderer, SDL_GLContext* gl_context) {
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    IMG_Quit();
    SDL_Quit();
}

int main(int arg_c, char* arg_v[]) {
    if (!init_submodules()) {
        exit(1);
    }

    SDL_Window* window = init_window();

    if (!window) {
        exit(1);
    }

    SDL_Renderer* renderer = init_renderer(window);

    if (!renderer) {
        exit(1);
    }

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    if (!init_glad(window)) {
        exit(1);
    }

    int width, height;
    SDL_GetWindowSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glClearColor(0.16f, 0.17f, 0.21f, 1.0f);

    Uint64 current_frame_time = SDL_GetPerformanceCounter();
    Uint64 last_frame_time;
    double delta_frame_time;
    bool is_running = true;
    SDL_Event event;

    SDL_GL_SetSwapInterval(-1); // Adaptive Vsync

    while (is_running) {
        last_frame_time = current_frame_time;
        current_frame_time = SDL_GetPerformanceCounter();

        delta_frame_time = ((((double) current_frame_time - (double) last_frame_time) * 1000.0 / (double) SDL_GetPerformanceFrequency())) * 0.001;

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT: {
                    is_running = false;
                    break;
                }

                case SDL_KEYDOWN: {
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            is_running = false;
                            break;
                    }
                }
            }

            Game::poll();
        }

        printf("INFO: %d FPS\n", (int) (1.0 / delta_frame_time));
        Game::update(delta_frame_time);

        glClear(GL_COLOR_BUFFER_BIT);

        Game::render();

        SDL_GL_SwapWindow(window);
    }

    destroy(window, renderer, &gl_context);

    return 0;
}
