#pragma once

class Skill
{
public:
	Skill(string id, string name);
	~Skill() = default;

	virtual void Activate() = 0;
	virtual void Deactivate() = 0;
	virtual void LevelUp() = 0;
	virtual string GetDescription() const = 0;

	virtual void SetOwner(class Character* owner) { this->owner = owner; }

protected:
	class Character* owner = nullptr;
	string id;
	string name;
	int level = 1;
};
