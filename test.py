import time
from PIL import Image, ImageDraw
from vec3 import Vec3
from camera import Camera, Screen
from mesh import Polygon

dim = (500, 500)

# Use PIL as render medium
img = Image.new("RGB", dim)
img_draw = ImageDraw.Draw(img)

cam = Camera(Vec3(0.0,0.0,0.0), 500, 500, 200)

screen = Screen(500, 500)

polyg = Polygon(
    vec1:=Vec3(0.3, 0.5, 5.0),
    vec2:=Vec3(1.5, 1.4, 5.0),
    vec3:=Vec3(1.2, 2.0, 5.0)
)
projections = (
    vec1.project(cam, screen),
    vec2.project(cam, screen),
    vec3.project(cam, screen)
)
min_y, max_y = polyg.get_vertical_bounds(projections, cam)
print(min_y, max_y)
for y in range(min_y, max_y):
    min_x, min_y = polyg.get_render_row_range(y, projections)
    print(max(0, min_x), min(500, min_y))
    for x in range(min_x, min_y):
        img_draw.point((x,y), "white")

img.save("test.png")