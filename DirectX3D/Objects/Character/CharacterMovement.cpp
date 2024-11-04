#include "Framework.h"

void CharacterMovement::ApplyAcceleration(const Vector3& inputDirection) {
	accelerationSpeed += accelerationRate * DELTA;
	accelerationSpeed = Clamp(0, maxSpeed, accelerationSpeed);

	acceleration = inputDirection * accelerationSpeed;
	velocity += acceleration * DELTA;
}

void CharacterMovement::ApplyFriction() {
	if (velocity == Vector3()) return;

	float frictionPower = 1.0f - friction * DELTA;
	velocity.x *= frictionPower;
	velocity.z *= frictionPower;

	if (velocity.Length() < 0.01f){
		velocity = Vector3();
		acceleration = Vector3();
	}
}

void CharacterMovement::ApplyGravity()
{
	//Character* character = static_cast<Character*>(owner);

	//if (character == nullptr) return;

	//if (character->Pos().y <= character->GetHeight() * 0.5f) //바닥 높이 구해서 하는걸로 바꿔야함
	//{
	//	float height = character->GetHeight();

	//	character->Pos().y = height * 0.5f;
	//	isFalling = false;
	//	return;
	//}
	//isFalling = true;
	//velocity.y -= gravity * DELTA;
}

void CharacterMovement::Update()
{
	ApplyFriction();

	Matrix rotY = XMMatrixRotationY(CAM->Rot().y);
	direction = XMVector3TransformCoord(velocity, rotY);
	owner->Pos() += direction *  DELTA;
}

void CharacterMovement::Stop()
{
	velocity = Vector3();
	acceleration = Vector3();
}
