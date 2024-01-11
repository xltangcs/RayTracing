#include "Scene.h"

Scene::Scene(std::shared_ptr<SceneObject> object)
{
    AddSceneObject(object);
}



void Scene::AddSceneObject(std::shared_ptr<SceneObject> object)
{
    m_SceneObjects.push_back(object);
}

void Scene::AddMaterial(std::shared_ptr<Material> material)
{
    m_Materials.push_back(material);
}

bool Scene::TraceRay(const Ray& r, HitPayload& payload)
{
    HitPayload temp;
    bool hit_anything = false;
    float closest_so_far = Infinity;

    for (const auto& object : m_SceneObjects) {
        if (object->Hit(r, 0.0001f, closest_so_far, temp)) {
            hit_anything = true;
            closest_so_far = temp.HitDistance;
            payload = temp;
        }
    }

    return hit_anything;
}

std::shared_ptr<Material> Scene::GetMaterial(int materialIndex)
{
    if (materialIndex < m_Materials.size()) {
        return m_Materials[materialIndex];
    }
    else {
        return nullptr;
    }
}

std::shared_ptr<SceneObject> Scene::GetSceneObject(int sceneObjectIndex)
{
    if (sceneObjectIndex < m_SceneObjects.size()) {
        return m_SceneObjects[sceneObjectIndex];
    }
    else {
        return nullptr;
    }
}
