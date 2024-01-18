#pragma once

#include <glm/glm.hpp>

#include <vector>
#include <memory>
#include <iostream>

#include "Toffee/Random.h"

#include "SceneObject.h"

const float Infinity = std::numeric_limits<float>::max();;

class Scene : public SceneObject
{
public:
	Scene() = default;
	Scene(std::shared_ptr<SceneObject> object);

	void AddSceneObject(std::shared_ptr<SceneObject> object);
	//void AddCube(std::shared_ptr<SceneObject> object);

	virtual bool Hit(const Ray& ray, float t_min, float t_max, HitPayload& payload) const;
	virtual bool BoundingBox(float t0, float t1, AABB& output_box) const;

	std::shared_ptr<SceneObject> GetSceneObject(int sceneObjectIndex);

	const size_t GetSceneObjectSize() const { return m_SceneObjects.size(); }

	std::vector<std::shared_ptr<SceneObject>>& GetSceneObjects() { return m_SceneObjects; }

private:
	std::vector<std::shared_ptr<SceneObject>> m_SceneObjects;
};

class BVHNode : public SceneObject
{
public:
	BVHNode() = default;
	BVHNode(Scene& scene, float t0, float t1);
	BVHNode(std::vector<std::shared_ptr<SceneObject>>& objects, size_t start, size_t end, float t0, float t1);


	virtual bool Hit(const Ray& ray, float t_min, float t_max, HitPayload& payload) const;
	virtual bool BoundingBox(float t0, float t1, AABB& output_box) const;

	static bool box_compare(const std::shared_ptr<SceneObject> a, const std::shared_ptr<SceneObject> b, int axis_index);
	static bool box_x_compare(const std::shared_ptr<SceneObject> a, const std::shared_ptr<SceneObject> b);
	static bool box_y_compare(const std::shared_ptr<SceneObject> a, const std::shared_ptr<SceneObject> b);
	static bool box_z_compare(const std::shared_ptr<SceneObject> a, const std::shared_ptr<SceneObject> b);
private:
	std::shared_ptr<SceneObject> m_LeftChild;
	std::shared_ptr<SceneObject> m_RightChild;
	AABB m_BoundingBox;
};

class Cube : public Scene
{
public:
	Cube(const glm::vec3& a, const glm::vec3& b, int materialindex);
private:
	std::shared_ptr<Scene> m_CubeScene;
};
