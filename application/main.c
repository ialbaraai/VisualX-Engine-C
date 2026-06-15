#include <stdio.h>
#include <stdbool.h>
#include <SDL3/SDL.h>
#include "tinyfiledialogs.h"

int main(void)
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());

        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Window", 800, 600, SDL_WINDOW_RESIZABLE);

    if (!window)
    {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());

        SDL_Quit();

        return 2;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);

    if (!renderer)
    {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());

        SDL_DestroyWindow(window);
        SDL_Quit();

        return 3;
    }

    bool is_running = true;
    SDL_Event event;

    SDL_FRect rect = { (800 / 2.0f) - (100 / 2.0f), (600 / 2.0f) - (100 / 2.0f), 100, 100 };
    SDL_Color rectc = { 0, 0, 0, 255 };

    uint64_t last_tick = SDL_GetTicks();
    float accumulator = 0.0f;

    bool is_changing_color = false;

    char const* filter[] = {"*.lua"};

    while (is_running)
    {
        uint64_t current_tick = SDL_GetTicks();
        float dt = (current_tick - last_tick) / 1000.0f;
        accumulator += dt;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                is_running = false;
            }
            if (event.type == SDL_EVENT_KEY_DOWN)
            {
                if (event.key.key == SDLK_P)
                {
                    is_changing_color = !is_changing_color;
                }
                if (event.key.key == SDLK_F)
                {
                    printf("Will handle this later!\n");
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 225, 225, 225, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, rectc.r, rectc.g, rectc.b, 255);
        SDL_RenderFillRect(renderer, &rect);

        if (is_changing_color)
        {
            rectc.r += 3;
            rectc.g += 2;
            rectc.b += 1;

            rectc.r = rectc.r > 255 ? 0 : rectc.r;
            rectc.g = rectc.g > 255 ? 0 : rectc.g;
            rectc.b = rectc.b > 255 ? 0 : rectc.b;
        }

        SDL_RenderPresent(renderer);

        last_tick = SDL_GetTicks();

        uint64_t duration = last_tick - current_tick;

        if (duration < 1000.0f / 60.0f)
        {
            SDL_Delay((1000.0f / 60.0f) - duration);
        }
    }

    printf("Time in engine: %.2fs\n", accumulator);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}