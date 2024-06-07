#include "Object.h"
#include "Camera.h"
#include "Polygon.h"
#include "Mesh.h"
#include <chrono>
#include <thread>
#include "util.h"
#include "glad/gl.h"


object::object(mesh* mesh, vec3 position, vec3 rotation, vec3 scale)
: mesh_data(mesh), position(position), rotation(rotation), scale(scale) {
    this->vertex_shader = shader(default_vertex_shader, ShaderType::VERTEX);
    this->fragment_shader = shader(default_fragment_shader, ShaderType::VERTEX);
}

object::object(mesh* mesh, vec3 position, vec3 rotation, vec3 scale, shader vertex_shader)
: mesh_data(mesh), position(position), rotation(rotation), scale(scale), vertex_shader(vertex_shader) {
    this->fragment_shader = shader(default_fragment_shader, ShaderType::VERTEX);
}
object::object(mesh* mesh, vec3 position, vec3 rotation, vec3 scale, shader vertex_shader, shader fragment_shader)
: mesh_data(mesh), position(position), rotation(rotation), scale(scale), vertex_shader(vertex_shader), fragment_shader(fragment_shader) {
}

void object::link_shaders() {
    this->vertex_shader.compile();
    this->fragment_shader.compile();
    this->shader_program = glCreateProgram();
    glAttachShader(this->shader_program, this->vertex_shader.shader_handle);
    glAttachShader(this->shader_program, this->fragment_shader.shader_handle);
    glLinkProgram(this->shader_program);
    
    // Check for linking errors
    GLint success;
    GLchar infoLog[512];
    glGetProgramiv(this->shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(this->shader_program, 512, NULL, infoLog);
        throw std::format("ERROR::SHADER::PROGRAM::LINKING_FAILED\n{}\n", infoLog);
    }
    glDeleteShader(this->vertex_shader.shader_handle);
    glDeleteShader(this->fragment_shader.shader_handle);
}

void object::render(camera& camera, screen& screen) {
    if (!camera.use_software_renderer) {
        // opengl renderer
        vector<vec3> moved_verticies;
        vector<float> gl_verts;
        vector<unsigned int> gl_inds;
        for (auto [meshname, meshg] : *this->mesh_data->groups) {
            moved_verticies = this->get_translation(
                this->get_rotation(
                    *meshg.vertexes
                )
            );
            
            meshg.get_gl_verts(moved_verticies, &gl_verts);
            meshg.get_gl_vert_inds(moved_verticies, &gl_inds);
        }

        // this is where the render code should go
        // do a test in a separate project before trying to do the render code.



    } else {
        // software renderer
        vector<polygon> polygons;
        for (auto [meshname, meshg] : *this->mesh_data->groups) {
            //std::cout << "next_mesh = " << meshname << " :: " << meshg.vertexes->size() << std::endl;
            polygons = meshg.get_polygons(
                this->get_translation(
                    this->get_rotation(
                        *meshg.vertexes
                    )
                ) 
            );

            
            //std::cout << "MAT NAME " << meshg.material_data->name << std::endl;
            //print_vec(polygons);
            
            size_t p_len = polygons.size();
            for (size_t i = 0; i < p_len; i++) {
                screen.threadpool->QueueJob(std::bind(&polygon::render, polygons[i], &camera, &screen));
                //polygons[i].render(&camera, &screen);
            }
        }
        while (screen.threadpool->busy()){}
    }
}
 
vector<vec3> object::get_translation(vector<vec3> vertexes) {
    vector<vec3> ret_verts;
    vec3 pos = this->position;
    //std::cout << pos << "\n";
    for (vec3 vert : vertexes) {
        ret_verts.push_back(vert + pos);
    }
    //print_vec(ret_verts);
    return ret_verts;
}

vector<vec3> object::get_rotation(vector<vec3> vertexes) {
    vector<vec3> ret_verts;
    vec3 rot = this->rotation;
    for (vec3 vert : vertexes) {
        ret_verts.push_back(vert.rotate(rot));
    }
    return ret_verts;
}

template<typename T>
void print_vec(vector<T> in) {
    for (T thing : in)
        std::cout << thing << ", ";
    std::cout << "|||endvec\n";
}