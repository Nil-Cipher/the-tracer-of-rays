#ifndef DIELECTRICH
#define DIELECTRICH
#include "hitable.h"
#include "material.h"
class dielectric : public material {
   private:
    double ref_idx;
    bool refract(const vec3 &v, const vec3 &n, double ni_over_nt,
                 vec3 &refracted) const {
        vec3 uv = unit_vector(v);
        double dt = dot(uv, n);
        // is refraction possible?
        double discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);
        if (discriminant > 0) {
            refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
            return true;
        } else {
            return false;
        }
    }
    double schlick(double cosine, double ref_idx) const {
        double r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }
    vec3 reflect(const vec3 &v, const vec3 &n) const {
        return v - 2 * dot(v, n) * n;
    }

   public:
    dielectric(double ri) : ref_idx{ri} {};
    virtual bool scatter(const ray &r_in, const hit_record &rec,
                         vec3 &attenuation, ray &scattered) const {
        vec3 outward_normal;
        vec3 reflected = reflect(r_in.direction(), rec.normal);
        double ni_over_nt;
        attenuation = vec3(1.0, 1.0, 1.0);
        vec3 refracted;
        double reflect_prob;
        double cosine;
        // dielectric to air
        if (dot(r_in.direction(), rec.normal) > 0) {
            outward_normal = -rec.normal;
            ni_over_nt = ref_idx;
            cosine = ref_idx * dot(r_in.direction(), rec.normal) /
                     r_in.direction().length();
            // air to dielectric
        } else {
            outward_normal = rec.normal;
            ni_over_nt = 1 / ref_idx;
            cosine =
                -dot(r_in.direction(), rec.normal) / r_in.direction().length();
        }
        if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted)) {
            reflect_prob = schlick(cosine, ref_idx);
        } else {
            reflect_prob = 1.0;
        }
        if (drand48() < reflect_prob) {
            scattered = ray(rec.p, reflected);
        } else {
            scattered = ray(rec.p, refracted);
        }
        return true;
    }
};
#endif