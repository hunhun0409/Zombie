#include "Framework.h"

PlayerHUD::PlayerHUD(Character* owner)
	:owner(owner)
{
	ExpBar = new ProgressBar(L"Textures/UI/ExpBar_Front.png", L"Textures/UI/ExpBar_Back.png");
	ExpBar->Pos() = { CENTER_X, WIN_HEIGHT - 16 };
	ExpBar->UpdateWorld();
	ExpBar->SetAmount(0.5f);
}

PlayerHUD::~PlayerHUD()
{
	delete ExpBar;
}

void PlayerHUD::Update()
{
	amount += DELTA;
	if (amount >= 1)
	{
		amount = 0;
	}
	ExpBar->SetAmount(amount);
}

void PlayerHUD::PostRender()
{
	ExpBar->Render();
}

void PlayerHUD::GUIRender()
{
	ExpBar->GUIRender();
}
