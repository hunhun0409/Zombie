#include "Framework.h"

void CharacterMovement::ApplyAcceleration(const Vector3& inputDirection)
{
	if (isFalling) return;

	accelerationSpeed += accelerationRate * DELTA;
	accelerationSpeed = Clamp(0, maxSpeed, accelerationSpeed);

	acceleration = inputDirection * accelerationSpeed;
	velocity += acceleration * DELTA;

	if (velocity.Length() > maxSpeed)
	{
		velocity = velocity.GetNormalized() * maxSpeed;
	}
}

void CharacterMovement::ApplyDeceleration()
{
	if (velocity == Vector3()) return;

	if (acceleration.Length() == 0)
	{
		Vector3 deceleration = velocity.GetNormalized() * decelerationRate;
		velocity -= deceleration * DELTA;

		// ���ӵ��� ���ҽ�Ŵ
		accelerationSpeed -= decelerationRate * DELTA;
		accelerationSpeed = Clamp(0, maxSpeed, accelerationSpeed);

		// �ӵ��� ���� �۴ٸ� 0���� ����
		if (velocity.Length() < 0.01f)
		{
			velocity = Vector3();
		}
		if (!isFalling)
		{
			velocity.y -= deceleration.y * DELTA;
		}
	}
	else
	{
		// ���ӵ��� ���� ���� ������ �پ��
		accelerationSpeed -= decelerationRate * DELTA * 0.5f;
		accelerationSpeed = Clamp(0, maxSpeed, accelerationSpeed);
	}
}

void CharacterMovement::ApplyFriction()
{
	if (velocity == Vector3()) return;
	if (isFalling) return;

	float frictionPower = 1.0f - friction * DELTA;

	// ��� �࿡ �����ϰ� ���� ����
	velocity.x *= frictionPower;
	velocity.z *= frictionPower;

	// �ӵ��� ���� �۾����ٸ� 0���� ����
	if (velocity.Length() < 0.01f)
	{
		velocity = Vector3();
	}
}

void CharacterMovement::ApplyGravity()
{
	//Character* character = static_cast<Character*>(owner);

	//if (character == nullptr) return;

	//if (character->Pos().y <= character->GetHeight() * 0.5f) //�ٴ� ���� ���ؼ� �ϴ°ɷ� �ٲ����
	//{
	//	float height = character->GetHeight();

	//	character->Pos().y = height * 0.5f;
	//	isFalling = false;
	//	return;
	//}
	//isFalling = true;
	//velocity.y -= gravity * DELTA;
}

void CharacterMovement::Move()
{
	//ApplyGravity();
	ApplyDeceleration();
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
