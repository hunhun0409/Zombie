#pragma once

class CharacterController;

class InstanceCharacter
{
protected:
	const float PATH_UPDATE_INTERVAL = 1.0f;
public:
	InstanceCharacter(string name, Transform* transform, ModelAnimatorInstancing* instancing, UINT index);
	~InstanceCharacter();

	virtual void Update();
	virtual void Render();
	virtual void GUIRender();

	CapsuleCollider* GetCollider() { return collider; }
	Transform* GetTransform() { return transform; }

	virtual void Spawn(Vector3 pos);
	float GetHeight() { return collider->Height() + collider->Radius() * 2; }
	void ApplySeperation(vector<InstanceCharacter*>& others);
	void SetAStar(AStar* aStar) { this->aStar = aStar; }
	void SetTerrain(Terrain* terrain) { this->terrain = terrain; }

protected:
	virtual void SetState(UINT state, float playRate = 1.0f, float takeTime = 0.2f, bool canOverride = false);

	void SetEvent(int clip, Event event, float timeRatio);
	void ExecuteEvent();

	void SetPath();

public:
	virtual void TakeDamage(float damage);

public:
	Character* target = nullptr;
protected:
	Transform* transform;
	CapsuleCollider* collider;

	ModelAnimatorInstancing* instancing;
	ModelAnimatorInstancing::Motion* motion;

	string name;
	UINT index;

	int curState = 0;
	unordered_map<int, string> states;

	vector<map<float, Event>> totalEvents;
	vector<map<float, Event>::iterator> eventIters;

	//status
	Vector3 velocity = Vector3();
	float speed = 1.0f;

	float maxHp = 100.0f;
	float curHp = 100.0f;

	//collision
	float separationRadius = 3.0f;
	float separationStrength = 5.0f;

	Vector3 destPos;
	vector<Vector3> path;

	Terrain* terrain;
	AStar* aStar;

	float pathUpdateInterval = 0;
};

