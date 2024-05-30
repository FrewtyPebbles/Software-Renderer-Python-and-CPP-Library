import time
from PIL import Image, ImageDraw
from vec3 import Vec3
from camera import Camera, Screen
from polygon import Polygon
from mesh import Mesh
from object import Object
from rasterize import rasterize

# TODO: trace your steps carefully through the render pipeline and figure out what is breaking.

dim = (500, 500)

# Use PIL as render medium
img = Image.new("RGB", dim)
img_draw = ImageDraw.Draw(img)



cam = Camera(Vec3(0.0,0.0,0.0), *dim, 200)
print("create screen")
screen = Screen(500, 500)
print("load mesh")
tp = Mesh.from_obj("meshes/teapot.obj")
teapot = Object(tp, Vec3(0.0,-50.0,100.0))
print("Loaded mesh!")
t1 = time.time()
teapot.render(cam, screen)
rasterize(img_draw, cam)
t2 = time.time()

print(f"time to render: {(t2-t1) * 1000:0.4f} ms")

img.save("test.png")