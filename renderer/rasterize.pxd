# distutils: language = c++

from renderer.camera cimport Camera

cpdef void render(object img_draw, Camera cam)