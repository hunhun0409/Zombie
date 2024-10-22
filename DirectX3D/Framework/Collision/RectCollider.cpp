#include "Framework.h"

RectCollider::RectCollider(Vector2 size)
    :size(Vector3(size.x, size.y, 0))
{
    type = Type::RECT;

    MakeMesh();
    mesh->CreateMesh();
}

RectCollider::RectCollider(Vector3 size)
    : size(size)
{
    type = Type::RECT;

    MakeMesh();
    mesh->CreateMesh();
}

bool RectCollider::IsPointCollision(Vector3 point)
{
    Vector3 half = Half();
    Vector3 worldPos = GlobalPos();

    return (point.x >= worldPos.x - half.x && point.x <= worldPos.x + half.x) &&
        (point.y >= worldPos.y - half.y && point.y <= worldPos.y + half.y);
}

bool RectCollider::IsRectCollision(RectCollider* rect)
{
    return IsAABB(rect);
}

bool RectCollider::IsCircleCollision(CircleCollider* circle)
{
    Vector3 circlePos = circle->GlobalPos();
    float radius = circle->Radius();

    // ���� �߽ɰ� �簢���� �ִ� �Ÿ��� ���
    Vector3 half = Half();
    Vector3 rectPos = GlobalPos();

    // ���� �߽ɿ��� ���� ����� �簢�� ���� ���� ã��
    float closestX = max(rectPos.x - half.x, min(circlePos.x, rectPos.x + half.x));
    float closestY = max(rectPos.y - half.y, min(circlePos.y, rectPos.y + half.y));

    // ���� �߽ɰ� ���� ����� �� ������ �Ÿ��� ���
    float distanceX = circlePos.x - closestX;
    float distanceY = circlePos.y - closestY;

    float distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);
    return distanceSquared <= (radius * radius);
    
}


bool RectCollider::IsAABB(RectCollider* rect)
{
    Vector3 thisHalf = Half();
    Vector3 otherHalf = rect->Half();

    Vector3 thisPos = GlobalPos();
    Vector3 otherPos = rect->GlobalPos();

    return (abs(thisPos.x - otherPos.x) <= (thisHalf.x + otherHalf.x)) &&
        (abs(thisPos.y - otherPos.y) <= (thisHalf.y + otherHalf.y));
}


void RectCollider::MakeMesh()
{
    vector<Vertex>& vertices = mesh->Vertices();

    Vector3 halfSize = size * 0.5f;

    vertices.emplace_back(-halfSize.x, +halfSize.y, 0);
    vertices.emplace_back(+halfSize.x, +halfSize.y, 0);

    vertices.emplace_back(+halfSize.x, +halfSize.y, 0);
    vertices.emplace_back(+halfSize.x, -halfSize.y, 0);

    vertices.emplace_back(+halfSize.x, -halfSize.y, 0);
    vertices.emplace_back(-halfSize.x, -halfSize.y, 0);

    vertices.emplace_back(-halfSize.x, -halfSize.y, 0);
    vertices.emplace_back(-halfSize.x, +halfSize.y, 0);
}
