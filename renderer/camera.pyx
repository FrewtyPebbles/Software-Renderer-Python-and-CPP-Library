# distutils: language = c++
from renderer.vec3 cimport Vec3, vec3
from renderer.tup cimport tup3ui8
from cython.operator cimport dereference

cdef class Screen:
    def __cinit__(self, int width, int height) -> None:
        self.c_class = new screen(width, height)
    
    def __dealloc__(self):
        del self.c_class

        

cdef class Camera:
    def __init__(self, Vec3 position, int view_width, int view_height, int view_distance) -> None:
        self.c_class = camera(&position.c_class, view_width, view_height, view_distance)
