#pragma once

#include <memory>

#include "glm/glm.hpp"

#include "Toffee/Random.h"

#include "Ray.h"
#include "Texture.h"
#include "HitPayload.h"

class Material {
public:
    virtual glm::vec3 Emitted(glm::vec2 uv, glm::vec3& pos) const;
    virtual bool Scatter(const Ray& rayIn, const HitPayload& payload, glm::vec3& attenuation, Ray& scattered) const = 0;
};

class Lambertian : public Material {
public:
    Lambertian(std::shared_ptr<Texture> albedo);

    virtual bool Scatter(const Ray& rayIn, const HitPayload& payload, glm::vec3& attenuation, Ray& scattered) const;

private:
    std::shared_ptr<Texture> m_Albedo;
};

class Metal : public Material {
public:
    Metal(const glm::vec3& albedo, const float& roughness = 1.0f);

    virtual bool Scatter(const Ray& rayIn, const HitPayload& payload, glm::vec3& attenuation, Ray& scattered) const;

private:
    glm::vec3 m_Albedo;
    float m_Roughness = 1.0f;
};

class Dielectric : public Material {
public:
    Dielectric(double refractivity);

    virtual bool Scatter(const Ray& rayIn, const HitPayload& payload, glm::vec3& attenuation, Ray& scattered) const;
private:
    double schlick(double cosine, double ref_idx) const;
private:
    double m_Refractivity;
};

class DiffuseLight : public Material {
public:
    DiffuseLight(std::shared_ptr<Texture> emit);

    virtual bool Scatter(const Ray& rayIn, const HitPayload& payload, glm::vec3& attenuation, Ray& scattered) const;
    glm::vec3 Emitted(glm::vec2 uv, glm::vec3& pos) const;

private:
    std::shared_ptr<Texture> m_Emit;
};