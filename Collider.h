#pragma once
enum class ColliderType
{
   SPHERE,
   BOX,
   OBOX,
};

class Collider : public Transform
{
    friend class GameObject;
public:
    shared_ptr<Mesh>        mesh;
    shared_ptr<Shader>      shader;
    ColliderType            type;
    bool                    visible;
public:
    Collider(ColliderType type);
    ~Collider();
    void                Update(class GameObject* ob);
    void                Render();
    void                RenderDetail();
    bool                Intersect(Collider* target);
    bool                Intersect(Ray Ray,Vector3& Hit);
};

