#pragma once
#include "Vec3.h"
#include <vector>
#include "Shader.h"
using std::vector;
class mesh;
class camera;
class screen;

// Shaders
const string default_vertex_shader = "#version 330 core\n"
    "layout (location = 0) in vec3 position;\n"
    "void main()\n"
    "{\n"
    "gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
    "}";
const string default_fragment_shader = "#version 330 core\n"
    "out vec4 color;\n"
    "void main()\n"
    "{\n"
    "color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n";

class object {
public:
    object(){};
    // create 3 different constructors for the default arguments in .pxd file
    object(mesh* mesh, vec3 position, vec3 rotation, vec3 scale);
    object(mesh* mesh, vec3 position, vec3 rotation, vec3 scale, shader vertex_shader);
    object(mesh* mesh, vec3 position, vec3 rotation, vec3 scale, shader vertex_shader, shader fragment_shader);
    
    mesh* mesh_data;
    vec3 position;
    vec3 rotation;
    vec3 scale;
    
    shader vertex_shader, fragment_shader;

    void render(camera& camera, screen& screen);
    vector<vec3> get_translation(vector<vec3> vertexes);
    vector<vec3> get_rotation(vector<vec3> vertexes);
    void link_shaders();
private:
    GLuint shader_program;
};

typedef object object3d;