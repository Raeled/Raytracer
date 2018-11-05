#pragma once

#include <cmath>

struct vec2 {
    float x, y;
};

struct vec3 {
    float x, y, z;
    
    vec3() : x(0), y(0), z(0) {}
    vec3(float x, float y, float z) : x(x), y(y), z(z) {};
    
    float Length() const { return sqrt(x * x + y * y + z * z); }
    
    static float DotProduct(const vec3 a, const vec3 b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }
    
    static vec3 CrossProduct(const vec3 a, const vec3 b) {
        return vec3(a.y * b.z - a.z * b.y,
                    a.z * b.x - a.x * b.z,
                    a.x * b.y - a.y * b.x);
    }
};

static vec3 operator +(const vec3 a, const float b) { return vec3(a.x + b, a.y + b, a.z + b); }
static vec3 operator *(const vec3 a, const float b) { return vec3(a.x * b, a.y * b, a.z * b); }
static vec3 operator /(const vec3 a, const float b) { return vec3(a.x / b, a.y / b, a.z / b); }
static vec3 operator +(const vec3 a, const vec3 b) { return vec3(a.x + b.x, a.y + b.y, a.z + b.z); }
static vec3 operator -(const vec3 a, const vec3 b) { return vec3(a.x - b.x, a.y - b.y, a.z - b.z); }