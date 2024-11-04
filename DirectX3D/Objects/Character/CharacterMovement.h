#pragma once

class Character;

class CharacterMovement
{
public:
	CharacterMovement() = default;
	~CharacterMovement() = default;

	void ApplyAcceleration(const Vector3& inputDirection);

	void ApplyFriction();
	void ApplyGravity();
	void Update();

	void Stop();

	void SetOwner(Transform* owner) { this->owner = owner; }

	float& MaxSpeed() { return maxSpeed; }

	Vector3 GetVelocity() { return velocity; }
public:
	Transform* owner = nullptr;

	Vector3 velocity = Vector3();
	Vector3 acceleration = Vector3();

	bool isFalling = false;

	Vector3 direction;
	float maxSpeed = 100.0f;


	float gravity = 10.0f;

	float friction = 10.0f;
	
	float accelerationSpeed = 5.0f;
	
	float accelerationRate = 1500.0f;
	float decelerationRate = 500.0f;
};
