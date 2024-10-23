#pragma once

class Panel : public Quad
{
public:
    Panel(wstring textureFile);
    ~Panel();

    void Update();
    void Render();
    void GUIRender();

    void AddButton(string key, Button* button, Vector3 pos);

    virtual void Show(Vector3 pos);
    virtual void Hide();
protected:
    Vector2 offset;
    unordered_map<string, Button*> buttons;
};