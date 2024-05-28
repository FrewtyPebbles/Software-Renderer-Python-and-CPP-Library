# distutils: language = c++
from vec3 cimport Vec3
from polygon cimport Polygon



cdef class Mesh:
    def __init__(self, list[Vec3] vertexes, list[(int,int,int)] polygon_inds):
        self.init(vertexes, polygon_inds)

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


    cpdef init(self, list[Vec3] vertexes, list[(int,int,int)] polygon_inds):
        self.c_class = mesh()
        cdef Vec3 vec
        for vec in vertexes:
            self.c_class.vertexes.push_back(vec.c_class)
        cdef int[3] cind_arr
        cdef tup3i cind
        cdef (int,int,int) ind
        for ind in polygon_inds:
            cind_arr = [ind[0], ind[1], ind[2]]
            cind = tup3i(cind_arr)
            self.c_class.polygon_inds.push_back(cind)

    @property
    def polygons(self) -> list[Polygon]:
        cdef:
            vector[polygon] pgons = self.c_class.polygons
            list[Polygon] ret = []
            Polygon POLY
        for pgon in pgons:
            POLY = Polygon.from_cpp(pgon)
            ret.append(POLY)
            
        return ret

    @staticmethod
    cdef Mesh from_cpp(mesh cppinst):
        cdef Mesh ret = Mesh([],[])
        ret.c_class = cppinst
        return ret

    @staticmethod
    def from_obj(file_path:str) -> Mesh:
        return _from_obj(file_path)

cpdef Mesh _from_obj(str file_path):
    return Mesh.from_cpp(mesh.from_obj(file_path.encode()))