from os import getenv, listdir, path
from setuptools import Extension, setup
from Cython.Build import cythonize
from dotenv import load_dotenv

load_dotenv()

PY_PATH = path.dirname(__file__)

C_PATH = path.join(path.dirname(__file__), "src/")

INCLUDE_DIRS = [
    getenv("INC_DIR")
]
LIBRARY_DIRS = [
    getenv("LIB_DIR")
]
LIBRARIES = [
    'opencv_core4', 'opencv_highgui4', "opencv_imgcodecs4", "opencv_imgproc4"
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