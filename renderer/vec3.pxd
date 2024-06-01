# distutils: language = c++
from renderer.camera cimport camera, screen, Camera, Screen
from renderer.tup cimport tup2f, tup3f, tup3tup2f

cdef extern from "../src/Vec3.h":
    cdef cppclass vec3:
        vec3() except +
        vec3(float x, float y, float z) except +
        float x
        float y
        float z
        tup2f project(camera* camera, screen* screen)

        # operators
        vec3 operator+(vec3& other)
        vec3 operator+(float& other)
        vec3 operator-(vec3& other)
        vec3 operator-(float& other)
        vec3 operator*(vec3& other)
        vec3 operator*(float& other)
        vec3 operator/(vec3& other)
        vec3 operator/(float& other)
        float dot(vec3& other)
        float cross(vec3& other)


cdef class Vec3:
    cdef vec3 c_class

    cpdef (float, float) project(self, Camera camera_obj, Screen screen_obj)

    cpdef Vec3 vecadd(self, Vec3 other)

    cpdef Vec3 floatadd(self, float other)

    cpdef Vec3 vecsub(self, Vec3 other)

    cpdef Vec3 floatsub(self, float other)

    cpdef Vec3 vecmul(self, Vec3 other)

    cpdef Vec3 floatmul(self, float other)

    cpdef Vec3 vecdiv(self, Vec3 other)

    cpdef Vec3 floatdiv(self, float other)
    
cdef Vec3 _from_cpp(vec3 cppinst)