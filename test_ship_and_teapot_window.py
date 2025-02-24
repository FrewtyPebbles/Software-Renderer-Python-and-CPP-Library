import time
from PIL import Image, ImageDraw
from renderer.vec3 import Vec3
from renderer.camera import Camera, Screen
from renderer.polygon import Polygon
from renderer.mesh import Mesh
from renderer.object import Object
from renderer.rasterize import render
from renderer.window import Window, event
import math


dim = (1280, 720)

print("creating camera")
camera = Camera(Vec3(0.0,0.0,0.0), *dim, 450, 5*math.pi/6)
print("created camera")
screen = Screen(*dim)
print("creating window")
window = Window("Sailing ship", camera, screen, *dim)
print("created window")

ship = Object(Mesh.from_obj("meshes\pirate_ship\pirate_ship.obj"),
    Vec3(0.0,-70.0,100), Vec3(0,0,0))
teapot = Object(Mesh.from_obj("meshes/teapot/teapot.obj"),
    Vec3(0.0,-70.0,200), Vec3(0,0,0))

render_list = [
    ship,
    teapot
]
print("Enter render loop:")
i=0
while True:
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
    camera.render(render_list, screen)
    window.update()
    t2 = time.time()
    print(f"time to render: {(t2-t1) * 1000:0.4f} ms")
    i+=1
    
    if window.current_event == event.WINDOW_CLOSE:
        break
    