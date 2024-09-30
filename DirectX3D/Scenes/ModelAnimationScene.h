#pragma once

class ModelAnimationScene : public Scene
{
public:
    ModelAnimationScene();
    ~ModelAnimationScene();

    virtual void Update() override;
    virtual void PreRender() override;
    virtual void Render() override;
    virtual void PostRender() override;
    virtual void GUIRender() override;

private:
    void SetDraw2();
    void SetSheath2();
    void SetEndSheath();
    void SetIdle();

private:
    ModelAnimator* modelAnimator;
};