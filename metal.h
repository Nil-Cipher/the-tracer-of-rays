#ifndef METALH
#define METALH
#include "hitable.h"
#include "material.h"
class metal : public material {
   private:
    vec3 _albedo;
    double _fuzz;
    vec3 reflect(const vec3 &v, const vec3 &n) const {
        return v - 2 * dot(v, n) * n;
    }

   public:
    metal(const vec3 &a, double f) : _albedo{a} { _fuzz = f < 1 ? f : 1; };
    virtual bool scatter(const ray &r_in, const hit_record &rec,
                         vec3 &attenuation, ray &scattered) const {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected + _fuzz * random_in_unit_sphere());
        attenuation = _albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }
};
#endif