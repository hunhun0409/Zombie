#include "Framework.h"

bool Collider2D::isRender = true;

Collider2D::Collider2D()
    :GameObject(L"Basic/Collider.hlsl")
{
    mesh = new Mesh<Vertex>();

    SetColor(0, 1, 0);
}

Collider2D::~Collider2D()
{
    delete mesh;
}

void Collider2D::Render()
{
    if (!Active()) return;
    if (!isRender) return;

    SetRender();

    mesh->Draw(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}

bool Collider2D::IsCollision(Collider2D* collider)
{
    if (!Active()) return false;
    if (!collider->Active()) return false;

    switch (collider->type)
    {
    case Collider2D::Type::RECT:
        return IsRectCollision((RectCollider*)collider);
    case Collider2D::Type::CIRCLE:
        return IsCircleCollision((CircleCollider*)collider);
    }

    return false;
}
