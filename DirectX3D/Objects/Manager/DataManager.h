#pragma once

class DataManager : public Singleton<DataManager>
{
public:
	struct SkillDescriptionData
	{
		string id;
		string name;
		string descriptions[5];
	};

private:
	friend class Singleton;
	DataManager();
	~DataManager() = default;

public:
	void Init(); //�����͵� ���� �ε�

	void LoadSkillDescription(); //��ų ���� ���� �ε�
	//���� �߰��� �����Ͱ� �߰��Ǹ� �Լ� �߰�

	bool GetDescription(string id, OUT SkillDescriptionData& info);

private:
	unordered_map<string, SkillDescriptionData> totalSkillDatas;
};
