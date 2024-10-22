#pragma once
#include "Objects/Basic/GameObject.h"

class Collider2D : public GameObject
{
protected:
    enum class Type
    {
        RECT, CIRCLE
    };

    const float PUSH_SPEED = 500.0f;

public:
    Collider2D();
    ~Collider2D();

    void Render();

    bool IsCollision(Collider2D* collider);

    virtual bool IsPointCollision(Vector3 point) = 0;
    virtual bool IsRectCollision(class RectCollider* rect) = 0;
    virtual bool IsCircleCollision(class CircleCollider* circle) = 0;

    void SetColor(Float4 color) { material->GetData().diffuse = color; }
    void SetColor(float r, float g, float b)
    {
        material->GetData().diffuse = { r, g, b, 1 };
    }

    static void RenderOnOff() { isRender = !isRender; }

protected:
    BasicObject* owner;

    Type type;

    Mesh<Vertex>* mesh;
private:
    static bool isRender;
};