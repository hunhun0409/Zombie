#pragma once

class CharacterController;

class InstanceCharacter : public BasicObject
{
protected:
	const float PATH_UPDATE_INTERVAL = 0.5f;
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
	void SetTarget(Transform* target);
protected:
	virtual void SetState(UINT state, float playRate = 1.0f, float takeTime = 0.2f, bool canOverride = false);

	void SetEvent(int clip, Event event, float timeRatio);
	void ExecuteEvent();

	void SetPath();

public:
	virtual void TakeDamage(float damage);

	
protected:
	Transform* target = nullptr;

	class AIController* controller;

	CapsuleCollider* collider;

	ModelAnimatorInstancing* instancing;
	ModelAnimatorInstancing::Motion* motion;

	string name;
	UINT index;

	unordered_map<int, string> states;

	vector<map<float, Event>> totalEvents;
	vector<map<float, Event>::iterator> eventIters;

	//status

	float speed = 1.0f;

	float maxHp = 100.0f;
	float curHp = 100.0f;

	//collision
	float separationRadius = 5.0f;
	float separationStrength = 10.0f;

	Vector3 destPos;
	vector<Vector3> path;

	Terrain* terrain;
	AStar* aStar;

	float pathUpdateInterval = 0;

public:
	Transform* transform;
	int curState = 0;
	Vector3 velocity = Vector3();

};

