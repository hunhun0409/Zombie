#pragma once
class InstanceBuilding : public InstanceObject
{
public:
	InstanceBuilding(string name, Transform* transform);
	~InstanceBuilding();

public:
	UINT index;
};
