# distutils: language = c++

cdef class Window:

    def __init__(self, str title, Camera cam, Screen scr) -> None:
        cdef Py_ssize_t length = len(title)
        cdef wstring titl = wstring(
            PyUnicode_AsWideCharString(title, &length)
        )
        self.c_class = new window(titl, &cam.c_class, scr.c_class)
        

    def __dealloc__(self):
        del self.c_class

    cpdef void update(self):
        self.c_class.update()