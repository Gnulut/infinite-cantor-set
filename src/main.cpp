#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <algorithm>
#include <string>

#include "cantor.hpp"

constexpr int FPS = 60;
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

int window_width = 1500, window_height = 480;
constexpr double min_scale = 1.01, max_scale = 1.3;
double scaler = 1.05;
int scaler_show_frame = 0;
cantor_set cantor;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_SetAppMetadata("Cantor Set", "1.0", "");
    SDL_SetHint(SDL_HINT_MAIN_CALLBACK_RATE, std::to_string(FPS).c_str());

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Cantor Set", window_width, window_height, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    switch(event->type) {
        case SDL_EVENT_QUIT: {
            return SDL_APP_SUCCESS;
            break;
        }
        case SDL_EVENT_MOUSE_WHEEL: {
            expand_cantor(&cantor, scaler, window_width);
            break;
        }
        case SDL_EVENT_KEY_DOWN: {
            switch(event->key.scancode) {
                case SDL_SCANCODE_EQUALS: {
                    scaler = std::min(scaler+0.01, max_scale);
                    scaler_show_frame = FPS*3;
                    break;
                }
                case SDL_SCANCODE_MINUS: {
                    scaler = std::max(scaler-0.01, min_scale);
                    scaler_show_frame = FPS*3;
                    break;
                }
                case SDL_SCANCODE_Z: {
                    expand_cantor(&cantor, scaler, window_width);
                    break;
                }
            }
            break;
        }
    }
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    // get current window width, height
    SDL_GetWindowSize(window, &window_width, &window_height);

    // choose color
    const double now = ((double)SDL_GetTicks()) / 1000.0;  /* convert from milliseconds to seconds. */
    /* choose the color for the frame we will draw. The sine wave trick makes it fade between colors smoothly. */
    SDL_FColor color;
    color.r = (float) (0.5 + 0.5 * SDL_sin(now));
    color.g = (float) (0.5 + 0.5 * SDL_sin(now + SDL_PI_D * 2. / 3.));
    color.b = (float) (0.5 + 0.5 * SDL_sin(now + SDL_PI_D * 4. / 3.));
    color.a = SDL_ALPHA_OPAQUE_FLOAT;
    
    // fresh canvas
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);

    // render
    cantor.render(renderer, window, &color);

    // debug
    if(scaler_show_frame > 0){
        float strength = std::min((float)scaler_show_frame/FPS, 1.f);
        scaler_show_frame--;
        SDL_SetRenderScale(renderer, 4.f, 4.f);
        SDL_SetRenderDrawColorFloat(renderer, 1.0f*strength, 0.8f*strength, 0.3f*strength, SDL_ALPHA_OPAQUE_FLOAT);
        SDL_RenderDebugTextFormat(renderer, 0.f, 0.f, "Scale value: %.2lf", scaler);
        SDL_SetRenderScale(renderer, 1.f, 1.f);
    }

    // continue
    SDL_RenderPresent(renderer);
    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    /* SDL will clean up the window/renderer for us. */
}
