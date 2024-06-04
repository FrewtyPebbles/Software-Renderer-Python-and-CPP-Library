#include "Window.h"
#include <functional>
#include "Camera.h"
#include <iostream>
#include <mutex>

window::window() {
    this->title = L"Default Window Title";
    this->create_window(0, 0, 0, SW_SHOW);
}

window::window(wstring title, camera* cam, screen* scr) : cam(cam), scr(scr), title(title) {
    this->create_window(0, 0, 0, SW_SHOW);
}

void window::create_window(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    LPCWSTR CLASS_NAME = L"Window Class";
    
    this->window_class = { };

    this->window_class.lpfnWndProc   = this->WindowProc;
    this->window_class.hInstance     = hInstance;
    this->window_class.lpszClassName = CLASS_NAME;

    RegisterClass(&this->window_class);

    this->hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        this->title.c_str(),    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        this        // Additional application data
        );


    if (hwnd == NULL)
    {
        throw "Failed to create application window.";
    }

    ShowWindow(hwnd, nCmdShow);

    this->window_loop = std::thread([&]{
        MSG msg = { };
        while (GetMessage(&msg, this->hwnd, 0, 0) > 0)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    });

    return;
}


void window::update() {
    this->is_painting = true;
    InvalidateRect(this->hwnd, NULL, TRUE);
    UpdateWindow(this->hwnd);
}

std::mutex window_mutex;
LRESULT CALLBACK window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    window* self;
    if (uMsg == WM_NCCREATE) {
        self = static_cast<window*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);

        SetLastError(0);
        if (!SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(self)))
        {
            if (GetLastError() != 0)
                return FALSE;
        }
    } else {
        self = reinterpret_cast<window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
    }
    switch (uMsg)
    {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_PAINT:
            {
                
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hwnd, &ps);
                if (self) {
                    RECT rect = {0,0,self->cam->view_width,self->cam->view_height};
                    InvalidateRect(hwnd, &rect, TRUE);
                    FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));
                    // All painting occurs here, between BeginPaint and EndPaint.

                    for (pixel pixl : self->cam->frame_buffer) {
                        SetPixel(hdc, pixl.x, pixl.y, RGB(pixl.color[0],pixl.color[1],pixl.color[2]));
                    }
                }
                
                EndPaint(hwnd, &ps);
                
            }

            if (self) {
                self->is_painting = false;
            }
            return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}