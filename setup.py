from os import getenv
from setuptools import Extension, setup
from Cython.Build import cythonize
from dotenv import load_dotenv

load_dotenv()

C_PATH = "src/"
INCLUDE_DIRS = [
    getenv("OPENCV_INC")
]
LIBRARY_DIRS = [
    getenv("OPENCV_LIB")
]
LIBRARIES = [
    'opencv_world490',
    'opencv_world490d'
]

EXTENSIONS = [
    Extension('vec3', ["vec3.pyx", f"{C_PATH}Vec3.cpp"],
              include_dirs=INCLUDE_DIRS, library_dirs=LIBRARY_DIRS, libraries=LIBRARIES),
    Extension('tup', ["tup.pyx", f"{C_PATH}Tup.cpp"],
              include_dirs=INCLUDE_DIRS, library_dirs=LIBRARY_DIRS, libraries=LIBRARIES),
    Extension('polygon', ["polygon.pyx", f"{C_PATH}Polygon.cpp", f"{C_PATH}Vec3.cpp"],
              include_dirs=INCLUDE_DIRS, library_dirs=LIBRARY_DIRS, libraries=LIBRARIES),
    Extension('camera', ["camera.pyx", f"{C_PATH}Camera.cpp", f"{C_PATH}threadpool.cpp"],
              include_dirs=INCLUDE_DIRS, library_dirs=LIBRARY_DIRS, libraries=LIBRARIES),
    Extension('mesh', ["mesh.pyx", f"{C_PATH}Mesh.cpp", f"{C_PATH}Polygon.cpp", f"{C_PATH}Vec3.cpp", f"{C_PATH}Tup.cpp"],
              include_dirs=INCLUDE_DIRS, library_dirs=LIBRARY_DIRS, libraries=LIBRARIES),
    Extension('object', ["object.pyx", f"{C_PATH}Object.cpp", f"{C_PATH}Mesh.cpp", f"{C_PATH}Polygon.cpp", f"{C_PATH}Vec3.cpp", f"{C_PATH}threadpool.cpp"],
              include_dirs=INCLUDE_DIRS, library_dirs=LIBRARY_DIRS, libraries=LIBRARIES),
    Extension('rasterize', ["rasterize.pyx"],
              include_dirs=INCLUDE_DIRS, library_dirs=LIBRARY_DIRS, libraries=LIBRARIES),
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