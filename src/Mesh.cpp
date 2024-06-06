#include "Mesh.h"
#include "Polygon.h"
#include "Vec3.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include "debug.h"

void meshgroup::get_gl_vert_inds(vector<vec3> vertexes, vector<unsigned int>* mut_inds) {
    for (face fce : this->faces) {
        mut_inds->push_back(fce.vertex_indicies[0]);
        mut_inds->push_back(fce.vertex_indicies[1]);
        mut_inds->push_back(fce.vertex_indicies[2]);
    }
}

void meshgroup::get_gl_verts(vector<vec3> vertexes, vector<float>* mut_verts) {
    for (vec3 vert : *this->vertexes) {
        mut_verts->push_back(vert.x);
        mut_verts->push_back(vert.y);
        mut_verts->push_back(vert.z);
    }
}
 
vector<polygon> meshgroup::get_polygons(vector<vec3> vertexes) {
    
    vector<polygon> polygons;
    auto faces = this->faces;
    for (face fce : faces) {
        auto v_inds = fce.vertex_indicies;
        auto vt_inds = fce.vertex_tex_coord_indices;
        auto vn_inds = fce.normal_indicies;
        // std::cout << fce << std::endl;

        polygons.push_back(
            polygon(
                vertexes[v_inds[0]],
                vertexes[v_inds[1]],
                vertexes[v_inds[2]],
                vt_inds[0] != -1 ? (*this->uv_vertexes)[vt_inds[0]] : vec3(),
                vt_inds[1] != -1 ? (*this->uv_vertexes)[vt_inds[1]] : vec3(),
                vt_inds[2] != -1 ? (*this->uv_vertexes)[vt_inds[2]] : vec3(),
                vn_inds[0] != -1 ? (*this->vertex_normals)[vn_inds[0]] : vec3(),
                vn_inds[1] != -1 ? (*this->vertex_normals)[vn_inds[1]] : vec3(),
                vn_inds[2] != -1 ? (*this->vertex_normals)[vn_inds[2]] : vec3(),
                this
            ));
    }
    std::sort(polygons.begin(), polygons.end(), [](polygon a, polygon b){
        return std::min(a.A.z, std::min(a.B.z, a.C.z)) < std::min(b.A.z, std::min(b.B.z, b.C.z));
    });
    //print_vec(polygons);
    return polygons;
}

mesh* mesh::from_obj(string file_path) {
    // RETURNS HEAP ALLOCATED POINTER
    
    map<std::string, meshgroup>* groups = new map<std::string, meshgroup>;
    vector<string> tokens;
    string x, y, z, line, prefix;
    vector<tup<int, 3>> face_data, poly_buffer;
    map<std::string, material*> materials;
    string current_group = "default";
    vector<vec3>* vertexes = new vector<vec3>;
    vector<vec3>* uv_vertexes = new vector<vec3>;
    vector<vec3>* vertex_normals = new vector<vec3>;
    (*groups)[current_group] = meshgroup(vertexes, uv_vertexes, vertex_normals);
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
            vertexes->push_back(
                vec3(
                    std::stof(x),
                    std::stof(y),
                    std::stof(z)
                )
            );
        }
        else if (prefix == "vt") {
            x = tokens[1];
            y = tokens[2];
            z = tokens.size() == 4 ? tokens[3] : "0";// 4 cuz the prefix
            uv_vertexes->push_back(
                vec3(
                    std::stof(x),
                    std::stof(y),
                    std::stof(z)
                )
            );
        }
        else if (prefix == "vn") {
            x = tokens[1];
            y = tokens[2];
            z = tokens[3];
            vertex_normals->push_back(
                vec3(
                    std::stof(x),
                    std::stof(y),
                    std::stof(z)
                )
            );
        }
        else if (prefix == "f") {
            face_data.clear();
            face_data = mesh::parse_face(std::vector<std::string>(tokens.begin()+1, tokens.end()));
            size_t face_len = face_data.size();
            size_t last_ind = face_len-1;
            poly_buffer.clear();
            size_t i;
            for (i = 0; i < face_len; i++) {
                if (poly_buffer.size() == 2) {
                    poly_buffer.push_back(face_data[last_ind]);
                    // push back the correct faces
                    (*groups)[current_group].faces.push_back(face(
                        make_tup<int, 3>({
                            poly_buffer[0][0],
                            poly_buffer[1][0],
                            poly_buffer[2][0]
                        }),
                        make_tup<int, 3>({
                            poly_buffer[0][1],
                            poly_buffer[1][1],
                            poly_buffer[2][1]
                        }),
                        make_tup<int, 3>({
                            poly_buffer[0][2],
                            poly_buffer[1][2],
                            poly_buffer[2][2]
                        })
                    ));
                    poly_buffer.clear();
                    poly_buffer.push_back(face_data[i-1]);
                }
                poly_buffer.push_back(face_data[i]);
            }
            poly_buffer.push_back(face_data[last_ind]);
            (*groups)[current_group].faces.push_back(face(
                make_tup<int, 3>({
                    poly_buffer[0][0],
                    poly_buffer[1][0],
                    poly_buffer[2][0]
                }),
                make_tup<int, 3>({
                    poly_buffer[0][1],
                    poly_buffer[1][1],
                    poly_buffer[2][1]
                }),
                make_tup<int, 3>({
                    poly_buffer[0][2],
                    poly_buffer[1][2],
                    poly_buffer[2][2]
                })
            ));
        }
        else if (prefix == "g") {
            current_group = tokens[1];
            if (!groups->count(current_group))
                (*groups)[current_group] = meshgroup(vertexes, uv_vertexes, vertex_normals);
            (*groups)[current_group].material_data = materials.end()->second;
        }
        else if (prefix == "usemtl") {
            (*groups)[current_group].material_data = materials[tokens[1]];
        }
        else if (prefix == "mtllib") {
            string fpath = file_path.substr(0, file_path.find_last_of("\\/"));
            fpath += "/"+tokens[1];
            materials = mesh::get_materials(fpath);
            (*groups)["default"].material_data = materials.begin()->second;
        }
    }
    //std::cout << "thing -> " << (*groups)["default"].material_data->name << " is the first material\n";
    return new mesh(
        groups,
        materials,
        vertexes,
        uv_vertexes,
        vertex_normals
    );
}

vector<tup<int, 3>> mesh::parse_face(vector<string> tokens) {

    vector<tup<int, 3>> polygon;
    string vertex, text_coord, normal;
    if (tokens[0].find("//") != string::npos) {
        // Face with vertex normals
        for (string token : tokens) {
            vertex = token.substr(0, token.find("//"));
            normal = token.substr(token.find("//") + 1);
            
            polygon.push_back(make_tup<int,3>({
                std::stoi(vertex) - 1,
                -1,
                std::stoi(normal) - 1
            }));
        }
    }
    else if (tokens[0].find("/") != string::npos) {
        if (std::count(tokens[0].begin(), tokens[0].end(), '/') == 1){
            // Face with texture coords coord/coord
            for (string token : tokens) {
                vertex = token.substr(0, token.find("/"));
                text_coord = token.substr(token.find("/") + 1);
                
                polygon.push_back(make_tup<int,3>({
                    std::stoi(vertex) - 1,
                    std::stoi(text_coord) - 1,
                    -1
                }));
            }
        }
        else {
            // Face with txt and norms
            for (string token : tokens) {
                vertex = token.substr(0, token.find("/"));
                text_coord = token.substr(token.find("/") + 1, sub_str_ind(token, "/", 2));
                normal = token.substr(sub_str_ind(token, "/", 2) + 1);
                
                polygon.push_back(make_tup<int,3>({
                    std::stoi(vertex) - 1,
                    std::stoi(text_coord) - 1,
                    std::stoi(normal) - 1
                }));
            }
            
        }
    }
    else {
        // Face
        for (string token : tokens) {
            polygon.push_back(make_tup<int,3>({
                std::stoi(token) - 1,
                -1,
                -1
            }));
        }
    }
    return polygon;
}

map<std::string, material*> mesh::get_materials(string file_path) {
    // RETURNS A HEAP ALLOCATED POINTER
    string r, g, b, line, prefix, fpath;
    vector<string> tokens;
    std::ifstream file(file_path);
    map<std::string, material*> ret_mats;
    string current_mat;
    while (std::getline(file, line)) {
        tokens = split(trim(line));
        if (tokens.size() == 0) {
            continue;
        }
        prefix = tokens[0];
        if (prefix == "newmtl") {
            current_mat = tokens[1];
            ret_mats[current_mat] = new material();
            ret_mats[current_mat]->name = tokens[1];
        }
        switch (prefix[0])
        {
            case 'K':
                // material color and illumination statements
                switch (prefix[1])
                {
                    case 'a':
                        // ambient
                        r = tokens[1];
                        g = tokens[2];
                        b = tokens[3];
                        ret_mats[current_mat]->ambient = make_tup<float, 3>({
                            std::stof(r),
                            std::stof(g),
                            std::stof(b)
                        });
                        break;

                    case 'd':
                        // ambient
                        r = tokens[1];
                        g = tokens[2];
                        b = tokens[3];
                        ret_mats[current_mat]->diffuse = make_tup<float, 3>({
                            std::stof(r),
                            std::stof(g),
                            std::stof(b)
                        });
                        break;

                    case 's':
                        // ambient
                        r = tokens[1];
                        g = tokens[2];
                        b = tokens[3];
                        ret_mats[current_mat]->specular = make_tup<float, 3>({
                            std::stof(r),
                            std::stof(g),
                            std::stof(b)
                        });
                        break;

                    case 'e':
                        // ambient
                        r = tokens[1];
                        g = tokens[2];
                        b = tokens[3];
                        ret_mats[current_mat]->emissive_coeficient = make_tup<float, 3>({
                            std::stof(r),
                            std::stof(g),
                            std::stof(b)
                        });
                        break;
                    
                    default:
                        throw "Failed to parse.";
                        break;
                }
                break;
            
            case 'N':
                switch (prefix[1])
                {
                    case 'i':
                        ret_mats[current_mat]->optical_density = std::stof(tokens[1]);
                        break;

                    case 's':
                        ret_mats[current_mat]->specular_exponent = std::stof(tokens[1]);
                        break;
                    
                    default:
                        break;
                }
                break;

            case 'i':
                ret_mats[current_mat]->illumination_model = static_cast<illum_model>(std::stoi(tokens[1]));
                break;

            case 'm':
                switch (*(prefix.end()-2))
                {
                    case 'K':
                        switch (*(prefix.end()-1))
                        {
                            case 'a':
                                ret_mats[current_mat]->ambient_tex_file = tokens[1];
                                break;
 
                            case 'd':
                                ret_mats[current_mat]->diffuse_tex_file = tokens[1];
                                fpath = file_path.substr(0, file_path.find_last_of("\\/"));
                                fpath += "/"+tokens[1];
                                ret_mats[current_mat]->diffuse_texture = cv::imread(fpath);
                                break;
                            
                            default:
                                break;
                        }
                        break;

                    case 'N':
                        switch (prefix.back())
                        {
                            case 's':
                                ret_mats[current_mat]->specular_highlight_file = tokens[1];
                                break;
                            
                            default:
                                break;
                        }
                        break;
                    
                    default:
                        break;
                }
                break;

            case 'T':
                switch (prefix[1])
                {
                case 'r':
                    ret_mats[current_mat]->transparency = std::stof(tokens[1]);
                    break;
                
                default:
                    break;
                }
                break;
            
            default:
            break;
        }
    }

    return ret_mats;
}

