from camera cimport Camera, pixel
from tup cimport tup3ui8
from cython.operator cimport dereference
cpdef void rasterize(object img_draw, Camera cam):
    cdef:
        pixel pxl
        tup3ui8 color
        size_t fb_len = cam.c_class.frame_buffer.size()
    for pxl in cam.c_class.frame_buffer:
        color = pxl.color
        img_draw.point((pxl.x,pxl.y), (color.data[0],color.data[1],color.data[2]))