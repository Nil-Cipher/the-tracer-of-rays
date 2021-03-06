#ifndef CAMERAH
#define CAMERAH

#include "ray.h"

class camera{
    public:
        vec3 origin;
        vec3 lower_left_corner;
        vec3 horizontal;
        vec3 vertical;

        camera(){
            lower_left_corner = vec3{-2,-1,-1};
            horizontal = vec3{4,0,0};
            vertical = vec3{0,2,0};
            origin = vec3{0,0,0};
        }
        ray get_ray(double u, double v){
            return ray{origin, lower_left_corner + u * horizontal + v * vertical};
        }
};
#endif