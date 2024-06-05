# distutils: language = c++
from renderer.vec3 cimport Vec3
from renderer.camera cimport Camera


cdef tup3tup2f conv_proj_tuples(PROJECTIONS projections):
    cdef:
        float[2] arr1 = [projections[0][0], projections[0][1]]
        float[2] arr2 = [projections[1][0], projections[1][1]]
        float[2] arr3 = [projections[2][0], projections[2][1]]
        tup2f[3] arr = [
        tup2f(arr1),
        tup2f(arr2),
        tup2f(arr3)
    ]
    return tup3tup2f(arr)

cdef class Polygon:

    def __init__(self, Vec3 A, Vec3 B, Vec3 C) -> None:
        self.c_class = polygon(A.c_class, B.c_class, C.c_class)

    def __repr__(self) -> str:
        return f"poly:<vec3:<{self.c_class.A.x},{self.c_class.A.y},{self.c_class.A.z}>vec3:<{self.c_class.B.x},{self.c_class.B.y},{self.c_class.B.z}>vec3:<{self.c_class.C.x},{self.c_class.C.y},{self.c_class.C.z}>>"

    @staticmethod
    cdef Polygon from_cpp(polygon cppinst):
        cdef Polygon ret = Polygon(Vec3(0.0,0.0,0.0),Vec3(0.0,0.0,0.0),Vec3(0.0,0.0,0.0))
        ret.c_class = cppinst
        return ret

    cpdef (int, int) get_vertical_bounds(self, PROJECTIONS projections, Camera camera, Screen screen):
        cdef tup2i res = self.c_class.get_vertical_bounds(conv_proj_tuples(projections), camera.c_class, screen.c_class)
        return res.data[0], res.data[1]

    cpdef (int, int) get_render_row_range(self, int y, PROJECTIONS projections, Camera camera, Screen screen):
        cdef tup2i res = self.c_class.get_render_row_range(y, conv_proj_tuples(projections), camera.c_class, screen.c_class)
        return res.data[0], res.data[1]

    cpdef PROJECTIONS project(self, Camera camera, Screen screen):

        cdef tup3tup2f res = self.c_class.project(camera.c_class, screen.c_class)

        return (
            (res.data[0].data[0], res.data[0].data[1]),
            (res.data[1].data[0], res.data[1].data[1]),
            (res.data[2].data[0], res.data[2].data[1])
        )

