#pragma once

class SkillManager : public Singleton<SkillManager>//skill �ߵ��ϰ� ������Ʈ
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
