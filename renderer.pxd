# distutils: language = c++
from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp.map cimport map

cdef extern from "src/Shader.h":
    cpdef enum class ShaderType:
        FRAGMENT,
        VERTEX
    cdef cppclass shader:
        shader() except +
        shader(string source, ShaderType type) except +
        void compile() except +
        const char * source
        ShaderType type
        unsigned int shader_handle
 
cdef class Shader:
    cdef shader c_class


ctypedef unsigned char uint8

cdef extern from "src/Tup.h":
    cdef cppclass tup2f:
        tup2f() except +
        tup2f(float[2] values) except +
        float[2] data

    cdef cppclass tup3tup2f:
        tup3tup2f() except +
        tup3tup2f(tup2f[3] values) except +
        tup2f[3] data


    cdef cppclass tup3f:
        tup3f() except +
        tup3f(float[3] values) except +
        float[3] data

    cdef cppclass tup4f:
        tup4f() except +
        tup4f(float[4] values) except +
        float[4] data
    
    cdef cppclass tup2i:
        tup2i() except +
        tup2i(int[2] values) except +
        int[2] data

    cdef cppclass tup3tup2i:
        tup3tup2i() except +
        tup3tup2i(tup2i[3] values) except +
        tup2i[3] data

    cdef cppclass tup3i:
        tup3i() except +
        tup3i(int[3] values) except +
        int[3] data

    cdef cppclass tup4i:
        tup4i() except +
        tup4i(int[4] values) except +
        int[4] data

    cdef cppclass tup3ui8:
        tup3i() except +
        tup3i(uint8[3] values) except +
        uint8[3] data

cdef extern from "src/Vec3.h":
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
        float get_magnitude()
        vec3 get_normalized()

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
    
    cpdef float get_magnitude(self)

    cpdef Vec3 get_normalized(self)
    
cdef Vec3 vec_from_cpp(vec3 cppinst)


cdef extern from "opencv2/core.hpp" namespace "cv":
    cdef cppclass Mat:
        Mat() except +
        void create(int, int, int) except +
        void* data
        int rows
        int cols

ctypedef ((float, float),(float, float),(float, float)) PROJECTIONS
 
cdef extern from "src/Polygon.h":
    cdef cppclass polygon:
        polygon() except +
        polygon(vec3 A, vec3 B, vec3 C) except +
        polygon(vec3 A,vec3 B,vec3 C,vec3 A_tex,vec3 B_tex,vec3 C_tex,vec3 A_norm,vec3 B_norm,vec3 C_norm,mesh* mesh)
        vec3 A
        vec3 B
        vec3 C
        vec3 A_tex
        vec3 B_tex
        vec3 C_tex
        vec3 A_norm
        vec3 B_norm
        vec3 C_norm
        mesh* mesh_data
        void render(camera* camera, screen* screen)
        tup2i get_vertical_bounds(tup3tup2f projections, camera* camera, screen* screen)
        tup2i get_render_row_range(int y, tup3tup2f projections, camera* camera, screen* screen)
        tup3tup2f project(camera* camera, screen* screen)

cdef tup3tup2f conv_proj_tuples(PROJECTIONS projections)

cdef class Polygon:
    cdef polygon c_class

    @staticmethod
    cdef Polygon from_cpp(polygon cppinst)

    cpdef (int, int) get_vertical_bounds(self, PROJECTIONS projections, Camera camera, Screen screen)

    cpdef (int, int) get_render_row_range(self, int y, PROJECTIONS projections, Camera camera, Screen screen)

    cpdef PROJECTIONS project(self, Camera camera, Screen screen)

cdef extern from "src/Mesh.h":
    cdef enum illum_model:
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
        mesh(map[string, meshgroup]* groups, map[string, material*] materials, vector[vec3]* vertexes, vector[vec3]* uv_vertexes, vector[vec3]* vertex_normals) except +
        @staticmethod
        mesh* from_obj(string file_path)
        map[string, meshgroup]* groups
        map[string, material*] materials
        vector[vec3]* vertexes
        vector[vec3]* uv_vertexes
        vector[vec3]* vertex_normals

    cdef cppclass meshgroup:
        meshgroup() except +
        meshgroup(vector[vec3]* vertexes, vector[vec3]* uv_vertexes, vector[vec3]* vertex_normals) except +
        meshgroup(meshgroup& rhs) except +
        vector[polygon] get_polygons(vector[vec3] vertexes)


        vector[vec3]* vertexes
        vector[face] faces
        vector[vec3]* uv_vertexes
        vector[vec3]* vertex_normals
        material* materials

cdef class Mesh:
    cdef mesh* c_class

    @staticmethod
    cdef Mesh from_cpp(mesh* cppinst)
    
cpdef Mesh mesh_from_obj(str file_path)

cdef extern from "src/Object.h":
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

cdef extern from "src/Camera.h":
    cdef cppclass pixel:
        pixel() except +
        pixel(int x, int y, tup3ui8 color) except +
        int x
        int y
        tup3ui8 color 
     
    cdef cppclass pixelHash:
        pixelHash() except +

    cdef cppclass screen:
        screen() except +
        screen(int width, int height) except +
        int width, height

    cdef cppclass camera:
        camera() except +
        camera(vec3* position, int view_width, int view_height, int focal_length, float fov) except +
        void render(vector[object3d*] objects, screen& screen)
        vec3 position
        int view_width, view_height, focal_length
        vector[vector[float]] depth_buffer
        vector[vector[float]] cleared_depth_buffer
        vector[pixel] py_frame_buffer
        unsigned int* frame_buffer
        float fov


cdef class Screen:
    cdef screen* c_class

cdef class Camera:
    cdef camera* c_class

    cpdef void render(self, list[Object] objects, Screen screen)

cpdef void render_PIL(object img_draw, Camera cam)


cdef extern from "src/Window.h":
    cpdef enum class event:
        NOTHING,
        WINDOW_CLOSE,
        QUIT,
        KEY_UP,
        KEY_DOWN,
        KEY_RIGHT,
        KEY_LEFT,
        KEY_SPACE

    cdef cppclass window:
        window() except +
        window(string title, camera* cam, screen* scr, int width, int height) except +
        camera* cam
        screen* scr
        string title
        int width, height
        void update()
        event current_event

cdef class Window:
    cdef window* c_class

    cpdef void update(self)