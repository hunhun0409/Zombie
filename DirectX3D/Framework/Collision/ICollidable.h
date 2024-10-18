#pragma once

class ICollidable
{
public:
	virtual void OnCollision(Collider* other) = 0;
	virtual string GetName() = 0;
	virtual ~ICollidable() = default;
};
