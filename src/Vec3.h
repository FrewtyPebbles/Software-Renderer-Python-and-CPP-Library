#pragma once
#include <iostream>
#include <math.h>
#include "Tup.h"

// cpp class names are camelcase, python class names are title case

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

    // ROTATION

    static inline tup<tup<vec3, 3>, 3> get_rotation_matrix(vec3 rot) {
        return make_tup<tup<vec3, 3>, 3>({
            make_tup<vec3, 3>({
                vec3(1.0f, 0.0f, 0.0f),
                vec3(0.0f, std::cosf(rot.x), -std::sinf(rot.x)),
                vec3(0.0f, std::sinf(rot.x), std::cosf(rot.x))
            }),
            make_tup<vec3, 3>({
                vec3(std::cosf(rot.y), 0.0f, std::sinf(rot.y)),
                vec3(0.0f, 1.0f, 0.0f),
                vec3(-std::sinf(rot.y), 0.0f, std::cosf(rot.y))
            }),
            make_tup<vec3, 3>({
                vec3(std::cosf(rot.z), -std::sinf(rot.z), 0.0f),
                vec3(std::sinf(rot.z), std::cosf(rot.z), 0.0f),
                vec3(0.0f, 0.0f, 1.0f)
            })
        });
    }

    inline vec3 rotate(vec3 rotation) {
        vec3 new_vec = *this;
        float x, y, z;

        
        for (tup<vec3, 3> rot : vec3::get_rotation_matrix(rotation).data) {
            x = new_vec.x;
            y = new_vec.y;
            z = new_vec.z;
            new_vec = vec3(
                std::fmaf(rot[0].x, x, std::fmaf(rot[0].y, y, rot[0].z*z)),
                std::fmaf(rot[1].x, x, std::fmaf(rot[1].y, y, rot[1].z*z)),
                std::fmaf(rot[2].x, x, std::fmaf(rot[2].y, y, rot[2].z*z))
            );
        }
            
        return new_vec;
    }

    // Operators

    inline vec3 operator+(vec3 const& other)
    {
        return vec3(x+other.x, y+other.y, z+other.z);
    }

    inline vec3 operator+(float const& other)
    {
        return vec3(x+other, y+other, z+other);
    }

    inline vec3 operator-(vec3 const& other)
    {
        return vec3(x-other.x, y-other.y, z-other.z);
    }

    inline vec3 operator-(float const& other)
    {
        return vec3(x-other, y-other, z-other);
    }
    
    inline vec3 operator*(vec3 const& other)
    {
        return vec3(x*other.x, y*other.y, z*other.z);
    }

    inline vec3 operator*(float const& other)
    {
        return vec3(x*other, y*other, z*other);
    }
    
    inline vec3 operator/(vec3 const& other)
    {
        return vec3(x/other.x, y/other.y, z/other.z);
    }

    inline vec3 operator/(float const& other)
    {
        return vec3(x/other, y/other, z/other);
    }

    inline float dot(vec3 const& other)
    {
        return fmaf(x, other.x, fmaf(y, other.y, z*other.z));
    }

    inline float cross(vec3 const& other)
    {
        return fmaf(y, other.z, -z*other.y) + fmaf(z, other.x, -x*other.z) + fmaf(z, other.y, -y*other.x);
    }
};

