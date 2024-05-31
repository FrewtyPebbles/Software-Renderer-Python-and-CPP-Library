# distutils: language = c++
from tup cimport tup3i, tup3f
from libcpp.vector cimport vector
from vec3 cimport vec3, Vec3
from polygon cimport polygon, Polygon
from libcpp.string cimport string

cdef extern from "opencv2/opencv.hpp" namespace "cv":
    cdef cppclass Mat:
        pass

cdef extern from "src/Mesh.h":
    cpdef enum class illum_model(int):
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
        mesh(vector[vec3] vertexes, vector[tup3i] polygon_inds) except +
        vector[polygon] get_polygons(vector[vec3] vertexes)

        @staticmethod
        mesh from_obj(string file_path)

        vector[vec3] vertexes
        vector[vec3] texture_vertexes
        vector[vec3] vertex_normals
        vector[face] faces
        vector[material] materials

cdef class Mesh:
    cdef mesh* c_class
    @staticmethod
    cdef Mesh from_cpp(mesh cppinst)

cpdef Mesh _from_obj(str file_path)