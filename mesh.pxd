# distutils: language = c++
from vec3 cimport vec3
from tup cimport tup2i, tup2f, tup3tup2f
from camera cimport camera, Camera, screen, Screen
from libcpp.vector cimport vector

ctypedef ((float, float),(float, float),(float, float)) PROJECTIONS

cdef extern from "src/Mesh.h":
    cdef cppclass polygon:
        polygon() except +
        polygon(vec3& A, vec3& B, vec3& C) except +
        vec3* A
        vec3* B
        vec3* C
        tup2i get_vertical_bounds(tup3tup2f projections, camera& camera)
        tup2i get_render_row_range(int y, tup3tup2f projections)
        tup3tup2f project(camera* camera, screen* screen)

cdef tup3tup2f conv_proj_tuples(PROJECTIONS projections)

cdef class Polygon:
    cdef polygon c_class

    cpdef (int, int) get_vertical_bounds(self, PROJECTIONS projections, Camera camera)

    cpdef (int, int) get_render_row_range(self, int y, PROJECTIONS projections)

    cpdef PROJECTIONS project(self, Camera camera, Screen screen)