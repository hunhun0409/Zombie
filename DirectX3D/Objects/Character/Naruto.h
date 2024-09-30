#pragma once

class Naruto : public Character
{
private:
	enum State
	{
		IDLE, RUN_FORWARD, RUN_BACK, RUN_LEFT, RUN_RIGHT, THROW
	};
public:
	Naruto();
	~Naruto();

	void Update();
	void Render();
	void PostRender();
	void GUIRender();

	void Idle();
	void ThrowKunai();
private:
	void Control();
	void Move();
	void Rotate();
	void Attack();
	void SetAnimation();

	void SetState(State state, float playRate = 1.0f, float takeTime = 0.2f);


private:
	Transform* rightHand;
	Model* kunai;

	Quad* crossHair;

	State curState = IDLE;

	float moveSpeed = 200.0f;
	float rotXSpeed = 0.5f;
	float rotYSpeed = 5.0f;
	float deceleration = 10.0f;

	Vector3 targetPos;
	bool isTarget;

	POINT clientCenterPos = { WIN_WIDTH >> 1, WIN_HEIGHT >> 1 };

	LightBuffer::Light* light;
};
