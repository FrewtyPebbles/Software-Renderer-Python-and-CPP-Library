# distutils: language = c++
from renderer.vec3 cimport Vec3, vec3
from renderer.tup cimport tup3ui8
from cython.operator cimport dereference
from libcpp.vector cimport vector

cdef class Screen:
    def __cinit__(self, int width, int height) -> None:
        self.c_class = new screen(width, height)
    
    def __dealloc__(self):
        del self.c_class

        

cdef class Camera:
    def __init__(self, Vec3 position, int view_width, int view_height, int focal_length, float fov) -> None:
        self.c_class = new camera(&position.c_class, view_width, view_height, focal_length, fov)

    def __dealloc__(self):
        del self.c_class

    cpdef void render(self, list[Object] objects, Screen screen):
        cdef:
            vector[object3d*] vec
            Object obj
        for obj in objects:
            vec.push_back(&obj.c_class)
        self.c_class.render(vec, screen.c_class[0])
