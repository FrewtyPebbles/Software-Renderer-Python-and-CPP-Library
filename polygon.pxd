# distutils: language = c++
from vec3 cimport vec3
from tup cimport tup2i, tup2f, tup3tup2f
from camera cimport camera, Camera, screen, Screen
from libcpp.vector cimport vector

ctypedef ((float, float),(float, float),(float, float)) PROJECTIONS

cdef extern from "src/Polygon.h":
    cdef cppclass polygon:
        polygon() except +
        polygon(vec3 A, vec3 B, vec3 C) except +
        polygon(vec3 A,vec3 B,vec3 C,vec3 A_tex,vec3 B_tex,vec3 C_tex,vec3 A_norm,vec3 B_norm,vec3 C_norm)
        vec3 A
        vec3 B
        vec3 C
        vec3 A_tex
        vec3 B_tex
        vec3 C_tex
        vec3 A_norm
        vec3 B_norm
        vec3 C_norm
        void render(camera* camera, screen* screen)
        tup2i get_vertical_bounds(tup3tup2f projections, camera* camera, screen* screen)
        tup2i get_render_row_range(int y, tup3tup2f projections, camera* camera, screen* screen)
        tup3tup2f project(camera* camera, screen* screen)

cdef tup3tup2f conv_proj_tuples(PROJECTIONS projections)

cdef class Polygon:
    cdef polygon c_class

    @staticmethod
    cdef Polygon from_cpp(polygon cppinst)

    cpdef (int, int) get_vertical_bounds(self, PROJECTIONS projections, Camera camera, Screen screen)

    cpdef (int, int) get_render_row_range(self, int y, PROJECTIONS projections, Camera camera, Screen screen)

    cpdef PROJECTIONS project(self, Camera camera, Screen screen)