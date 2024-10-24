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
	void Init(); //데이터들 전부 로드

	void LoadSkillDescription(); //스킬 설명 정보 로드
	//이후 추가할 데이터가 추가되면 함수 추가

	bool GetDescription(string id, OUT SkillDescriptionData& info);

private:
	unordered_map<string, SkillDescriptionData> totalSkillDatas;
};
