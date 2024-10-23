#pragma once

class SkillManager : public Singleton<SkillManager>//skill 발동하고 업데이트
{
public:
	SkillManager();
	~SkillManager();

	void Update();
	void Render();

	void Add(string key, Skill* skill);
	void LevelUp(string key);
private:

};
