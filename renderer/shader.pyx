

cdef class Shader:
    
    def __init__(self, str source, ShaderType shader_type) -> None:
        self.c_class = shader(source, shader_type)