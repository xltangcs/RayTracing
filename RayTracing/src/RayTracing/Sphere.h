#pragma once

#include "Hittable.h"

class Sphere : public Hittable {
public:
    Sphere(const glm::vec3& _center, float _radius) : center(_center), radius(_radius) {}

    bool hit(const Ray& r, float Ray_tmin, float Ray_tmax, hit_record& rec) const override {
		glm::vec3 oc = center - r.origin();
		auto a = glm::dot(r.direction(), r.direction());
		auto b = -2.0f * dot(r.direction(), oc);
		auto c = glm::dot(oc, oc) - radius * radius;
		auto discriminant = b * b - 4.0f * a * c;
		if (discriminant < 0.0f)
			return false;
		float t0 = -b - sqrt(discriminant) / (2.0f * a);
		float t1 = -b + sqrt(discriminant) / (2.0f * a);

		rec.t = t0;
		if (t0 <= Ray_tmin || Ray_tmax <= t0) {
			rec.t = t1;
			if (t1 <= Ray_tmin || Ray_tmax <= t1)
				return false;
		}

		rec.p = r.at(rec.t);
		glm::vec3 outward_normal = (rec.p - center) / radius;
		rec.normal = outward_normal;

		//rec.set_face_normal(r, outward_normal);

		return true;
    }

private:
    glm::vec3 center;
    float radius;
};