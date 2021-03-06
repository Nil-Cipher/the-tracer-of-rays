#include <math.h>
#include <cstdlib>
#include <iostream>
#include "camera.h"
#include "dielectric.h"
#include "float.h"
#include "hitable.h"
#include "hitable_list.h"
#include "lambertian.h"
#include "material.h"
#include "metal.h"
#include "sphere.h"
// returns the color of a ray
vec3 color(const ray& r, hitable* world, int depth) {
    hit_record rec;
    if (world->hit(r, 0.001, DBL_MAX, rec)) {
        material* m = rec.mat_ptr;
        ray s;
        vec3 attenuation;
        if (depth < 50 && m->scatter(r, rec, attenuation, s)) {
            return attenuation * color(s, world, depth + 1);
        } else {
            return vec3{0, 0, 0};
        }
    } else {
        vec3 unit_direction = unit_vector(r.direction());
        double t = 0.5 * (unit_direction.y() + 1);
        return (1.0 - t) * vec3{1, 1, 1} + t * vec3{0.5, 0.7, 1.0};
    }
}
int main() {
    int nx = 500;
    int ny = 250;
    int ns = 100;
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";
    hitable* list[5];
    list[0] =
        new sphere{vec3{0, 0, -1}, 0.5, new lambertian{vec3{0.1, 0.2, 0.5}}};
    list[1] =
        new sphere{vec3{0, -100.5, -1}, 100, new lambertian{vec3{0.8, 0.8, 0}}};
    list[2] =
        new sphere{vec3{1, 0, -1}, 0.5, new metal{vec3{0.8, 0.6, 0.2}, 0.1}};
    list[3] = new sphere{vec3{-1, 0, -1}, 0.5, new dielectric{1.5}};
    // list[4] = new sphere{vec3{-1, 0, -1}, -0.45, new dielectric{1.5}};
    hitable* world = new hitable_list{list, 4};
    camera cam;
    for (int j = ny - 1; j >= 0; --j) {
        for (int i = 0; i < nx; ++i) {
            vec3 col{0, 0, 0};
            for (int s = 0; s < ns; ++s) {
                double u = double(i + drand48()) / double(nx);
                double v = double(j + drand48()) / double(ny);
                ray r = cam.get_ray(u, v);
                col += color(r, world, 0);
            }
            col /= double(ns);
            col = vec3{sqrt(col[0]), sqrt(col[1]), sqrt(col[2])};
            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);

            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}