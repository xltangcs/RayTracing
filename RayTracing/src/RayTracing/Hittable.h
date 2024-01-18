#pragma once

#include "Ray.h"

class hit_record {
public:
    glm::vec3 p;
    glm::vec3 normal;
    float t;

    bool front_face;

    void set_face_normal(const Ray& r, const glm::vec3& outward_normal) {
        // Sets the hit record normal vector.
        // NOTE: the parameter `outward_normal` is assumed to have unit length.

        front_face = glm::dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }

};

class Hittable {
public:
    virtual ~Hittable() = default;

    virtual bool hit(const Ray& r, float ray_tmin, float ray_tmax, hit_record& rec) const = 0;
};