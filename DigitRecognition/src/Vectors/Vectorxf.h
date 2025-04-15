#ifndef VECTORXF
#define VECTORXF

#include <vector>

struct Vector2f {
    float x;
    float y;

    Vector2f(float x, float y);
};

struct Vector2i {
    int x;
    int y;

    Vector2i(int x, int y);
};

struct Vector3f {
    float x;
    float y;
    float z;

    Vector3f(float x, float y, float z);
};

#endif