# distutils: language = c++
from mesh cimport Mesh
from vec3 cimport Vec3, vec3
from camera cimport Camera, Screen

cimport vec3 as v3

cdef class V3Property:

    @staticmethod
    cdef V3Property init(vec3* ptr):
        ret = V3Property()
        ret.c_class = ptr
        return ret


    @property
    def x(self):
        return self.c_class[0].x
    
    @property
    def y(self):
        return self.c_class[0].y

    @property
    def z(self):
        return self.c_class[0].z

    @x.setter
    def x(self, float value):
        self.c_class[0].x = value
    
    @y.setter
    def y(self, float value):
        self.c_class[0].y = value

    @z.setter
    def z(self, float value):
        self.c_class[0].z = value

    def __add__(self, other:Vec3 | float) -> Vec3:
        if isinstance(other, Vec3):
            return self.vecadd(other)
        else:
            return self.floatadd(other)

    cpdef Vec3 vecadd(self, Vec3 other):
        cdef vec3 s = self.c_class[0]
        return v3._from_cpp(s + other.c_class)

    cpdef Vec3 floatadd(self, float other):
        cdef vec3 s = self.c_class[0]
        return v3._from_cpp(s + other)

    def __sub__(self, other:Vec3 | float) -> Vec3:
        if isinstance(other, Vec3):
            return self.vecsub(other)
        else:
            return self.floatsub(other)

    cpdef Vec3 vecsub(self, Vec3 other):
        cdef vec3 s = self.c_class[0]
        return v3._from_cpp(s - other.c_class)

    cpdef Vec3 floatsub(self, float other):
        cdef vec3 s = self.c_class[0]
        return v3._from_cpp(s - other)

    def __mul__(self, other:Vec3 | float) -> Vec3:
        if isinstance(other, Vec3):
            return self.vecmul(other)
        else:
            return self.floatmul(other)
        
    cpdef Vec3 vecmul(self, Vec3 other):
        cdef vec3 s = self.c_class[0]
        return v3._from_cpp(s * other.c_class)

    cpdef Vec3 floatmul(self, float other):
        cdef vec3 s = self.c_class[0]
        return v3._from_cpp(s * other)

    def __truediv__(self, other:Vec3 | float) -> Vec3:
        if isinstance(other, Vec3):
            return self.vecdiv(other)
        else:
            return self.floatdiv(other)
        
    cpdef Vec3 vecdiv(self, Vec3 other):
        cdef vec3 s = self.c_class[0]
        return v3._from_cpp(s / other.c_class)

    cpdef Vec3 floatdiv(self, float other):
        cdef vec3 s = self.c_class[0]
        return v3._from_cpp(s / other)

    def dot(self, Vec3 other) -> float:
        return self.c_class[0].dot(other.c_class)

    def dot(self, Vec3 other) -> float:
        return self.c_class[0].cross(other.c_class)

cdef class Object:
    def __init__(self, Mesh mesh_instance, Vec3 position = Vec3(0.0,0.0,0.0), Vec3 rotation = Vec3(0.0,0.0,0.0), Vec3 scale = Vec3(0.0,0.0,0.0)) -> None:
        self.mesh = mesh_instance
        self.c_class = object3d(mesh_instance.c_class, position.c_class, rotation.c_class, scale.c_class)

    @property
    def position(self) -> V3Property:
        return V3Property.init(&self.c_class.position)

    @position.setter
    def position(self, other:Vec3 | V3Property):
        if isinstance(other, Vec3):
            self.set_position_vec(other)
        elif isinstance(other, V3Property):
            self.set_position_prop(other)

    cpdef void set_position_vec(self, Vec3 other):
        cdef vec3 o = other.c_class
        self.c_class.position = o

    cpdef void set_position_prop(self, V3Property other):
        cdef vec3 o = other.c_class[0]
        self.c_class.position = o

    @property
    def rotation(self) -> V3Property:
        return V3Property.init(&self.c_class.rotation)

    @rotation.setter
    def rotation(self, other:Vec3 | V3Property):
        if isinstance(other, Vec3):
            self.set_rotation_vec(other)
        elif isinstance(other, V3Property):
            self.set_rotation_prop(other)

    cpdef void set_rotation_vec(self, Vec3 other):
        cdef vec3 o = other.c_class
        self.c_class.rotation = o

    cpdef void set_rotation_prop(self, V3Property other):
        cdef vec3 o = other.c_class[0]
        self.c_class.rotation = o

    @property
    def scale(self) -> V3Property:
        return V3Property.init(&self.c_class.scale)

    @scale.setter
    def scale(self, other:Vec3 | V3Property):
        if isinstance(other, Vec3):
            self.set_scale_vec(other)
        elif isinstance(other, V3Property):
            self.set_scale_prop(other)

    cpdef void set_scale_vec(self, Vec3 other):
        cdef vec3 o = other.c_class
        self.c_class.scale = o

    cpdef void set_scale_prop(self, V3Property other):
        cdef vec3 o = other.c_class[0]
        self.c_class.scale = o


    cpdef void render(self, Camera camera, Screen screen):
        self.c_class.render(camera.c_class, screen.c_class[0])



    