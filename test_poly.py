import time
from PIL import Image, ImageDraw
from renderer import Vec3, Camera, Screen, Polygon, Mesh, Object

# TODO: trace your steps carefully through the render pipeline and figure out what is breaking.

dim = (500, 500)

# Use PIL as render medium
img = Image.new("RGB", dim)
img_draw = ImageDraw.Draw(img)

 

cam = Camera(Vec3(0.0,0.0,0.0), *dim, 200)
screen = Screen(500, 500)
tp = Mesh(
    [
        Vec3(0,0,0),
        Vec3(0,1,0),
        Vec3(1,1,0)
    ],
    [
        (0,1,2)
    ]
)
teapot = Object(tp, Vec3(0.0,0.0,7.0), Vec3(0,3.1,0))
t1 = time.time()
teapot.render(cam, screen)
for x, y, color in cam.get_pixel():
    img_draw.point((x,y), color)
t2 = time.time()

print(f"time to render: {(t2-t1) * 1000:0.4f} ms")

img.save("test.png")