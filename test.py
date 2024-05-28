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

# my_mesh = Mesh(
#     [
#         Vec3(2, 1, 3.5),
#         Vec3(-1, 0, 3.0),
#         Vec3(1, 7, 5.0),
#         Vec3(-2, -2, 4.0),
#         Vec3(-4, -6, 4.0),
#         Vec3(2, -1, 4.0)
#     ],
#     [
#         (0,1,2),
#         (2,3,4),
#         (3,4,5)
#     ]
# )
teapot = Object(Mesh.from_obj("meshes/teapot.obj"))
t1 = time.time()
teapot.render(cam, screen)
for x in range(dim[0]):
    for y in range(dim[1]):
        #print(cam.get_pixel(x, y))
        img_draw.point((x,y), cam.get_pixel(x, y))
t2 = time.time()

#print(f"time to render: {(t2-t1) * 1000:0.4f} ms")

img.save("test.png")