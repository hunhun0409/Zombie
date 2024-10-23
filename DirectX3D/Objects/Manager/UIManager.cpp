#include "Framework.h"

UIManager::~UIManager()
{
	for (auto panel : totalPanels)
	{
		delete panel.second;
	}
}

void UIManager::Update()
{
	for (auto panel : totalPanels)
	{
		if (!panel.second->Active()) continue;

		panel.second->Update();
	}
}

void UIManager::Render()
{
	for (auto panel : totalPanels)
	{
		if (!panel.second->Active()) continue;

		panel.second->Render();
	}
}

void UIManager::Add(string key, Panel* panel)
{
	if (totalPanels.count(key) > 0) return;

	totalPanels[key] = panel;
}

void UIManager::Remove(string key)
{
	if (totalPanels.count(key) == 0) return;

	delete totalPanels[key];
	totalPanels.erase(key);
}

void UIManager::Show(string key, Vector3 pos)
{
	if (totalPanels.count(key) == 0) return;
	
	totalPanels[key]->Show(pos);
}

void UIManager::Hide(string key)
{
	if (totalPanels.count(key) == 0) return;

	totalPanels[key]->Hide();
}
