#pragma once

#include <string>
#include <vector>
#include <Math.hpp>

struct Mesh {
    struct Vertex {
        vec3 position;
        vec3 normal;
        vec2 uv;
    };

    std::vector<Vertex> vertexes;
    std::vector<unsigned int> indexes;
};

Mesh loadObjFile(const std::string& filename);
