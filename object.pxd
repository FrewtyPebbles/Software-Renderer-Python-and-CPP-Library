# distutils: language = c++
from mesh cimport mesh, Mesh
from vec3 cimport vec3, Vec3
from camera cimport camera, screen, Camera, Screen
from libcpp.vector cimport vector

cdef extern from "src/Object.h":
    cdef cppclass object3d:
        # alias for cpp class object since name object is reserved by python
        object3d() except +
        object3d(mesh& mesh, vec3 position, vec3 rotation, vec3 scale) except +
        mesh* mesh_data
        vec3 position, rotation, scale

        void render(camera& camera, screen& screen)
        
        vector[vec3] get_translation(vector[vec3] vertexes)

        vector[vec3] get_rotation(vector[vec3] vertexes)


cdef class Object:
    cdef object3d c_class

    
    cpdef void render(self, Camera camera, Screen screen)
        
    # cpdef list[Vec3] get_translation(list[Vec3] vertexes)

    # cpdef list[Vec3] get_rotation(list[Vec3] vertexes)