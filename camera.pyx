# distutils: language = c++
from vec3 cimport Vec3, vec3
from tup cimport tup3ui8

cdef class Screen:
    def __init__(self, int width, int height) -> None:
        self.c_class = screen(width, height)

cdef class Camera:
    def __init__(self, Vec3 position, int view_width, int view_height, int view_distance) -> None:
        self.c_class = camera(position.c_class, view_width, view_height, view_distance)

    cpdef (int, int, int) get_pixel(self, int x, int y):
        cdef tup3ui8 tupl = self.c_class.frame_buffer[x][y]
        return <int>tupl.data[0], <int>tupl.data[1], <int>tupl.data[2]