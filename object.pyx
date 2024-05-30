# distutils: language = c++
from mesh cimport Mesh
from vec3 cimport Vec3
from camera cimport Camera, Screen


cdef class Object:
    def __init__(self, Mesh mesh_instance, Vec3 position = Vec3(0.0,0.0,0.0), Vec3 rotation = Vec3(0.0,0.0,0.0), Vec3 scale = Vec3(0.0,0.0,0.0)) -> None:
        c_class = object3d(mesh_instance.c_class, position.c_class, rotation.c_class, scale.c_class)
        self.c_class = c_class


    cpdef void render(self, Camera camera, Screen screen):
        self.c_class.render(camera.c_class, screen.c_class[0])



    