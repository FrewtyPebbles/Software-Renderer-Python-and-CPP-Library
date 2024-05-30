from os import getenv
from setuptools import Extension, setup
from Cython.Build import cythonize

C_PATH = "src/"

EXTENSIONS = [
    Extension('vec3', ["vec3.pyx", f"{C_PATH}Vec3.cpp"],
              extra_compile_args=['-O3']),
    Extension('tup', ["tup.pyx", f"{C_PATH}Tup.cpp"],
              extra_compile_args=['-O3']),
    Extension('polygon', ["polygon.pyx", f"{C_PATH}Polygon.cpp", f"{C_PATH}Vec3.cpp"],
              extra_compile_args=['-O3']),
    Extension('camera', ["camera.pyx", f"{C_PATH}Camera.cpp", f"{C_PATH}threadpool.cpp"],
              extra_compile_args=['-O3']),
    Extension('mesh', ["mesh.pyx", f"{C_PATH}Mesh.cpp", f"{C_PATH}Polygon.cpp", f"{C_PATH}Vec3.cpp"],
              extra_compile_args=['-O3']),
    Extension('object', ["object.pyx", f"{C_PATH}Object.cpp", f"{C_PATH}Mesh.cpp", f"{C_PATH}Polygon.cpp", f"{C_PATH}Vec3.cpp", f"{C_PATH}threadpool.cpp"],
              extra_compile_args=['-O3']),
    Extension('rasterize', ["rasterize.pyx"],
              extra_compile_args=['-O3']),
]

setup(
    name='3d_projection_mapper',
    ext_modules = cythonize(EXTENSIONS,
        compiler_directives={
            "language_level": 3,
            "profile": False
        }
    ),
)