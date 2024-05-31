from camera cimport Camera, pixel
from tup cimport tup3ui8, uint8
from cython.operator cimport dereference
from libcpp.vector cimport vector
from cython.parallel cimport prange

cpdef void rasterize(object img_draw, Camera cam):
    cdef:
        pixel pxl
        uint8* color
        vector[pixel] pixl_vec = cam.c_class.frame_buffer
        size_t fb_len = pixl_vec.size()
        size_t i
    with nogil:
        for i in prange(fb_len):
            pxl = pixl_vec[i]
            color = pxl.color.data
            with gil:
                img_draw.point((pxl.x,pxl.y), (color[0],color[1],color[2]))