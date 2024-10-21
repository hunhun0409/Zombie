#include "Framework.h"

PlayerHUD::PlayerHUD(Character* owner)
	:owner(owner)
{
	expBar = new ProgressBar(L"Textures/UI/ExpBar_Front.png", L"Textures/UI/ExpBar_Back.png");
	expBar->Pos() = { CENTER_X, WIN_HEIGHT - 16 };
	expBar->UpdateWorld();
	expBar->SetAmount(0.0f);

	hpBar = new ProgressBar(L"Textures/UI/hp_bar.png", L"Textures/UI/hp_bar_BG.png");
	hpBar->Pos() = { CENTER_X, CENTER_Y - 30 };
	hpBar->Scale() *= 0.3f;
	hpBar->UpdateWorld();
	hpBar->SetAmount(1.0f);

	Observer::Get()->AddFloatParamEvent("UpdateExp", bind(&PlayerHUD::UpdateExp, this, placeholders::_1));
	Observer::Get()->AddFloatParamEvent("UpdateHp", bind(&PlayerHUD::UpdateHp, this, placeholders::_1));
}

PlayerHUD::~PlayerHUD()
{
	delete expBar;
	delete hpBar;
}

void PlayerHUD::Update()
{
}

void PlayerHUD::PostRender()
{
	expBar->Render();
	hpBar->Render();
}

void PlayerHUD::GUIRender()
{
	expBar->GUIRender();
	hpBar->GUIRender();
}

void PlayerHUD::UpdateExp(float ratio)
{
	expBar->SetAmount(ratio);
}

void PlayerHUD::UpdateHp(float ratio)
{
	hpBar->SetAmount(ratio);
}
