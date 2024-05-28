# distutils: language = c++

cdef class Vec3:
    def __init__(self, float x, float y, float z) -> None:
        self.c_class = vec3(x,y,z)

    @property
    def x(self):
        return self.c_class.x

    @x.setter
    def x(self, float value):
        self.c_class.x = value

    @property
    def y(self):
        return self.c_class.y

    @y.setter
    def y(self, float value):
        self.c_class.y = value

    @property
    def z(self):
        return self.c_class.z

    @z.setter
    def z(self, float value):
        self.c_class.z = value

    cpdef (float, float) project(self, Camera camera_obj, Screen screen_obj):
        cdef tup2f res = self.c_class.project(camera_obj.c_class, screen_obj.c_class)
        return res.data[0], res.data[1]

    # OPERATORS

    def __add__(self, other:Vec3 | float) -> Vec3:
        if isinstance(other, Vec3):
            return self.vecadd(other)
        else:
            return self.floatadd(other)

    cpdef Vec3 vecadd(self, Vec3 other):
        return _from_cpp(self.c_class + other.c_class)

    cpdef Vec3 floatadd(self, float other):
        return _from_cpp(self.c_class + other)

    def __sub__(self, other:Vec3 | float) -> Vec3:
        if isinstance(other, Vec3):
            return self.vecsub(other)
        else:
            return self.floatsub(other)

    cpdef Vec3 vecsub(self, Vec3 other):
        return _from_cpp(self.c_class - other.c_class)

    cpdef Vec3 floatsub(self, float other):
        return _from_cpp(self.c_class - other)

    def __mul__(self, other:Vec3 | float) -> Vec3:
        if isinstance(other, Vec3):
            return self.vecmul(other)
        else:
            return self.floatmul(other)
        
    cpdef Vec3 vecmul(self, Vec3 other):
        return _from_cpp(self.c_class * other.c_class)

    cpdef Vec3 floatmul(self, float other):
        return _from_cpp(self.c_class * other)

    def __truediv__(self, other:Vec3 | float) -> Vec3:
        if isinstance(other, Vec3):
            return self.vecdiv(other)
        else:
            return self.floatdiv(other)
        
    cpdef Vec3 vecdiv(self, Vec3 other):
        return _from_cpp(self.c_class / other.c_class)

    cpdef Vec3 floatdiv(self, float other):
        return _from_cpp(self.c_class / other)

    def dot(self, Vec3 other) -> float:
        return self.c_class.dot(other.c_class)

    def dot(self, Vec3 other) -> float:
        return self.c_class.cross(other.c_class)

cdef Vec3 _from_cpp(vec3 cppinst):
    return Vec3(cppinst.x, cppinst.y, cppinst.z)
