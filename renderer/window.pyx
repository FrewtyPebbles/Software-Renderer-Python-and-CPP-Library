# distutils: language = c++

cdef class Window:

    def __init__(self, str title, Camera cam, Screen scr, int width, int height) -> None:
        self.c_class = new window(title.encode(), cam.c_class, scr.c_class, width, height)
    
    @property
    def current_event(self):
        return self.c_class.current_event

    def __dealloc__(self):
        del self.c_class

    cpdef void update(self):
        self.c_class.update()