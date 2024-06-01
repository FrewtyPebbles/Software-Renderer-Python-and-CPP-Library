# distutils: language = c++
from renderer.vec3 cimport Vec3
from renderer.polygon cimport Polygon
from libcpp.vector cimport vector



cdef class Mesh:
    def __init__(self):
        self.c_class = new mesh()

    def __dealloc__(self):
        del self.c_class

    @property
    def vertexes(self) -> list[Vec3]:
        cdef list[Vec3] ret = []
        cdef Vec3 current_vec
        for vec in self.c_class.vertexes:
            current_vec = Vec3(0.0, 0.0, 0.0)
            current_vec.c_class.x = vec.x
            current_vec.c_class.y = vec.y
            current_vec.c_class.z = vec.z
            ret.append(current_vec)
        return ret

    @staticmethod
    cdef Mesh from_cpp(mesh cppinst):
        cdef Mesh ret = Mesh()
        ret.c_class.materials = cppinst.materials
        ret.c_class.faces = cppinst.faces
        ret.c_class.vertexes = cppinst.vertexes
        ret.c_class.vertex_normals = cppinst.vertex_normals
        ret.c_class.uv_vertexes = cppinst.uv_vertexes
        return ret

    @staticmethod
    def from_obj(file_path:str) -> Mesh:
        return _from_obj(file_path)

cpdef Mesh _from_obj(str file_path):
    return Mesh.from_cpp(mesh.from_obj(file_path.encode()))