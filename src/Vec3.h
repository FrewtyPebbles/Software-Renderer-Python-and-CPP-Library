#pragma once
#include <iostream>

// cpp class names are camelcase, python class names are title case

template<typename T, size_t N>
class tup;
class camera;
class screen;

class vec3 {
public:
    vec3();
    vec3(float x, float y, float z);
    vec3(const vec3& rhs) : x(rhs.x),y(rhs.y),z(rhs.z) {}
    float x;
    float y;
    float z;
    tup<float, 2> project(camera& camera, screen& screen);

    friend inline std::ostream& operator<<(std::ostream& os, const vec3& self){
        os << "vec3<" << self.x << ", " << self.y << ", " << self.z << '>';
        return os;
    }
};

