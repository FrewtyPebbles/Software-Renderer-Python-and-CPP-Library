# distutils: language = c++
from renderer.tup cimport tup3i, tup3f
from libcpp.vector cimport vector
from renderer.vec3 cimport vec3, Vec3
from renderer.polygon cimport polygon, Polygon
from libcpp.string cimport string

cdef extern from "opencv2/core.hpp" namespace "cv":
    cdef cppclass Mat:
        Mat() except +
        void create(int, int, int) except +
        void* data
        int rows
        int cols

cdef extern from "../src/Mesh.h":
    cdef enum illum_model:
        CONSTANT_COLOR, 
        DIFFUSE,
        DIFFUSE_AND_SPECULAR

    cdef cppclass material:
        material() except +
        tup3f ambient, diffuse, specular, emissive_coeficient
        float specular_exponent, optical_density, transparency
        illum_model illumination_model
        string ambient_tex_file, diffuse_tex_file, specular_highlight_file
        Mat ambient_texture, diffuse_texture, specular_highlight_texture

    cdef cppclass face:
        face() except +
        face(tup3i vertex_indicies, tup3i vertex_tex_coord_indices, tup3i normal_indicies)
        tup3i vertex_indicies, vertex_tex_coord_indices, normal_indicies

    cdef cppclass mesh:
        mesh() except +
        mesh(vector[vec3] vertexes, vector[tup3i] faces, vector[vec3] uv_vertexes, vector[vec3] vertex_normals, vector[vec3] materials) except +
        mesh(mesh& rhs) except +
        vector[polygon] get_polygons(vector[vec3] vertexes)

        @staticmethod
        mesh from_obj(string file_path)

        vector[vec3] vertexes
        vector[vec3] uv_vertexes
        vector[vec3] vertex_normals
        vector[face] faces
        vector[material] materials

cdef class Mesh:
    cdef mesh* c_class
    @staticmethod
    cdef Mesh from_cpp(mesh cppinst)

cpdef Mesh _from_obj(str file_path)