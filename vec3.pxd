# distutils: language = c++
from camera cimport camera, screen, Camera, Screen
from tup cimport tup2f, tup3f, tup3tup2f

cdef extern from "src/Vec3.h":
    cdef cppclass vec3:
        vec3() except +
        vec3(float x, float y, float z) except +
        float x
        float y
        float z
        tup2f project(camera& camera, screen& screen)

cdef class Vec3:
    cdef vec3 c_class

    cpdef (float, float) project(self, Camera camera_obj, Screen screen_obj)