import time
from PIL import Image, ImageDraw
from renderer import Vec3, Camera, Screen, Polygon, Mesh, Object, rasterize

# TODO: trace your steps carefully through the render pipeline and figure out what is breaking.

dim = (500, 500)

# Use PIL as render medium
img = Image.new("RGB", dim)
img_draw = ImageDraw.Draw(img)



cam = Camera(Vec3(0.0,0.0,0.0), *dim, 200)
screen = Screen(500, 500)
print("load mesh")
teapot = Object(Mesh.from_obj("meshes/teapot/teapot.obj"), Vec3(0.0,-25.0,120), Vec3(-1,0.5,-0.3))
print("Loaded mesh!")

#create gif
frames = []

t1 = time.time()
cppt1 = time.time()
teapot.rotation += 0.1
teapot.render(cam, screen)
cppt2 = time.time()
print(f"cpp time to render: {(cppt2-cppt1) * 1000:0.4f} ms")
rasterize(img_draw, cam)
t2 = time.time()
print(f"time to render: {(t2-t1) * 1000:0.4f} ms")
frames.append(img.copy())
img_draw.rectangle((0,0,dim[0]-1,dim[1]-1), "black")

for i in range(60):
    t1 = time.time()
    cppt1 = time.time()
    teapot.rotation += 0.1
    teapot.render(cam, screen)
    cppt2 = time.time()
    print(f"cpp time to render: {(cppt2-cppt1) * 1000:0.4f} ms")
    rasterize(img_draw, cam)
    t2 = time.time()
    print(f"time to render: {(t2-t1) * 1000:0.4f} ms")
    frames.append(img.copy())
    img_draw.rectangle((0,0,dim[0]-1,dim[1]-1), "black")

t1 = time.time()
cppt1 = time.time()
teapot.rotation += 0.1
teapot.render(cam, screen)
cppt2 = time.time()
print(f"cpp time to render: {(cppt2-cppt1) * 1000:0.4f} ms")
rasterize(img_draw, cam)
t2 = time.time()
print(f"time to render: {(t2-t1) * 1000:0.4f} ms")
frames.append(img.copy())

img.save("result.gif", save_all = True, append_images = frames, duration = 5, loop = 0)