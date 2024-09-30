#pragma once

class Zombie 
{
private:
	enum State
	{
		STAND_UP,
		WALK, RUN, HIT, DIE
	};
public:
	Zombie(Transform* transform, ModelAnimatorInstancing* instancing, UINT index);
	~Zombie();

	void Update();
	void Render();
	void PostRender();

	CapsuleCollider* GetCollider() { return collider; }
	Transform* GetTransform() { return transform; }

	void SetTarget(Transform* target) { this->target = target; }

private:
	void SetEvent(int clip, Event event, float timeRatio);
	void ExecuteEvent();
	
	void EndStandUp();
	void EndHit();
	void EndDying();


public:
	void Hit();
	void Spawn(Vector3 pos);

	void SetState(State state, float playRate = 1.0f, float takeTime = 0.2f);

	void Move();
	void UpdateHpBar();




private:

	Transform* transform;
	Transform* root;

	CapsuleCollider* collider;
	
	ModelAnimatorInstancing* instancing;
	ModelAnimatorInstancing::Motion* motion;
	ProgressBar* hpBar;
	Transform* target;

	UINT index;

	float maxHp = 100.0f;
	float curHp = 100.0f;

	float speed = 3.0f;
	Vector3 velocity;
	Vector3 hpBarPos;

	State curState = STAND_UP;

	vector<map<float, Event>> totalEvents;
	vector<map<float, Event>::iterator> eventIters;

	
};
