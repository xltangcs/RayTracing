#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <memory>

#include "Material.h"
#include "SceneObject.h"

const float Infinity = std::numeric_limits<float>::max();;

class Scene 
{
public:
	Scene() = default;
	Scene(std::shared_ptr<SceneObject> object);

	void AddSceneObject(std::shared_ptr<SceneObject> object);
	void AddMaterial(std::shared_ptr<Material> material);

	bool TraceRay(const Ray& r, HitPayload& payload);

	std::shared_ptr<Material> GetMaterial(int materialIndex);
	std::shared_ptr<SceneObject> GetSceneObject(int sceneObjectIndex);

	const size_t GetSceneObjectSize() const { return m_SceneObjects.size(); }
	const size_t GetMaterialSize() const { return m_Materials.size(); }

private:
	std::vector<std::shared_ptr<SceneObject>> m_SceneObjects;
	std::vector<std::shared_ptr<Material>> m_Materials;

};