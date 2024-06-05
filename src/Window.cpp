#include "Window.h"
#include <functional>
#include "Camera.h"
#include <iostream>
#include <mutex>
#include <format>

window::~window(){
    SDL_DestroyTexture(this->texture);
    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->app_window);
    SDL_Quit();
}

window::window() {
    this->title = "Default Window Title";
    this->create_window();
}

window::window(string title, camera* cam, screen* scr, int width, int height) : cam(cam), scr(scr), title(title), width(width), height(height), current_event(event::NOTHING) {
    this->create_window();
}

void window::create_window() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw std::format("could not initialize sdl2: {}\n", SDL_GetError());
    }
    this->app_window = SDL_CreateWindow(
        this->title.c_str(),
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        this->width, this->height,
        SDL_WINDOW_SHOWN
	);
    if (this->app_window == NULL) {
        throw std::format("could not create window: {}\n", SDL_GetError());
    }

    this->renderer = SDL_CreateRenderer(this->app_window, -1, SDL_RENDERER_ACCELERATED);

    this->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, this->width, this->height);

    return;
}


void window::update() {

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        /* this is where we will record sdl events */
        switch (event.type) {
            
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        this->current_event = event::KEY_UP;
                        break;

                    case SDLK_DOWN:
                        this->current_event = event::KEY_DOWN;
                        break;

                    case SDLK_LEFT:
                        this->current_event = event::KEY_LEFT;
                        break;

                    case SDLK_RIGHT:
                        this->current_event = event::KEY_RIGHT;
                        break;

                    case SDLK_SPACE:
                        this->current_event = event::KEY_SPACE;
                        break;
                }
                break;

            case SDL_WINDOWEVENT:

                switch (event.window.event) {

                    case SDL_WINDOWEVENT_CLOSE:   // exit game
                        this->current_event = event::WINDOW_CLOSE;
                        break;

                    default:
                        this->current_event = event::NOTHING;
                        break;
                }
                break;

            default:
                this->current_event = event::NOTHING;
                break;
        }
    }

    void* texPixels;
    int pitch;
    SDL_LockTexture(texture, NULL, &texPixels, &pitch);
    // Copy pixel data into texture's pixel buffer
    memcpy(texPixels, this->cam->frame_buffer, this->width * this->height * sizeof(Uint32));
    // Unlock texture
    SDL_UnlockTexture(this->texture);

    SDL_RenderClear(this->renderer);

    // Render texture
    SDL_RenderCopy(this->renderer, this->texture, NULL, NULL);

    // Update screen
    SDL_RenderPresent(this->renderer);

}