#include "Framework.h"

UIManager::~UIManager()
{
	for (auto panel : totalPanels)
	{
		delete panel.second.panel;
	}
}

void UIManager::Update()
{
	for (auto panel : totalPanels)
	{
		if (!panel.second.panel->Active()) continue;

		panel.second.panel->Update();
	}
}

void UIManager::Render()
{
	for (auto panel : totalPanels)
	{
		if (!panel.second.panel->Active()) continue;

		panel.second.panel->Render();
	}
}

void UIManager::GUIRender()
{
	for (auto panel : totalPanels)
	{
		if (!panel.second.panel->Active()) continue;

		panel.second.panel->GUIRender();
	}
}

void UIManager::Add(string key, Panel* panel, Vector3 pos)
{
	if (totalPanels.count(key) > 0) return;

	UIData data;
	data.panel = panel;
	data.pos = pos;

	totalPanels[key] = data;
}

void UIManager::Remove(string key)
{
	if (totalPanels.count(key) == 0) return;

	delete totalPanels[key].panel;
	totalPanels.erase(key);
}

void UIManager::Show(string key)
{
	if (totalPanels.count(key) == 0) return;
	
	totalPanels[key].panel->Show(totalPanels[key].pos);
}

void UIManager::Hide(string key)
{
	if (totalPanels.count(key) == 0) return;

	totalPanels[key].panel->Hide();
}
