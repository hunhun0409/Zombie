#pragma once

class CollisionScene : public Scene
{
public:
    CollisionScene();
    ~CollisionScene();

    virtual void Update() override;
    virtual void PreRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual void GUIRender() override;

private:
    QuadTree* qt;
    vector<Collider*> colliders;
    vector<Direction> dirs;
    int index = 0;
    bool b = true;

    float rate = 0.05f;
    float interval = 0.0f;
};