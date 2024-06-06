# distutils: language = c++
from renderer.mesh cimport mesh, Mesh
from renderer.vec3 cimport vec3, Vec3
from renderer.camera cimport camera, screen, Camera, Screen
from libcpp.vector cimport vector
from renderer.shader cimport shader, Shader, ShaderType

cdef extern from "../src/Object.h":
    cdef cppclass object3d:
        # alias for cpp class object since name object is reserved by python
        object3d() except +
        object3d(mesh* mesh, vec3 position, vec3 rotation, vec3 scale) except +
        object(mesh* mesh, vec3 position, vec3 rotation, vec3 scale, shader vertex_shader) except +
        object(mesh* mesh, vec3 position, vec3 rotation, vec3 scale, shader vertex_shader, shader fragment_shader) except +
        mesh* mesh_data
        vec3 position
        vec3 rotation
        vec3 scale

        void render(camera& camera, screen& screen)
        
        vector[vec3] get_translation(vector[vec3] vertexes)

        vector[vec3] get_rotation(vector[vec3] vertexes)
        void link_shaders() except +


cdef class Object:
    cdef object3d c_class
    cdef Mesh mesh

    cpdef void link_shaders(self)
    
    cpdef void render(self, Camera camera, Screen screen)
        
    cpdef void set_scale_prop(self, V3Property other)

    cpdef void set_scale_vec(self, Vec3 other)

    cpdef void set_rotation_vec(self, Vec3 other)

    cpdef void set_rotation_prop(self, V3Property other)

    cpdef void set_position_vec(self, Vec3 other)

    cpdef void set_position_prop(self, V3Property other)

cdef class V3Property:
    cdef vec3* c_class

    @staticmethod
    cdef V3Property init(vec3* ptr)

    cpdef float get_magnitude(self)

    cpdef Vec3 get_normalized(self)

    cpdef Vec3 vecadd(self, Vec3 other)

    cpdef Vec3 floatadd(self, float other)

    cpdef Vec3 vecsub(self, Vec3 other)

    cpdef Vec3 floatsub(self, float other)

    cpdef Vec3 vecmul(self, Vec3 other)

    cpdef Vec3 floatmul(self, float other)

    cpdef Vec3 vecdiv(self, Vec3 other)

    cpdef Vec3 floatdiv(self, float other)