import time
from PIL import Image, ImageDraw
from renderer import Vec3, Camera, Screen, Mesh, Object, render_PIL
import math


dim = (500,500)

# Use PIL as render medium
img = Image.new("RGB", dim)
img_draw = ImageDraw.Draw(img)



cam = Camera(Vec3(0.0,0.0,0.0), *dim, 450, 5*math.pi/6)
screen = Screen(*dim)


print("load mesh")
ship = Object(Mesh.from_obj("meshes\pirate_ship\pirate_ship.obj"),
    Vec3(0.0,-70.0,100), Vec3(0,0,0))
print("Loaded mesh!")

print("load mesh")
teapot = Object(Mesh.from_obj("meshes/teapot/teapot.obj"),
    Vec3(0.0,-70.0,200), Vec3(0,0,0))
print("Loaded mesh!")

render_list = [
    ship,
    teapot
]

#create gif
frames = []

totaltime_start = time.time()

t1 = time.time()
cppt1 = time.time()
ship.rotation.y += 0.1
ship.position += Vec3(
    -math.cos(ship.rotation.y)*10,
    math.sin(0)/2 * 5,
    math.sin(ship.rotation.y)*10
)
ship.rotation.z = math.cos(0)/3
teapot.rotation.z = math.sin(0)/2
teapot.rotation.x = math.cos(0)/2
teapot.position.y += math.cos(0)/2 * 5
cam.render(render_list, screen)
cppt2 = time.time()
print(f"cpp time to render: {(cppt2-cppt1) * 1000:0.4f} ms")
render_PIL(img_draw, cam)
t2 = time.time()
print(f"time to render: {(t2-t1) * 1000:0.4f} ms")
frames.append(img.copy())
img_draw.rectangle((0,0,dim[0]-1,dim[1]-1), "black")

for i in range(60):
    t1 = time.time()
    ship.rotation.y += 0.1
    ship.position += Vec3(
        -math.cos(ship.rotation.y)*15,
        math.sin(i/3)/2 * 5,
        math.sin(ship.rotation.y)*15
    )
    ship.rotation.z = math.cos(i/5)/3
    teapot.rotation.z = math.sin(i/5)/2
    teapot.rotation.x = math.cos(i/5)/2
    teapot.position.y += math.cos(i/3)/2 * 5
    cam.render(render_list, screen)
    render_PIL(img_draw, cam)
    t2 = time.time()
    print(f"time to render: {(t2-t1) * 1000:0.4f} ms")
    frames.append(img.copy())
    img_draw.rectangle((0,0,dim[0]-1,dim[1]-1), "black")

t1 = time.time()
cppt1 = time.time()
ship.rotation.y += 0.1
# soh cah toa
ship.position += Vec3(
    -math.cos(ship.rotation.y)*10,
    math.sin(60/3)/2*5,
    math.sin(ship.rotation.y)*10
)
ship.rotation.z = math.cos(60/5)/3
teapot.rotation.z = math.sin(60/5)/2
teapot.rotation.x = math.cos(60/5)/2
teapot.position.y += math.cos(60/3)/2 * 5
cam.render(render_list, screen)
cppt2 = time.time()
print(f"cpp time to render: {(cppt2-cppt1) * 1000:0.4f} ms")
render_PIL(img_draw, cam)
t2 = time.time()
print(f"time to render: {(t2-t1) * 1000:0.4f} ms")
frames.append(img.copy())

totaltime_end = time.time()

print(f"Total time to render: {(totaltime_end-totaltime_start) * 1000:0.4f}")

img.save("result.gif", save_all = True, append_images = frames, duration = 5, loop = 0)