#ifndef MATERIALH
#define MATERIALH
#include "hitable.h"

class material {
   public:
    virtual bool scatter(const ray &r_in, const hit_record &rec,
                         vec3 &attenuation, ray &scattered) const = 0;
    vec3 random_in_unit_sphere() const {
        vec3 p;
        do {
            p = 2 * vec3{drand48(), drand48(), drand48()} - vec3{1, 1, 1};
        } while (p.squared_length() >= 1);
        return p;
    }
};
#endif