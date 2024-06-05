# distutils: language = c++
from renderer.camera cimport camera, Camera, screen, Screen
from libcpp.string cimport string

cdef extern from "../src/Window.h":
    cpdef enum class event:
        NOTHING,
        WINDOW_CLOSE,
        QUIT,
        KEY_UP,
        KEY_DOWN,
        KEY_RIGHT,
        KEY_LEFT,
        KEY_SPACE

    cdef cppclass window:
        window() except +
        window(string title, camera* cam, screen* scr, int width, int height) except +
        camera* cam
        screen* scr
        string title
        int width, height
        void update()
        event current_event

cdef class Window:
    cdef window* c_class

    cpdef void update(self)