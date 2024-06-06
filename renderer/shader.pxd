# distutils: language = c++
from libcpp.string cimport string

cdef extern from "../src/Shader.h":
    cpdef enum class ShaderType:
        FRAGMENT,
        VERTEX
    cdef cppclass shader:
        shader() except +
        shader(string source, ShaderType type) except +
        void compile()
        const char * source
        ShaderType type
        unsigned int shader_handle

cdef class Shader:
    cdef shader c_class
