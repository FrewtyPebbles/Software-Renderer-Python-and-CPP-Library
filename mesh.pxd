# distutils: language = c++
from tup cimport tup3i
from libcpp.vector cimport vector
from vec3 cimport vec3, Vec3
from polygon cimport polygon

cdef extern from "src/Mesh.h":
    cdef cppclass mesh:
        mesh() except +
        mesh(vector[vec3] vertexes, vector[tup3i] polygon_inds) except +
        vector[polygon] get_polygons(vector[vec3] vertexes)

        vector[vec3] vertexes
        vector[tup3i] polygon_inds

cdef class Mesh:
    cdef mesh c_class
    cpdef init(self, list[Vec3] vertexes, list[(int,int,int)] polygon_inds)
    cpdef list[Polygon] get_polygons(self, list[vec3] vertexes)