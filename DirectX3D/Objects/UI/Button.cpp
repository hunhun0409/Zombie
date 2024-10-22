#include "Framework.h"

Button::Button(wstring textureFile)
    : Quad(textureFile)
{
    collider = new RectCollider(size);
    collider->SetParent(this);
}

Button::Button(Vector2 size)
    : Quad(size)
{
    collider = new RectCollider(size);
    collider->SetParent(this);
}

Button::~Button()
{
    delete collider;
}

void Button::Update()
{
    if (!isActive) return;

    if (collider->IsPointCollision(mousePos))
    {
        if (KEY_DOWN(VK_LBUTTON))
            isDownCheck = true;

        if (KEY_PRESS(VK_LBUTTON))
            state = DOWN;
        else
            state = OVER;

        if (isDownCheck && KEY_UP(VK_LBUTTON))
        {
            if (event != nullptr)
                event();

            if (paramEvent != nullptr)
                paramEvent(object);

            isDownCheck = false;
        }
    }
    else
    {
        state = NONE;

        if (KEY_UP(VK_LBUTTON))
            isDownCheck = false;
    }

    switch (state)
    {
    case Button::NONE:
        GetMaterial()->GetData().diffuse = NONE_COLOR;
        break;
    case Button::DOWN:
        GetMaterial()->GetData().diffuse = DOWN_COLOR;
        break;
    case Button::OVER:
        GetMaterial()->GetData().diffuse = OVER_COLOR;
        break;    
    }

    UpdateWorld();
    collider->UpdateWorld();
}

void Button::Render()
{
    if (!isActive) return;

    Quad::Render();
    collider->Render();
}
