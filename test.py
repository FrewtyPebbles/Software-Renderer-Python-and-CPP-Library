import time
from PIL import Image, ImageDraw
from vec3 import Vec3
from camera import Camera, Screen
from polygon import Polygon
from mesh import Mesh

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
teapot = Mesh.from_obj("meshes/teapot.obj")
t1 = time.time()
for poly in teapot.polygons:
    projections = poly.project(cam, screen)
    proj1, proj2, proj3= projections
    #print("get bounds")
    min_y, max_y = poly.get_vertical_bounds(projections, cam)
    #print(min_y, max_y)
    for y in range(min_y, max_y):
        #print("\tget_row_range")
        min_x, max_x = poly.get_render_row_range(y, projections)
        #print("\t", min(dim[0], max(0, min_x)), max(0, min(dim[0],max_x)))
        for x in range(min(dim[0], max(0, min_x)), max(0, min(dim[0],max_x))):
            img_draw.point((x,y), "white")
t2 = time.time()

print(f"time to render: {(t2-t1) * 1000:0.4f} ms")

img.save("test.png")