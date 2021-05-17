#ifndef LAMBERTIANH
#define LAMBERTIANH
#include "hitable.h"
#include "material.h"

class lambertian : public material {
   private:
    vec3 _albedo;

   public:
    lambertian(const vec3 &a) : _albedo{a} {};
    virtual bool scatter(const ray &r_in, const hit_record &rec,
                         vec3 &attenuation, ray &scattered) const {
        vec3 s = rec.p + rec.normal + random_in_unit_sphere();
        scattered = ray(rec.p, s - rec.p);
        attenuation = _albedo;
        return true;
    }
};
#endif