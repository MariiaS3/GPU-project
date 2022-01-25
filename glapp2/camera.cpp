#include "camera.h"

Camera::Camera() {
    pos = {1,0,-3};
    up = {0,1,0};
    forward = {1,0,3};
}

vec3 Camera::x() const{
    return cross(forward, up);
}

mat4 Camera::matrix() const {
        mat4 M = identity();
        vec3 f = normal(forward);
        vec3 xx = x();
        normalize(xx);
        vec3 upp = cross(xx, forward);
        normalize(upp);
        M.m[0] = xx.x;    M.m[1] = upp.x;    M.m[2]  = -f.x;
        M.m[4] = xx.y;    M.m[5] = upp.y;    M.m[6]  = -f.y;
        M.m[8] = xx.z;    M.m[9] = upp.z;    M.m[10] = -f.z;

        M = mul(M,translationMat({-pos.x, -pos.y, -pos.z}));

        return M;
}
