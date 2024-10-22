#pragma once

class CircleCollider : public Collider2D
{
private:
    const UINT VERTEX_COUNT = 64;

public:
    CircleCollider(float radius = 1.0f);
    ~CircleCollider() = default;
        
    virtual bool IsPointCollision(Vector3 point) override;
    virtual bool IsRectCollision(RectCollider* rect) override;
    virtual bool IsCircleCollision(CircleCollider* circle) override;

    float Radius() { return radius * max(GlobalScale().x, GlobalScale().y); }

private:
    void MakeMesh();

private:
    float radius;
};