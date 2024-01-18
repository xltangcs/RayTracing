#include "Scene.h"

AABB SurroundingBox(AABB box0, AABB box1)
{
    glm::vec3 small(std::min(box0.GetMinBound().x, box1.GetMinBound().x),
        std::min(box0.GetMinBound().y, box1.GetMinBound().y),
        std::min(box0.GetMinBound().z, box1.GetMinBound().z)
    );

    glm::vec3 big(std::max(box0.GetMaxBound().x, box1.GetMaxBound().x),
        std::max(box0.GetMaxBound().y, box1.GetMaxBound().y),
        std::max(box0.GetMaxBound().z, box1.GetMaxBound().z)
    );

    return AABB(small, big);
}

Scene::Scene(std::shared_ptr<SceneObject> object)
{
    AddSceneObject(object);
}

void Scene::AddSceneObject(std::shared_ptr<SceneObject> object)
{
    m_SceneObjects.push_back(object);
}

bool Scene::Hit(const Ray& ray, float t_min, float t_max, HitPayload& payload) const
{
    HitPayload temp;
    bool hit_anything = false;
    float closest_so_far = Infinity;

    for (const auto& object : m_SceneObjects) {
        if (object->Hit(ray, 0.0001f, closest_so_far, temp)) {
            hit_anything = true;
            closest_so_far = temp.HitDistance;
            payload = temp;
        }
    }

    return hit_anything;
}

bool Scene::BoundingBox(float t0, float t1, AABB& output_box) const
{
    if (m_SceneObjects.empty()) return false;

    AABB tempbox;
    bool firstbox = true;

    for (const auto& object : m_SceneObjects) {
        if (!object->BoundingBox(t0, t1, tempbox) )
        {
            return false;
        }
        output_box = firstbox ? tempbox : SurroundingBox(output_box, tempbox);
        firstbox = false;
    }

    return true;
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

BVHNode::BVHNode(Scene& scene, float t0, float t1)
    : BVHNode(scene.GetSceneObjects(), 0, scene.GetSceneObjectSize(), t0, t1)
{
}

BVHNode::BVHNode(std::vector<std::shared_ptr<SceneObject>>& objects, size_t start, size_t end, float t0, float t1)
{
    int axis = Toffee::Random::UInt(0, 2);

    auto comparator = (axis == 0) ? box_x_compare
                    : (axis == 1) ? box_y_compare
                    : box_z_compare;

    size_t object_span = end - start;

    if (object_span == 1) 
    {
        m_LeftChild  = objects[start];
        m_RightChild = objects[start];
    }
    else if (object_span == 2) 
    {
        if (comparator(objects[start], objects[start + 1])) {
            m_LeftChild  = objects[start];
            m_RightChild = objects[start + 1];
        }
        else {
            m_LeftChild  = objects[start + 1];
            m_RightChild = objects[start];
        }
    }
    else 
    {
        std::sort(objects.begin() + start, objects.begin() + end, comparator);

        auto mid = start + object_span / 2;
        m_LeftChild = std::make_shared<BVHNode>(objects, start, mid, t0, t1);
        m_RightChild = std::make_shared<BVHNode>(objects, mid, end, t0, t1);
    }

    AABB box_left, box_right;

    if (!m_LeftChild->BoundingBox(t0, t1, box_left)
        || !m_RightChild->BoundingBox(t0, t1, box_right)
        )
        std::cerr << "No bounding box in bvh_node constructor.\n";

    m_BoundingBox = SurroundingBox(box_left, box_right);
}

bool BVHNode::Hit(const Ray& ray, float t_min, float t_max, HitPayload& payload) const
{
    if (! m_BoundingBox.Hit(ray, t_min, t_max))
        return false;

    bool lefthit  = m_LeftChild ->Hit(ray, t_min, t_max, payload);
    bool righthit = m_RightChild->Hit(ray, t_min, lefthit ? payload.HitDistance : t_max, payload);

    return lefthit || righthit;
}

bool BVHNode::BoundingBox(float t0, float t1, AABB& output_box) const
{
    output_box = m_BoundingBox;
    return true;
}

bool BVHNode::box_compare(const std::shared_ptr<SceneObject> a, const std::shared_ptr<SceneObject> b, int axis_index)
{
    AABB boxa, boxb;

    if (!a->BoundingBox(0, 0, boxa) || !b->BoundingBox(0, 0, boxb))
    {
        std::cerr << "No bounding box in bvh_node constructor.\n";
    }

    return boxa.GetMinBound()[axis_index] < boxb.GetMinBound()[axis_index];
}

bool BVHNode::box_x_compare(const std::shared_ptr<SceneObject> a, const std::shared_ptr<SceneObject> b)
{
    return box_compare(a, b, 0);
}

bool BVHNode::box_y_compare(const std::shared_ptr<SceneObject> a, const std::shared_ptr<SceneObject> b)
{
    return box_compare(a, b, 1);
}

bool BVHNode::box_z_compare(const std::shared_ptr<SceneObject> a, const std::shared_ptr<SceneObject> b)
{
    return box_compare(a, b, 2);
}

Cube::Cube(const glm::vec3& a, const glm::vec3& b, int materialindex)
{
    auto min = glm::vec3(fmin(a.x, b.x), fmin(a.y, b.y), fmin(a.z, b.z));
    auto max = glm::vec3(fmax(a.x, b.x), fmax(a.y, b.y), fmax(a.z, b.z));

    auto dx = glm::vec3(max.x - min.x, 0, 0);
    auto dy = glm::vec3(0, max.y - min.y, 0);
    auto dz = glm::vec3(0, 0, max.z - min.z);

    m_CubeScene->AddSceneObject(std::make_shared<Quad>(glm::vec3(min.x, min.y, max.z), dx, dy, materialindex)); // Ç°
    m_CubeScene->AddSceneObject(std::make_shared<Quad>(glm::vec3(max.x, min.y, max.z), -dz, dy, materialindex)); // ÓÒ
    m_CubeScene->AddSceneObject(std::make_shared<Quad>(glm::vec3(max.x, min.y, min.z), -dx, dy, materialindex)); // ºó
    m_CubeScene->AddSceneObject(std::make_shared<Quad>(glm::vec3(min.x, min.y, min.z), dz, dy, materialindex)); // ×ó
    m_CubeScene->AddSceneObject(std::make_shared<Quad>(glm::vec3(min.x, max.y, max.z), dx, -dz, materialindex)); // ¶¥
    m_CubeScene->AddSceneObject(std::make_shared<Quad>(glm::vec3(min.x, min.y, min.z), dx, dz, materialindex)); // µ×
}
