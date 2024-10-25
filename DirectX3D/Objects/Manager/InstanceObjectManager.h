#pragma once

class InstanceObjectManager : public Singleton<InstanceObjectManager>
{
private:
	const UINT SIZE = 48;
private:
	friend class Singleton;
	InstanceObjectManager();
	~InstanceObjectManager();

public:
	void Update();
	void Render();
	void PostRender();
	void GUIRender();

	void Add(string key, string name);

	void Remove(string key);
	void Clear();

	void Spawn(string key, Vector3 pos, Vector3 rot = Vector3(), Vector3 scale = Vector3(1, 1, 1));

	void SetShader(wstring file);

private:
	unordered_map<string, ModelInstancing*>::iterator iter;
	unordered_map<string, ModelInstancing*> totalInstancies;

	typedef vector<InstanceObject*> InstanceObjects;
	unordered_map<string, InstanceObjects> totalObjects;

	bool isChanged = false;
};