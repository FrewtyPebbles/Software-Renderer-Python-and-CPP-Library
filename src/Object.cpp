#include "Object.h"
#include "Camera.h"
#include "Polygon.h"
#include "Mesh.h"
#include <chrono>
#include <thread>
#include "util.h"



object::object(mesh* mesh, vec3 position, vec3 rotation, vec3 scale) : mesh_data(mesh), position(position), rotation(rotation), scale(scale) {}

void object::render(camera& camera, screen& screen) {
    //std::cout << "Threads available: " << std::thread::hardware_concurrency() << std::endl;
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