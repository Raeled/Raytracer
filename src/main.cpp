#include <iostream>
#include <vector>
#include <Math.hpp>
#include <ObjFile.hpp>

#include <stb_image_write.h>


float toRad(float degree) {
    return degree * (M_PI / 180.0f);
}

bool rayTriangleIntersects(const vec3 line[2], const vec3 ta, const vec3 tb, const vec3 tc, float& t) {
    
    // Implementation of https://en.wikipedia.org/wiki/M%C3%B6ller%E2%80%93Trumbore_intersection_algorithm
    vec3 triangle[] = {ta, tb, tc};
    
    const float EPSILON = 0.0000001;
    
    auto edge1 = triangle[1] - triangle[0];
    auto edge2 = triangle[2] - triangle[0];
    auto dir = line[1] - line[0];
    
    auto h = vec3::CrossProduct(dir, edge2);
    auto a = vec3::DotProduct(edge1, h);
    
    if (fabs(a) < EPSILON) return false;
    
    auto f = 1.0f / a;
    auto s = line[0] - triangle[0];
    auto u = f * vec3::DotProduct(s, h);
    
    if (u < 0.0 || u > 1.0) return false;
    
    auto q = vec3::CrossProduct(s, edge1);
    auto v = f * vec3::DotProduct(dir, q);
    
    if (v < 0.0 || u + v > 1.0) return false;
    
    t = f * vec3::DotProduct(edge2, q);
    if (t > EPSILON) return true;
    return false;
}

struct Bitmap {
    int width;
    int height;
    std::vector<unsigned char> data;

    Bitmap(int width, int height)
    : width(width)
    , height(height)
    , data(width * height * 4)
    { }

    int offset(int x, int y) const {
        return (width * y + x) * 4;
    }

    void witeToPng(const char* filename) const {
        stbi_write_png(filename, width, height, 4, data.data(), 0);
    }
};

int main() {
    float fov = 360.0f;
    
    int scale = 40;
    int resX = 40 * scale;
    int resY = 30 * scale;
    
    float aspect = (float)resY / (float)resX;
    
    float fovX = fov;
    float fovY = fov * aspect;
    float halfFovX = fovX / 2.0f;
    float halfFovY = fovY / 2.0f;
    float stepX = fovX / (resX - 1.0f);
    float stepY = fovY / (resY - 1.0f);
    
    

    Bitmap output(resX, resY);

    std::vector<std::vector<vec3>> triangles;

    auto scene = loadObjFile("monkey.obj");
    vec3 offset(0,0,0);
    
    for (int i=0; i<scene.indexes.size(); i+=3) {
        triangles.emplace_back();
        triangles.back().push_back(scene.vertexes[scene.indexes[i+0]].position + offset);
        triangles.back().push_back(scene.vertexes[scene.indexes[i+1]].position + offset);
        triangles.back().push_back(scene.vertexes[scene.indexes[i+2]].position + offset);
    }

    for (int i=0; i<resX; i++) {
        
        // for 2d canvas
        float someX = i - (resX/2.0f);
        
        // for spherical pano
        //float rayDegreeX = toRad((i * stepX) - halfFovX);
        
        for (int j=0; j<resY; j++) {

            // for 2d canvas
            float someY = -j + (resY/2.0f);
            vec3 rayVector(someX, someY, -14.0f * scale);
            

            // for spherical pano
            //float rayDegreeY = toRad((j * stepY) - halfFovY);
            //vec3 rayVector(cos(rayDegreeY) * sin(rayDegreeX), 
            //               -sin(rayDegreeY), 
            //               cos(rayDegreeY) * -cos(rayDegreeX));
            
            vec3 origin(-1,0,2);
            vec3 ray[] = {origin, origin + rayVector};
            
            float closest = 100000000.0f;
            std::vector<vec3>* current = nullptr;
            
            for (auto& triangle : triangles) {
                float t;
                if (rayTriangleIntersects(ray, triangle[0], triangle[1], triangle[2], t)) {
                    if (t < closest) {
                        closest = t;
                        current = &triangle;
                    }
                }
            }
            
            if (current != nullptr) {
                auto& triangle = *current;
                
                auto direction = ray[1] - ray[0];
                auto pos = ray[0] + direction * closest;
                
                vec3 newRay[] = {pos, vec3(6, 4.2, 14)};
                bool lightBlocked = false;
                for (auto& triangle2 : triangles) {
                    float t;
                    if (rayTriangleIntersects(newRay, triangle2[0], triangle2[1], triangle2[2], t)) {
                        if (t < 1.0f && t > 0.0001f) {
                            lightBlocked = true;
                            break;
                        }
                    }
                }
                
                auto normal = vec3::CrossProduct(triangle[1] - triangle[0], triangle[2] - triangle[0]);
                normal = normal / normal.Length();
                
                auto normalColor = (normal * 0.5f) + 0.5f;
                
                if (lightBlocked) normalColor = normalColor * 0.5f;
                
                output.data[output.offset(i, j) + 0] = normalColor.x * 255;
                output.data[output.offset(i, j) + 1] = normalColor.y * 255;
                output.data[output.offset(i, j) + 2] = normalColor.z * 255;
                output.data[output.offset(i, j) + 3] = 255;
            }
        }
    }

    output.witeToPng("output.png");
}