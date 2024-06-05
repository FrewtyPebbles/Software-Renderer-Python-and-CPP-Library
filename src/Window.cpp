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

    SDL_GL_LoadLibrary(NULL);

    // Request an OpenGL 4.5 context (should be core)
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    // Also request a depth buffer
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    if (this->fullscreen) {
        this->app_window = SDL_CreateWindow(
            this->title.c_str(), 
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
            0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_OPENGL
        );
    } else {
        this->app_window = SDL_CreateWindow(
            this->title.c_str(),
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            this->width, this->height,
            SDL_WINDOW_OPENGL
        );
    }
    if (this->app_window == NULL) {
        throw std::format("could not create window: {}\n", SDL_GetError());
    }

    this->gl_context = SDL_GL_CreateContext(this->app_window);
    if (this->gl_context == NULL) {
        throw "Failed to create OpenGL context";
    }

    std::cout << "OpenGL loaded\n";
    gladLoadGLLoader(SDL_GL_GetProcAddress);
    // std::cout << std::format("Vendor:   {}\n", glGetString(GL_VENDOR));
    // std::cout << std::format("Renderer: {}\n", glGetString(GL_RENDERER));
    // std::cout << std::format("Version:  {}\n", glGetString(GL_VERSION));

    this->renderer = SDL_CreateRenderer(this->app_window, -1, SDL_RENDERER_ACCELERATED);

    this->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, this->width, this->height);

    // Disable depth test and face culling.
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    SDL_GetWindowSize(this->app_window, &this->width, &this->height);
    // Use v-sync
    SDL_GL_SetSwapInterval(1);

    return;
}


void window::update() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        /* this is where we will record sdl events */
        switch (event.type) {
            case SDL_QUIT:
                this->current_event = event::QUIT;
                break;
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
                    default:
                        this->current_event = event::NOTHING;
                }
                break;

            case SDL_WINDOWEVENT:

                switch (event.window.event) {

                    case SDL_WINDOWEVENT_CLOSE:   // exit game
                        this->current_event = event::WINDOW_CLOSE;
                        break;

                    default:
                        this->current_event = event::NOTHING;
                }
                break;

            default:
                this->current_event = event::NOTHING;
        }
    }

    glViewport(0, 0, this->width, this->height);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glClear(GL_COLOR_BUFFER_BIT);


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

    SDL_GL_SwapWindow(this->app_window);

}