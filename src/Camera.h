#pragma once
#include <vector>
#include "Tup.h"
#include <unordered_set>
#include "threadpool.h"

using std::vector;
class vec3;

struct pixel {
    pixel(): x(0), y(0), color(make_tup<uint8_t, 3>({0,0,0})){}
    pixel(int x, int y, tup3ui8 color)
        : x(x), y(y), color(color) {}
    pixel(const pixel& o) : x(o.x), y(o.y), color(o.color) {}
    int x;
    int y;
    tup3ui8 color;
    inline friend std::ostream& operator<<(std::ostream& os, const pixel& self) {
        os << "pixel<" << self.x << ", " << self.y << ", " << self.color << ">";
        return os;
    }
};

inline bool operator== (pixel const& lhs, pixel const& rhs)
{
    return lhs.x == rhs.x && lhs.y == rhs.y; 
}

struct pixelHash {
    pixelHash()=default;
    size_t operator()(const pixel &pxl) const {
        return pxl.x + pxl.y + pxl.x > pxl.y;
    }
};

class camera {
public:
    camera();
    camera(vec3* position, int view_width, int view_height, int view_distance);
    vec3* position;
    int view_width, view_height, view_distance;
    vector<vector<float>> depth_buffer;
    vector<vector<float>> cleared_depth_buffer;
    vector<pixel> frame_buffer;
};

class screen {
public:
    screen();
    screen(int width, int height);
    ~screen();
    int width, height;
    ThreadPool* threadpool = nullptr;
};