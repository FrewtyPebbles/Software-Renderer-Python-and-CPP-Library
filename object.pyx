# distutils: language = c++
from mesh cimport Mesh
from vec3 cimport Vec3
from camera cimport Camera, Screen


cdef class Object:
    def __init__(self, Mesh mesh, position:Vec3|None = None, rotation:Vec3|None = None, scale:Vec3|None = None) -> None:
        position = position if position != None else Vec3(0.0,0.0,0.0)
        rotation = rotation if rotation != None else Vec3(0.0,0.0,0.0)
        scale = scale if scale != None else Vec3(0.0,0.0,0.0)
        self.init(mesh, position, rotation, scale)
        # IT LIKELY IS BEING POP'D FROM THE EXECUTION STACK AFTER __init__ finishes.
        # Maybe try allocating c_class with new.

    cpdef void init(self, Mesh mesh, Vec3 position, Vec3 rotation, Vec3 scale):
        self.c_class = object3d(mesh.c_class, position.c_class, rotation.c_class, scale.c_class)

    cpdef void render(self, Camera camera, Screen screen):
        print(self.c_class.mesh_data[0].vertexes.size(), "VERT CYTH")
        # BUG: For some reason the mesh's attributes are zeroed after the init. are being cleared?
        self.c_class.render(camera.c_class, screen.c_class)

    