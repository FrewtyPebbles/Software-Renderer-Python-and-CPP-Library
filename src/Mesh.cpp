#include "Mesh.h"
#include "Polygon.h"
#include "Tup.h"
#include "Vec3.h"
#include <algorithm>
#include <fstream>

mesh::mesh(vector<vec3> vertexes, vector<tup<int, 3>> polygon_inds) : vertexes(vertexes), polygon_inds(polygon_inds), polygons(get_polygons(vertexes)) {
}

vector<polygon> mesh::get_polygons(vector<vec3> vertexes) {
    
    vector<polygon> polygons;
    vector<vec3> poly_buffer;
    
    for (tup3i v_inds : this->polygon_inds) {
        poly_buffer.clear();
        size_t inds_len = sizeof(v_inds.data)/sizeof(int);
        size_t i;
        for (i = 0; i < inds_len; i++) {
            if (poly_buffer.size() == 2) {
                poly_buffer.push_back(vertexes[v_inds.data[i]]);
                polygons.push_back(polygon(poly_buffer[0], poly_buffer[1], poly_buffer[2]));
                poly_buffer.clear();
                poly_buffer.push_back(vertexes[v_inds.data[i]]);
            } else {
                poly_buffer.push_back(vertexes[v_inds.data[i]]);
            }
        }
        if (poly_buffer.size() == 2) {
            poly_buffer.push_back(vertexes[0]);
            polygons.push_back(polygon(polygon(poly_buffer[0], poly_buffer[1], poly_buffer[2])));
        }
    }
    return polygons;
}

mesh mesh::from_obj(string file_path) {
    vector<vec3> verticies;
    vector<tup<int, 3>> polygons;
    vector<string> tokens;
    string _, x, y, z, line, prefix;
    

    std::ifstream file(file_path);
    while (std::getline(file, line)) {
        tokens = split(trim(line));
        if (tokens.size() == 0) {
            continue;
        }
        prefix = tokens[0];

        if (prefix == "v") {
            x = tokens[1];
            y = tokens[2];
            z = tokens[3];
            verticies.push_back(
                vec3(
                    std::stof(x),
                    std::stof(y),
                    std::stof(z)
                )
            );
        }
        else if (prefix == "f") {
            vector<int> face;
            vector<int> poly_buffer;
            face = mesh::parse_face(std::vector<std::string>(tokens.begin()+1, tokens.end()));
            size_t face_len = face.size();
            size_t i;
            for (i = 0; i < face_len; i++) {
                if (poly_buffer.size() == 2) {
                    poly_buffer.push_back(poly_buffer[0]);
                    polygons.push_back(make_tup<int, 3>({poly_buffer[0], poly_buffer[1], poly_buffer[2]}));
                    poly_buffer.clear();
                    poly_buffer.push_back(face[i-1]);
                }
                poly_buffer.push_back(face[i]);
            }
            poly_buffer.push_back(poly_buffer[0]);
            polygons.push_back(make_tup<int, 3>({poly_buffer[0], poly_buffer[1], poly_buffer[2]}));
        }
        else if (prefix == "vt") {

        }
        else if (prefix == "g") {

        }
        else if (prefix == "usemtl") {

        }
    }
    return mesh(verticies, polygons);
}

vector<int> mesh::parse_face(vector<string> tokens) {
    vector<int> polygon;
    string vertex, text_coord, normal;
    if (tokens[0].find("//") != string::npos) {
        // Face with vertex normals
        for (string token : tokens) {
            vertex = token.substr(0, token.find("//"));
            normal = token.substr(1, token.find("//"));
            polygon.push_back(std::stoi(vertex) - 1);
        }
    }
    else if (tokens[0].find("/") != string::npos) {
        if (std::count(tokens[0].begin(), tokens[0].end(), '/') == 1){
            // Face with texture coords
            for (string token : tokens) {
                vertex = token.substr(0, token.find("/"));
                text_coord = token.substr(1, token.find("/"));
                polygon.push_back(std::stoi(vertex) - 1);
            }
        }
        else {
            // Face with txt and norms
            for (string token : tokens) {
                vertex = token.substr(0, token.find("/"));
                text_coord = token.substr(1, token.find("/"));
                normal = token.substr(2, token.find("/"));
                polygon.push_back(std::stoi(vertex) - 1);
            }
            
        }
    }
    else {
        // Face
        for (string token : tokens) {
            polygon.push_back(std::stoi(token) - 1);
        }
    }
        
    return polygon;
}

template<typename T>
void print_vec(vector<T> in) {
    for (T thing : in)
        std::cout << thing << ", ";
    std::cout << "|||endvec\n";
}