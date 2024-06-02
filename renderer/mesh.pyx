# distutils: language = c++
from renderer.vec3 cimport Vec3
from renderer.polygon cimport Polygon
from libcpp.vector cimport vector



cdef class Mesh:
    # def __init__(self):
    #     self.c_class = new mesh()

    def __dealloc__(self):
        del self.c_class

    @staticmethod
    cdef Mesh from_cpp(mesh* cppinst):
        cdef Mesh ret = Mesh()
        ret.c_class = cppinst
        return ret

    @staticmethod
    def from_obj(file_path:str) -> Mesh:
        return _from_obj(file_path)

cpdef Mesh _from_obj(str file_path):
    cdef mesh* m = mesh.from_obj(file_path.encode())
    return Mesh.from_cpp(m)