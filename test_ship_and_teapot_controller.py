import time
from PIL import Image, ImageDraw
from renderer import Vec3, Camera, Screen, Polygon, Mesh, Object, Window, event
import math


dim = (1280, 720)

camera = Camera(Vec3(0.0,0.0,0.0), *dim, 450, 5*math.pi/6)
screen = Screen(*dim)
window = Window("Sailing ship", camera, screen, *dim)

ship = Object(Mesh.from_obj("meshes\pirate_ship\pirate_ship.obj"),
    Vec3(0.0,-70.0,100), Vec3(0,0,0))
teapot = Object(Mesh.from_obj("meshes/teapot/teapot.obj"),
    Vec3(0.0,-70.0,200), Vec3(0,0,0))

render_list = [
    ship,
    teapot
]

# print("linking shaders")
# for ob in render_list:
#     ob.link_shaders()
# print("shaders linked")

i=0
while True:
    t1 = time.time()
    
    ship.rotation.z = math.cos(i/5)/3
    teapot.rotation.z = math.sin(i/5)/2
    teapot.rotation.x = math.cos(i/5)/2
    camera.render(render_list, screen)
    window.update()
    t2 = time.time()
    print(f"time to render: {(t2-t1) * 1000:0.4f} ms")
    i+=1

    if window.current_event == event.KEY_UP:
        ship.position += Vec3(
            -math.cos(ship.rotation.y)*15,
            math.sin(i/3)/2 * 5,
            math.sin(ship.rotation.y)*15
        )
    if window.current_event == event.KEY_DOWN:
        ship.position -= Vec3(
            -math.cos(ship.rotation.y)*15,
            math.sin(i/3)/2 * 5,
            math.sin(ship.rotation.y)*15
        )

    if window.current_event == event.KEY_RIGHT:
        ship.rotation.y -= 0.1

    if window.current_event == event.KEY_LEFT:
        ship.rotation.y += 0.1

    if window.current_event == event.QUIT:
        break