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
	void Init(); //데이터들 전부 로드

	void LoadSkillDescription(); //스킬 설명 정보 로드
	//이후 추가할 데이터가 추가되면 함수 추가

	void GetDescription(string id, OUT SkillDescriptionInfo& info);





private:
	unordered_map<string, SkillDescriptionInfo> totalSkillInfos;

};
