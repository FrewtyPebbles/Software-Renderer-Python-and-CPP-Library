import time
from PIL import Image, ImageDraw
from vec3 import Vec3
from camera import Camera, Screen
from polygon import Polygon
from mesh import Mesh
from object import Object

# BUG: sometimes test script freezes durring runtime.

dim = (500, 500)

# Use PIL as render medium
img = Image.new("RGB", dim)
img_draw = ImageDraw.Draw(img)



cam = Camera(Vec3(0.0,0.0,0.0), *dim, 200)
screen = Screen(500, 500)
tp = Mesh.from_obj("meshes/teapot.obj")
teapot = Object(tp, Vec3(0.0,0.0,100.0))
t1 = time.time()
teapot.render(cam, screen)
for x, y, color in cam.get_pixel():
    img_draw.point((x,y), color)
t2 = time.time()

print(f"time to render: {(t2-t1) * 1000:0.4f} ms")

img.save("test.png")