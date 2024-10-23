#pragma once

class UIManager : public Singleton<UIManager>
{
private:
	friend class Singleton;
	UIManager() = default;
	~UIManager();

public:
	void Update();
	void Render();

	void Add(string key, Panel* panel);
	void Remove(string key);

	void Show(string key, Vector3 pos);
	void Hide(string key);

private:
	unordered_map<string, Panel*> totalPanels;
};
