from os import getenv, listdir, path
from setuptools import Extension, setup
from Cython.Build import cythonize
from dotenv import load_dotenv

load_dotenv()

PY_PATH = "./"

C_PATH = path.join(path.dirname(__file__), "src/")

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
    Extension('renderer', [
        *[path.join(PY_PATH, pyxf) for pyxf in listdir(PY_PATH) if pyxf.endswith(".pyx")],
        *[path.join(C_PATH, cppf) for cppf in listdir(C_PATH) if cppf.endswith(".cpp")]
    ],
    include_dirs=INCLUDE_DIRS, library_dirs=LIBRARY_DIRS, libraries=LIBRARIES)
]

setup(
    name='renderer',
    ext_modules = cythonize(EXTENSIONS,
        compiler_directives={
            "language_level": 3,
            "profile": False
        }
    ),
)