#pragma once

class SkillManager : public Singleton<SkillManager>//skill �ߵ��ϰ� ������Ʈ
{
public:
	SkillManager();
	~SkillManager();

	void Init();//��� ��ų �ҷ�����

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
	unordered_map<string, ActiveSkill*> totalActiveSkills;//��ü ��Ƽ�� ��ų ���
	unordered_map<string, PassiveSkill*> totalPassiveSkills;//��ü �нú� ��ų ���
	unordered_map<string, ActiveSkill*> currentActiveSkills;//���� �÷��̾����� ����� ��Ƽ�� ��ų
	unordered_map<string, PassiveSkill*> currentPassiveSkills;//���� �÷��̾����� ����� �нú� ��ų
};
