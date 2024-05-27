#pragma once


// cpp class names are camelcase, python class names are title case

template<typename T, size_t N>
class tup;
class camera;
class screen;

class vec3 {
public:
    vec3();
    vec3(float x, float y, float z);
    float x;
    float y;
    float z;
    tup<float, 2> project(camera* camera, screen* screen);
};

