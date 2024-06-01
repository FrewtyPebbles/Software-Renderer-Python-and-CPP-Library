# distutils: language = c++

ctypedef unsigned char uint8

cdef extern from "../src/Tup.h":
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

