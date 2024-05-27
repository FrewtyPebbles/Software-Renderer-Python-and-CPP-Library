# distutils: language = c++
from vec3 cimport Vec3, vec3

cdef class Screen:
    def __init__(self, int width, int height) -> None:
        self.c_class = screen(width, height)

cdef class Camera:
    def __init__(self, Vec3 position, int view_width, int view_height, int view_distance) -> None:
        self.c_class = camera(position.c_class, view_width, view_height, view_distance)