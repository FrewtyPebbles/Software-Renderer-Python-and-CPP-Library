#pragma once
#include "Vec3.h"
#include <vector>
using std::vector;
class mesh;
class camera;
class screen;

class object {
public:
    object() {};
    object(mesh& mesh, vec3 position, vec3 rotation, vec3 scale);
    
    vec3 position;
    vec3 rotation;
    vec3 scale;
    mesh* mesh_data;
    void render(camera& camera, screen& screen);
    vector<vec3> get_translation(vector<vec3> vertexes);
    vector<vec3> get_rotation(vector<vec3> vertexes);
};

typedef object object3d;