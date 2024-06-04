# distutils: language = c++
from renderer.camera cimport camera, Camera, screen, Screen

cdef extern from "string":
    cdef cppclass wchar_t:
        pass
    
    cdef cppclass wstring:
        wstring()
        wstring(wchar_t*)

cdef extern from "Python.h":
    wchar_t* PyUnicode_AsWideCharString(object, Py_ssize_t *)

cdef extern from "../src/Window.h":
    cdef cppclass window:
        window() except +
        window(wstring title, camera* cam, screen* scr) except +
        void update()
        camera* cam
        screen* scr
        wstring title

cdef class Window:
    cdef window* c_class

    cpdef void update(self)