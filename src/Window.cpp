#include "Window.h"
#include <functional>
#include "Camera.h"
#include <iostream>
#include <mutex>
#include <format>

window::~window(){
    if (this->cam->use_software_renderer) {
        SDL_DestroyTexture(this->texture);
        SDL_DestroyRenderer(this->renderer);
    }
    SDL_GL_DeleteContext(this->gl_context);
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
    atexit(SDL_Quit);

    // SDL_GL_LoadLibrary(NULL);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

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
    
    if (!gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress)) {
        std::cout << "Failed to initialize GLAD\n";
        throw "Failed to initialize GLAD";
    }
    std::cout << std::format("Vendor:   {}\n", (char *)glGetString(GL_VENDOR));
    std::cout << std::format("Renderer: {}\n", (char *)glGetString(GL_RENDERER));
    std::cout << std::format("Version:  {}\n", (char *)glGetString(GL_VERSION));

    if (this->cam->use_software_renderer) {
        this->renderer = SDL_CreateRenderer(this->app_window, -1, SDL_RENDERER_ACCELERATED);

        this->texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_STREAMING, this->width, this->height);
    }

    // SDL_GetWindowSize(this->app_window, &this->width, &this->height);
    // Use v-sync
    SDL_GL_SetSwapInterval(1);
    glViewport(0, 0, this->width, this->height);
    // this->update();
    
    return;
}


void window::update() {
    SDL_Event event;
    SDL_GL_SwapWindow(this->app_window);
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

    

    if (this->cam->use_software_renderer) {
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

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT); 

    SDL_GL_SwapWindow(this->app_window);
    SDL_Delay(1);

}