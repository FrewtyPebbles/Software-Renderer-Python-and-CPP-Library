#include "Object.h"
#include "Camera.h"
#include "Polygon.h"
#include <thread>
#include "Mesh.h"

object::object(mesh& mesh, vec3 position, vec3 rotation, vec3 scale) : mesh_data(&mesh), position(position), rotation(rotation), scale(scale) {}

void object::render(camera& camera, screen& screen) {
    std::cout << this->mesh_data->vertexes.size() << " M_VERT\n";
    vector<polygon> polygons = this->mesh_data->get_polygons(
        this->get_translation(
            this->get_rotation(
                this->mesh_data->vertexes
            )
        )
    );
    vector<std::thread*> threads;
    size_t p_len = polygons.size();
    for (size_t i = 0; i < p_len; i++) {
        threads.push_back(new std::thread(&polygon::render, &polygons[i], camera, screen));
    }
    for (std::thread* thread : threads) {
        thread->join();
        delete thread;
    }
}

vector<vec3> object::get_translation(vector<vec3> vertexes) {
    vector<vec3> ret_verts;
    vec3 pos = this->position;
    for (vec3 vert : vertexes) {
        ret_verts.push_back(vert + pos);
    }
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