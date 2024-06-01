# 3D Projections in Python

This is a 3d rendering library for python written in c++.  It is a re-write of a library I made recently written fully in cython because I needed parrallelism.

Here are some rendering tests using this library and PIL as a render medium:

First successful texture map:

![](https://github.com/FrewtyPebbles/python-c---rasterizer-library/blob/main/tests/texture_mapping_success.gif)

First render after porting over position/rotation:

![](https://github.com/FrewtyPebbles/python-c---rasterizer-library/blob/main/tests/rotating%20teapot.gif)

Latest success using barycentric coordinates for depth buffering:

![](https://github.com/FrewtyPebbles/python-c---rasterizer-library/blob/main/tests/barycentric_coordinate_success.png)

A cool looking failure I rendered while getting barycentric coordinates working:

![](https://github.com/FrewtyPebbles/python-c---rasterizer-library/blob/main/tests/barycentric_coordinate_fail.png)

Another cool looking failure I rendered while getting barycentric coordinates working:

![](https://github.com/FrewtyPebbles/python-c---rasterizer-library/blob/main/tests/depth_failure.png)

What improper depth buffering was rendering:

![](https://github.com/FrewtyPebbles/python-c---rasterizer-library/blob/main/tests/first_successful_render.png)

A failure I rendered while porting over the projection mapping:

![](https://github.com/FrewtyPebbles/python-c---rasterizer-library/blob/main/tests/failure_mesh_render_parrallel.png)
