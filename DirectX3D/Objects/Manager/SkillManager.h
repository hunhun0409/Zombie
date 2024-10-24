#pragma once

class SkillManager : public Singleton<SkillManager>//skill 발동하고 업데이트
{
public:
	SkillManager();
	~SkillManager();

	void Init();//모든 스킬 불러오기

	void Update();
	void Render();

	bool Add(string key);
	bool LevelUp(string key);

	void Clear();

	vector<Skill*> GetThreeRandomSkills();

	bool HasActiveSkill(string key);
	bool HasPassiveSkill(string key);

	PassiveSkill* GetPassiveSkill(string key);
	ActiveSkill* GetActiveSkill(string key);
	int GetSkillLevel(string key);

private:
	unordered_map<string, ActiveSkill*> totalActiveSkills;//전체 액티브 스킬 목록
	unordered_map<string, PassiveSkill*> totalPassiveSkills;//전체 패시브 스킬 목록
	unordered_map<string, ActiveSkill*> currentActiveSkills;//현재 플레이어한테 적용된 액티브 스킬
	unordered_map<string, PassiveSkill*> currentPassiveSkills;//현재 플레이어한테 적용된 패시브 스킬
};
