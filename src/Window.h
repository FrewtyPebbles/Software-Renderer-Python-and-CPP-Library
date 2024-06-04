#pragma once
#ifndef UNICODE
#define UNICODE
#endif
#include <windows.h>
#include <string>
#include <thread>

using std::wstring;
class camera;
class screen;
class window {
public:
    window();
    window(wstring title, camera* cam, screen* scr);
    camera* cam;
    screen* scr;
    wstring title;
    void update();
private:
    void create_window(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow);
    HWND hwnd;
    WNDCLASS window_class;
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    std::thread window_loop;
    bool is_painting;
};