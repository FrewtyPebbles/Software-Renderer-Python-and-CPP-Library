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
