# distutils: language = c++
from vec3 cimport Vec3, vec3
from tup cimport tup3ui8

cdef class Screen:
    def __init__(self, int width, int height) -> None:
        self.c_class = screen(width, height)

cdef class Camera:
    def __init__(self, Vec3 position, int view_width, int view_height, int view_distance) -> None:
        self.c_class = camera(position.c_class, view_width, view_height, view_distance)

    def  get_pixel(self) -> tuple[int, int, tuple[int, int, int]]:
        cdef pixel* pixl
        cdef tup3ui8 color
        while self.c_class.frame_buffer.size() > 0:
            pixl = &self.c_class.frame_buffer.back()
            self.c_class.frame_buffer.pop_back()
            color = pixl.color
            yield ( pixl.x, pixl.y, ( color.data[0], color.data[1], color.data[2] ) )
        
