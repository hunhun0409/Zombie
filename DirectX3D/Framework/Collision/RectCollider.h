#pragma once

class RectCollider : public Collider2D
{
public:
    RectCollider(Vector2 size);
    RectCollider(Vector3 size);
    ~RectCollider() = default;

    virtual bool IsPointCollision(Vector3 point) override;
    virtual bool IsRectCollision(RectCollider* rect) override;
    virtual bool IsCircleCollision(CircleCollider* circle) override;

    Vector3 Size() { return size * GlobalScale(); }
    Vector3 Half() { return Size() * 0.5f; }

private:
    bool IsAABB(RectCollider* rect);
    void MakeMesh();
private:
    Vector3 size;
};