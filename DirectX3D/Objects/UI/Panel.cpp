#include "Framework.h"

Panel::Panel(wstring textureFile)
    : Quad(textureFile)
{
    isActive = false;
}

Panel::~Panel()
{
}

void Panel::Update()
{
    if (!Active()) return;

    for (pair<string, Button*> button : buttons)
    {
        button.second->Update();
    }

    UpdateWorld();
}

void Panel::Render()
{
    if (!Active()) return;

    Quad::Render();

    for (pair<string, Button*> button : buttons)
    {
        button.second->Render();
    }
}

void Panel::AddButton(string key, Button* button, Vector3 pos)
{
    buttons[key] = button;
    buttons[key]->SetParent(this);
    buttons[key]->Pos() = pos;
}


void Panel::Show(Vector3 pos)
{
    Pos() = pos;
    isActive = true;
}
