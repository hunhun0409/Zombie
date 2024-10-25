#include "Framework.h"

GameOverPanel::GameOverPanel()
	:Panel(L"Textures/UI/GameOverPanel.png")
{
	tag = "GameOverPanel";
	Load();

	Button* button = new Button(L"Textures/UI/Exit.png");
	button->SetTag("RetryButton");
	button->SetEvent(bind(&GameOverPanel::Retry, this));
	button->SetParent(this);
	button->Load();
	buttons["exit"] = button;

	Font::Get()->AddStyle("GameOverTimeFont", L"배달의민족 주아", 25.0f);
	Font::Get()->AddStyle("GameOverKillFont", L"배달의민족 주아", 25.0f, DWRITE_TEXT_ALIGNMENT_TRAILING);
}

void GameOverPanel::Update()
{
	Panel::Update();
	for (auto button : buttons)
	{
		button.second->Update();
	}
}

void GameOverPanel::Render()
{
	if (!Active()) return;
	Panel::Render();
	for (auto button : buttons)
	{
		button.second->Render();
	}

	float time = StageManager::Get()->gameTime;
	int killCount = StageManager::Get()->killCount;
	int min = time / 60;
	int sec = (int)time % 60;

	string sMin;
	if (min < 10)
	{
		sMin = "0" + to_string(min);
	}
	else
	{
		sMin = to_string(min);
	}
	string sColon = " : ";

	string sSec;
	if (sec < 10)
	{
		sSec = "0" + to_string(sec);

	}
	else
	{
		sSec = to_string(sec);
	}

	Float2 pos = Float2(GlobalPos().x, GlobalPos().y);
	pos.x += 80.0f;
	pos.y -= 8.0f;
	Float2 boxSize;
	boxSize.x = 100;
	boxSize.y = 50;
	Font::Get()->SetStyle("GameOverTimeFont");
	Font::Get()->SetColor("White");
	Font::Get()->RenderText(sMin, Float2(pos.x - 20, pos.y), boxSize);
	Font::Get()->RenderText(sColon, pos, boxSize);
	Font::Get()->RenderText(sSec, Float2(pos.x + 20, pos.y), boxSize);
	
	pos.y -= 30.0f;
	Font::Get()->SetStyle("GameOverKillFont");
	boxSize.x = 66;
	string sKillCount = to_string(killCount);
	Font::Get()->RenderText(sKillCount, pos, boxSize);
}

void GameOverPanel::Show(Vector3 pos)
{
	Timer::Get()->SetDeltaScale(0.0f);

	Panel::Show(pos);
}

void GameOverPanel::Hide()
{
	Panel::Hide();
	Timer::Get()->SetDeltaScale(1.0f);
}

void GameOverPanel::Retry()
{
	PostQuitMessage(0);
}
