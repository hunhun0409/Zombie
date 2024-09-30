#pragma once

class InstanceObjectManager : public Singleton<InstanceObjectManager>
{
private:
	const UINT SIZE = 10;
private:
	friend class Singleton;
	InstanceObjectManager();
	~InstanceObjectManager();

public:
	void Update();
	void Render();
	void PostRender();
	void GUIRender();

	void Add(string key, string name, Vector3 scale = Vector3(1, 1, 1), Vector3 colliderScale = Vector3(1, 1, 1));

	void Remove(string key);
	void Clear();

	void Spawn(string key, Vector3 pos, Vector3 rot = Vector3(), Vector3 scale = Vector3(1, 1, 1));

private:
	void Collision();

	void Save(string key);
	void Load(string key);

private:
	unordered_map<string, ModelInstancing*>::iterator iter;
	unordered_map<string, ModelInstancing*> totalInstancies;

	typedef vector<InstanceObject*> InstanceObjects;
	unordered_map<string, InstanceObjects> totalObjects;

	//ºÒÇÊ¿ä
	unordered_map<string, Transform*> totalModelTransforms;
	unordered_map<string, Transform*> totalColliderTransforms;

	bool isChanged = false;
};