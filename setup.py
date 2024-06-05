from os import getenv, listdir, path
from setuptools import setup, Extension
from Cython.Build import cythonize
from dotenv import load_dotenv

load_dotenv()

MODULE_NAME = "renderer"

PY_PATH = path.join(path.dirname(__file__), "renderer/")

C_PATH = path.join(path.dirname(__file__), "src/")

INCLUDE_DIRS = [
    getenv("INCLUDE_DIR"),
    path.join(path.dirname(__file__), "glad/include/")
]
LIBRARY_DIRS = [
    getenv("STATIC_LIB_DIR")
]

LIBRARIES = [f.replace(".lib", "") for f in listdir(getenv("STATIC_LIB_DIR")) if f.endswith(".lib")]

c_deps = [
    *[path.join(C_PATH, cppf) for cppf in listdir(C_PATH) if cppf.endswith(".cpp")],
    path.join(path.dirname(__file__), "glad/src/glad.c")
]
pyx_deps = [path.join(PY_PATH, cppf) for cppf in listdir(PY_PATH) if cppf.endswith(".pyx")]
print()
EXTENSIONS = [
    Extension(path.relpath(pyx_dep, path.dirname(__file__)).replace(".pyx", "").replace("\\", ".").replace("/", "."),
              sources=[pyx_dep, *c_deps],
              language="c++",
              include_dirs=INCLUDE_DIRS,
              libraries=LIBRARIES + [
                  "kernel32", "user32", "gdi32", "winmm", "imm32", "ole32", "oleaut32", "version", "uuid", "advapi32", "setupapi", "shell32", "dinput8"
              ],
              library_dirs=LIBRARY_DIRS,
              extra_compile_args=["/MT", "/std:c++20", "/MP", "/Ox"],
              extra_link_args=[]
              )
    for pyx_dep in pyx_deps
]


setup(
    name=MODULE_NAME,
    ext_modules = cythonize(EXTENSIONS,
        compiler_directives={
            "language_level": 3,
            "profile": False
        }
    ),
)