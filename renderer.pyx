# distutils: language = c++
from cython.parallel cimport prange

cdef class Screen:
    def __cinit__(self, int width, int height) -> None:
        self.c_class = new screen(width, height)
    
    def __dealloc__(self):
        del self.c_class

        

cdef class Camera:
    def __init__(self, Vec3 position, int view_width, int view_height, int focal_length, float fov) -> None:
        self.c_class = new camera(&position.c_class, view_width, view_height, focal_length, fov)

    def __dealloc__(self):
        del self.c_class

    cpdef void render(self, list[Object] objects, Screen screen):
        cdef:
            vector[object3d*] vec
            Object obj
        for obj in objects:
            vec.push_back(&obj.c_class)
        self.c_class.render(vec, screen.c_class[0])


cdef class Mesh:
    # def __init__(self):
    #     self.c_class = new mesh()

    def __dealloc__(self):
        del self.c_class

    @staticmethod
    cdef Mesh from_cpp(mesh* cppinst):
        cdef Mesh ret = Mesh()
        ret.c_class = cppinst
        return ret

    @staticmethod
    def from_obj(file_path:str) -> Mesh:
        return mesh_from_obj(file_path)

cpdef Mesh mesh_from_obj(str file_path):
    cdef mesh* m = mesh.from_obj(file_path.encode())
    return Mesh.from_cpp(m)


cdef class V3Property:
    # THIS NEEDS TO HAVE ALL OF THE SAME METHODS AND PROPERTIES AS Vec3
    @staticmethod
    cdef V3Property init(vec3* ptr):
        ret = V3Property()
        ret.c_class = ptr
        return ret

    cpdef float get_magnitude(self):
        return self.c_class.get_magnitude()

    cpdef Vec3 get_normalized(self):
        return vec_from_cpp(self.c_class.get_normalized())

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
        return vec_from_cpp(s + other.c_class)

    cpdef Vec3 floatadd(self, float other):
        cdef vec3 s = self.c_class[0]
        return vec_from_cpp(s + other)

    def __sub__(self, other:Vec3 | float) -> Vec3:
        if isinstance(other, Vec3):
            return self.vecsub(other)
        else:
            return self.floatsub(other)

    cpdef Vec3 vecsub(self, Vec3 other):
        cdef vec3 s = self.c_class[0]
        return vec_from_cpp(s - other.c_class)

    cpdef Vec3 floatsub(self, float other):
        cdef vec3 s = self.c_class[0]
        return vec_from_cpp(s - other)

    def __mul__(self, other:Vec3 | float) -> Vec3:
        if isinstance(other, Vec3):
            return self.vecmul(other)
        else:
            return self.floatmul(other)
        
    cpdef Vec3 vecmul(self, Vec3 other):
        cdef vec3 s = self.c_class[0]
        return vec_from_cpp(s * other.c_class)

    cpdef Vec3 floatmul(self, float other):
        cdef vec3 s = self.c_class[0]
        return vec_from_cpp(s * other)

    def __truediv__(self, other:Vec3 | float) -> Vec3:
        if isinstance(other, Vec3):
            return self.vecdiv(other)
        else:
            return self.floatdiv(other)
        
    cpdef Vec3 vecdiv(self, Vec3 other):
        cdef vec3 s = self.c_class[0]
        return vec_from_cpp(s / other.c_class)

    cpdef Vec3 floatdiv(self, float other):
        cdef vec3 s = self.c_class[0]
        return vec_from_cpp(s / other)

    def dot(self, Vec3 other) -> float:
        return self.c_class[0].dot(other.c_class)

    def dot(self, Vec3 other) -> float:
        return self.c_class[0].cross(other.c_class)

cdef class Object:
    def __init__(self, Mesh mesh_instance, Vec3 position = Vec3(0.0,0.0,0.0), Vec3 rotation = Vec3(0.0,0.0,0.0), Vec3 scale = Vec3(0.0,0.0,0.0)) -> None:
        self.mesh = mesh_instance
        self.c_class = object3d(mesh_instance.c_class, position.c_class, rotation.c_class, scale.c_class)

    cpdef void link_shaders(self):
        self.c_class.link_shaders()

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
        self.c_class.render(camera.c_class[0], screen.c_class[0])


cdef tup3tup2f conv_proj_tuples(PROJECTIONS projections):
    cdef:
        float[2] arr1 = [projections[0][0], projections[0][1]]
        float[2] arr2 = [projections[1][0], projections[1][1]]
        float[2] arr3 = [projections[2][0], projections[2][1]]
        tup2f[3] arr = [
        tup2f(arr1),
        tup2f(arr2),
        tup2f(arr3)
    ]
    return tup3tup2f(arr)

cdef class Polygon:

    def __init__(self, Vec3 A, Vec3 B, Vec3 C) -> None:
        self.c_class = polygon(A.c_class, B.c_class, C.c_class)

    def __repr__(self) -> str:
        return f"poly:<vec3:<{self.c_class.A.x},{self.c_class.A.y},{self.c_class.A.z}>vec3:<{self.c_class.B.x},{self.c_class.B.y},{self.c_class.B.z}>vec3:<{self.c_class.C.x},{self.c_class.C.y},{self.c_class.C.z}>>"

    @staticmethod
    cdef Polygon from_cpp(polygon cppinst):
        cdef Polygon ret = Polygon(Vec3(0.0,0.0,0.0),Vec3(0.0,0.0,0.0),Vec3(0.0,0.0,0.0))
        ret.c_class = cppinst
        return ret

    cpdef (int, int) get_vertical_bounds(self, PROJECTIONS projections, Camera camera, Screen screen):
        cdef tup2i res = self.c_class.get_vertical_bounds(conv_proj_tuples(projections), camera.c_class, screen.c_class)
        return res.data[0], res.data[1]

    cpdef (int, int) get_render_row_range(self, int y, PROJECTIONS projections, Camera camera, Screen screen):
        cdef tup2i res = self.c_class.get_render_row_range(y, conv_proj_tuples(projections), camera.c_class, screen.c_class)
        return res.data[0], res.data[1]

    cpdef PROJECTIONS project(self, Camera camera, Screen screen):

        cdef tup3tup2f res = self.c_class.project(camera.c_class, screen.c_class)

        return (
            (res.data[0].data[0], res.data[0].data[1]),
            (res.data[1].data[0], res.data[1].data[1]),
            (res.data[2].data[0], res.data[2].data[1])
        )

cpdef void render_PIL(object img_draw, Camera cam):
    cdef:
        pixel pxl
        uint8* color
        vector[pixel] pixl_vec = cam.c_class.py_frame_buffer
        size_t fb_len = pixl_vec.size()
        size_t i
    with nogil:
        for i in prange(fb_len):
            pxl = pixl_vec[i]
            color = pxl.color.data
            with gil:
                img_draw.point((pxl.x,pxl.y), (color[0],color[1],color[2]))

cdef class Shader:
    
    def __init__(self, str source, ShaderType shader_type) -> None:
        self.c_class = shader(source, shader_type)

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
        return vec_from_cpp(self.c_class + other.c_class)

    cpdef Vec3 floatadd(self, float other):
        return vec_from_cpp(self.c_class + other)

    def __sub__(self, other:Vec3 | float) -> Vec3:
        if isinstance(other, Vec3):
            return self.vecsub(other)
        else:
            return self.floatsub(other)

    cpdef Vec3 vecsub(self, Vec3 other):
        return vec_from_cpp(self.c_class - other.c_class)

    cpdef Vec3 floatsub(self, float other):
        return vec_from_cpp(self.c_class - other)

    def __mul__(self, other:Vec3 | float) -> Vec3:
        if isinstance(other, Vec3):
            return self.vecmul(other)
        else:
            return self.floatmul(other)
        
    cpdef Vec3 vecmul(self, Vec3 other):
        return vec_from_cpp(self.c_class * other.c_class)

    cpdef Vec3 floatmul(self, float other):
        return vec_from_cpp(self.c_class * other)

    def __truediv__(self, other:Vec3 | float) -> Vec3:
        if isinstance(other, Vec3):
            return self.vecdiv(other)
        else:
            return self.floatdiv(other)
        
    cpdef Vec3 vecdiv(self, Vec3 other):
        return vec_from_cpp(self.c_class / other.c_class)

    cpdef Vec3 floatdiv(self, float other):
        return vec_from_cpp(self.c_class / other)

    def dot(self, Vec3 other) -> float:
        return self.c_class.dot(other.c_class)

    def dot(self, Vec3 other) -> float:
        return self.c_class.cross(other.c_class)

    cpdef float get_magnitude(self):
        return self.c_class.get_magnitude()

    cpdef Vec3 get_normalized(self):
        return vec_from_cpp(self.c_class.get_normalized())

cdef Vec3 vec_from_cpp(vec3 cppinst):
    return Vec3(cppinst.x, cppinst.y, cppinst.z)


cdef class Window:

    def __init__(self, str title, Camera cam, Screen scr, int width, int height) -> None:
        self.c_class = new window(title.encode(), cam.c_class, scr.c_class, width, height)
    
    @property
    def current_event(self):
        return self.c_class.current_event

    def __dealloc__(self):
        del self.c_class

    cpdef void update(self):
        self.c_class.update()
