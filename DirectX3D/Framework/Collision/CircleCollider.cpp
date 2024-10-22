#include "Framework.h"

CircleCollider::CircleCollider(float radius)
    : radius(radius)
{
    type = Type::CIRCLE;

    MakeMesh();
    mesh->CreateMesh();
}

bool CircleCollider::IsPointCollision(Vector3 point)
{
    float distance = Distance(point, GlobalPos());

    return distance < Radius();
}

bool CircleCollider::IsRectCollision(RectCollider* rect)
{
    return rect->IsCircleCollision(this);
}

bool CircleCollider::IsCircleCollision(CircleCollider* circle)
{
    float distance = Distance(circle->GlobalPos(), GlobalPos());

    return distance < Radius() + circle->Radius();
}

void CircleCollider::MakeMesh()
{
    vector<Vertex>& vertices = mesh->Vertices();
    vertices.reserve(VERTEX_COUNT * 2);  // 선분당 2개의 정점
    float theta = XM_2PI / VERTEX_COUNT;

    for (UINT i = 0; i < VERTEX_COUNT; i++)
    {
        float x = cos(theta * i) * radius;
        float y = sin(theta * i) * radius;
        vertices.emplace_back(x, y, 0);

        float nextX = cos(theta * (i + 1)) * radius;  // 괄호 추가
        float nextY = sin(theta * (i + 1)) * radius;  // 괄호 추가
        vertices.emplace_back(nextX, nextY, 0);
    }


}
