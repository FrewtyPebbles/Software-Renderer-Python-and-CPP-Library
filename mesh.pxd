# distutils: language = c++
from tup cimport tup3i
from libcpp.vector cimport vector
from vec3 cimport vec3, Vec3
from polygon cimport polygon, Polygon
from libcpp.string cimport string

cdef extern from "src/Mesh.h":
    cdef cppclass mesh:
        mesh() except +
        mesh(vector[vec3] vertexes, vector[tup3i] polygon_inds) except +
        vector[polygon] get_polygons(vector[vec3] vertexes)

        @staticmethod
        mesh from_obj(string file_path)

        vector[vec3] vertexes
        vector[tup3i] polygon_inds
        vector[polygon] polygons

cdef class Mesh:
    cdef mesh c_class
    cpdef init(self, list[Vec3] vertexes, list[(int,int,int)] polygon_inds)
    @staticmethod
    cdef Mesh from_cpp(mesh cppinst)

cpdef Mesh _from_obj(str file_path)