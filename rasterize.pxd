# distutils: language = c++

from camera cimport Camera

cpdef void rasterize(object img_draw, Camera cam)