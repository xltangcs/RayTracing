#include "Renderer.h"

namespace Utils {

	static uint32_t ConvertToRGBA(const glm::vec4& color)
	{
		uint8_t r = (uint8_t)(color.r * 255.0f);
		uint8_t g = (uint8_t)(color.g * 255.0f);
		uint8_t b = (uint8_t)(color.b * 255.0f);
		uint8_t a = (uint8_t)(color.a * 255.0f);

		uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;
		return result;
	}
}

void Renderer::OnResize(uint32_t width, uint32_t height)
{
	if (m_FinalImage)
	{
		if (m_FinalImage->GetWidth() == width && m_FinalImage->GetHeight() == height)
			return;
		m_FinalImage->Resize(width, height);
	}
	else
	{
		m_FinalImage = std::make_shared<Image>(width, height);
	}
	delete[] m_ImageData;
	m_ImageData = new uint32_t[width * height];
}

void Renderer::Render(Camera& camera, Hittable_list& world)
{
	m_Camera = &camera;
	m_World = &world;

	auto focal_length = 1.0f;
	auto viewport_height = 2.0f;
	auto viewport_width = viewport_height * camera.GetAspectRatio();
	auto viewport_u = glm::vec3(viewport_width, 0, 0);
	auto viewport_v = glm::vec3(0, viewport_height, 0);
	auto pixel_delta_u = viewport_u / (float)m_FinalImage->GetWidth();
	auto pixel_delta_v = viewport_v / (float)m_FinalImage->GetHeight();

	auto viewport_upper_left = camera.GetPosition();
	-glm::vec3(0, 0, focal_length) - viewport_u / 2.0f - viewport_v / 2.0f;
	auto pixel00_loc = viewport_upper_left + 0.5f * (pixel_delta_u + pixel_delta_v);

	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
		{
			//auto pixel_center = pixel00_loc + (float)x * pixel_delta_u + (float)y * pixel_delta_v;
			//auto ray_direction = pixel_center - m_Camera->GetPosition();
			//Ray r(m_Camera->GetPosition(), ray_direction);

			Ray r(m_Camera->GetPosition(), m_Camera->GetRayDirections()[x + y * m_FinalImage->GetWidth()]);
			glm::vec3 color = ray_color(r);

			m_ImageData[x + y * m_FinalImage->GetWidth()] = Utils::ConvertToRGBA(glm::vec4(color, 1.0f));
		}
	}
	m_FinalImage->SetData(m_ImageData);
}

bool hit(const Ray& r, float Ray_tmin, float Ray_tmax, hit_record& rec) {
	//auto center = glm::vec3(0.0f, -100.5f, -1.0f);
	//float radius = 100.0f;

	auto center = glm::vec3(0.0f, 0.0f, -1.0f);
	float radius = 0.4f;

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
	rec.set_face_normal(r, outward_normal);

	return true;
}

glm::vec3 Renderer::ray_color(Ray& r)
{
	hit_record rec;
	if (m_World->hit(r, 0.0f, std::numeric_limits<float>::infinity(), rec)) {
		//return glm::vec3(0.0f, 0.2f, 0.8f);
		return 0.5f * (rec.normal + glm::vec3(1.0f));
	}
	glm::vec3 unit_direction = glm::normalize(r.direction());
	auto a = 0.5f * (unit_direction.y + 1.0f);
	return (1.0f - a) * glm::vec3(1.0f, 1.0f, 1.0f) + a * glm::vec3(0.5f, 0.7f, 1.0f);
}

