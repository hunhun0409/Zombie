#include "Framework.h"

LevelUpButton::LevelUpButton()
	:Button(L"Textures/UI/SkillSelection.png")
{
	Font::Get()->AddStyle("ButtonFontTitle", L"Amiri", 20);
	Font::Get()->AddStyle("ButtonFontDescription", L"Amiri", 16);
	
	icon = new Quad(Vector2(50, 50));
	icon->SetParent(this);
	icon->SetTag("LevelUpButtonIcon");
}

LevelUpButton::~LevelUpButton()
{
	delete icon;
}

void LevelUpButton::Update()
{
	Button::Update();

	icon->UpdateWorld();
}

void LevelUpButton::Render()
{
	Button::Render();
	icon->Render();

	Float2 boxSize;
	boxSize.x = size.x;
	boxSize.y = size.y;

	descriptionOffset.y = size.y * 0.6f;

	Float2 pos;
	pos.x = GlobalPos().x + descriptionOffset.x;
	pos.y = GlobalPos().y - descriptionOffset.y;

	Font::Get()->SetStyle("ButtonFontDescription");
	Font::Get()->SetColor("White");
	Font::Get()->RenderText(data.description, pos, boxSize); 

	titleOffset.x = size.x * 0.22f;
	titleOffset.y = size.y * 0.3f;

	pos.x = GlobalPos().x - titleOffset.x;
	pos.y = GlobalPos().y - titleOffset.y;

	Font::Get()->SetStyle("ButtonFontTitle");
	Font::Get()->SetColor("White");
	Font::Get()->RenderText(data.title, pos, boxSize);
}

void LevelUpButton::GUIRender()
{
	Button::GUIRender();
	icon->GUIRender();
}

void LevelUpButton::SetData(UpgradeData data)
{
	this->data = data;
	icon->GetMaterial()->SetDiffuseMap(data.iconTextureFile);
}

void LevelUpButton::Load()
{
	Button::Load();

	icon->Load();
}
