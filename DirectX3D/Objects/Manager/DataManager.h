#pragma once

class DataManager : public Singleton<DataManager>
{
public:
	struct SkillDescriptionInfo
	{
		string id;
		string name;
		vector<string> descriptions;
	};



private:
	friend class Singleton;
	DataManager() = default;
	~DataManager() = default;

public:
	void Init(); //�����͵� ���� �ε�

	void LoadSkillDescription(); //��ų ���� ���� �ε�
	//���� �߰��� �����Ͱ� �߰��Ǹ� �Լ� �߰�

	void GetDescription(string id, OUT SkillDescriptionInfo& info);





private:
	unordered_map<string, SkillDescriptionInfo> totalSkillInfos;

};
