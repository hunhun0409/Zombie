#pragma once

class UIManager : public Singleton<UIManager>
{
private:
	struct UIData
	{
		Panel* panel;
		Vector3 pos;
	};

private:
	friend class Singleton;
	UIManager() = default;
	~UIManager();

public:
	void Update();
	void Render();
	void GUIRender();

	void Add(string key, Panel* panel, Vector3 pos);
	void Remove(string key);

	void Show(string key);
	void Hide(string key);

private:
	unordered_map<string, UIData> totalPanels;
};
