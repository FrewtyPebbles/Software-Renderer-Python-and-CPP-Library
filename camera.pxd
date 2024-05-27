# distutils: language = c++
from vec3 cimport vec3
from libcpp.vector cimport vector

cdef extern from "src/Camera.h":
    cdef cppclass screen:
        screen() except +
        screen(int width, int height) except +
        int width, height

    cdef cppclass camera:
        camera() except +
        camera(vec3& position, int view_width, int view_height, int view_distance) except +
        int view_width, view_height, view_distance
        vec3 position
        vector[vector[float]] depth_buffer
        vector[vector[float]] cleared_depth_buffer


cdef class Screen:
    cdef screen c_class

cdef class Camera:
    cdef camera c_class