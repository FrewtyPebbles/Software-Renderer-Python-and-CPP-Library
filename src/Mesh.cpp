#include "Mesh.h"
#include "Polygon.h"
#include "Vec3.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include "debug.h"


 
vector<polygon> mesh::get_polygons(vector<vec3> vertexes) {
    
    vector<polygon> polygons;
    auto faces = this->faces;
    for (face fce : faces) {
        auto v_inds = fce.vertex_indicies;
        auto vt_inds = fce.vertex_tex_coord_indices;
        auto vn_inds = fce.normal_indicies;

        polygons.push_back(
            polygon(
                vertexes[v_inds[0]],
                vertexes[v_inds[1]],
                vertexes[v_inds[2]],
                vt_inds[0] != -1 ? this->uv_vertexes[vt_inds[0]] : vec3(),
                vt_inds[1] != -1 ? this->uv_vertexes[vt_inds[1]] : vec3(),
                vt_inds[2] != -1 ? this->uv_vertexes[vt_inds[2]] : vec3(),
                vn_inds[0] != -1 ? this->vertex_normals[vn_inds[0]] : vec3(),
                vn_inds[1] != -1 ? this->vertex_normals[vn_inds[1]] : vec3(),
                vn_inds[2] != -1 ? this->vertex_normals[vn_inds[2]] : vec3(),
                this
            ));
    }
    //print_vec(polygons);
    return polygons;
}

mesh mesh::from_obj(string file_path) {
    vector<vec3> verticies;
    vector<vec3> uv_vertexes;
    vector<vec3> vertex_normals;
    vector<face> faces;
    vector<string> tokens;
    string x, y, z, line, prefix;
    vector<tup<int, 3>> face_data, poly_buffer;
    vector<material> materials;

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
        else if (prefix == "vt") {
            x = tokens[1];
            y = tokens[2];
            z = tokens[3];
            uv_vertexes.push_back(
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
            vertex_normals.push_back(
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
                    faces.push_back(face(
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
            faces.push_back(face(
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

        }
        else if (prefix == "usemtl") {

        }
        else if (prefix == "mtllib") {
            string fpath = file_path.substr(0, file_path.find_last_of("\\/"));
            fpath += "/"+tokens[1];
            std::cout << fpath << "\n";
            materials = mesh::get_material(fpath);
        }
    }
    mesh ret = mesh(
        verticies,
        faces,
        uv_vertexes,
        vertex_normals,
        materials
    );
    return ret;
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

vector<material> mesh::get_material(string file_path) {
    string r, g, b, line, prefix, fpath;
    vector<string> tokens;
    std::ifstream file(file_path);
    vector<material> ret_mats;
    while (std::getline(file, line)) {
        tokens = split(trim(line));
        if (tokens.size() == 0) {
            continue;
        }
        prefix = tokens[0];
        if (prefix == "newmtl") {
            ret_mats.push_back(material());
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
                        ret_mats.back().ambient = make_tup<float, 3>({
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
                        ret_mats.back().diffuse = make_tup<float, 3>({
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
                        ret_mats.back().specular = make_tup<float, 3>({
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
                        ret_mats.back().emissive_coeficient = make_tup<float, 3>({
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
                        ret_mats.back().optical_density = std::stof(tokens[1]);
                        break;

                    case 's':
                        ret_mats.back().specular_exponent = std::stof(tokens[1]);
                        break;
                    
                    default:
                        break;
                }
                break;

            case 'i':
                ret_mats.back().illumination_model = static_cast<illum_model>(std::stoi(tokens[1]));
                break;

            case 'm':
                switch (*(prefix.end()-2))
                {
                    case 'K':
                        switch (*(prefix.end()-1))
                        {
                            case 'a':
                                ret_mats.back().ambient_tex_file = tokens[1];
                                break;
 
                            case 'd':
                                ret_mats.back().diffuse_tex_file = tokens[1];
                                fpath = file_path.substr(0, file_path.find_last_of("\\/"));
                                fpath += "/"+tokens[1];
                                std::cout << fpath << "\n";
                                ret_mats.back().diffuse_texture = cv::imread(fpath);
                                break;
                            
                            default:
                                break;
                        }
                        break;

                    case 'N':
                        switch (prefix.back())
                        {
                            case 's':
                                ret_mats.back().specular_highlight_file = tokens[1];
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
                    ret_mats.back().transparency = std::stof(tokens[1]);
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

