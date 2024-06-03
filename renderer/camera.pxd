# distutils: language = c++
from renderer.vec3 cimport vec3
from libcpp.vector cimport vector
from renderer.tup cimport tup3ui8
from libcpp.unordered_set cimport unordered_set
from renderer.object cimport object3d, Object
  
cdef extern from "../src/Camera.h":
    cdef cppclass pixel:
        pixel() except +
        pixel(int x, int y, tup3ui8 color) except +
        int x
        int y
        tup3ui8 color 
     
    cdef cppclass pixelHash:
        pixelHash() except +

    cdef cppclass screen:
        screen() except +
        screen(int width, int height) except +
        int width, height

    cdef cppclass camera:
        camera() except +
        camera(vec3* position, int view_width, int view_height, int focal_length, float fov) except +
        void render(vector[object3d*] objects, screen& screen)
        vec3 position
        int view_width, view_height, focal_length
        vector[vector[float]] depth_buffer
        vector[vector[float]] cleared_depth_buffer
        vector[pixel] frame_buffer
        float fov


cdef class Screen:
    cdef screen* c_class

cdef class Camera:
    cdef camera c_class

    cpdef void render(self, list[Object] objects, Screen screen)