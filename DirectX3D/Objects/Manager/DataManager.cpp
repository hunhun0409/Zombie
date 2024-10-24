#include "Framework.h"

DataManager::DataManager()
{
    Init();
}

void DataManager::Init()
{
	LoadSkillDescription();
}

void DataManager::LoadSkillDescription()
{
    const wstring filePath = L"TextData/ZombieSurvival/SkillDescription.json";
    // ���� ���� ���� Ȯ��
    if (GetFileAttributes(filePath.c_str()) == INVALID_FILE_ATTRIBUTES) {
        // ������ ���� ��� ó��
        return;
    }
    ifstream loadFile(filePath);
    json data = json::parse(loadFile);
    for (const auto& item : data)
    {
        SkillDescriptionData skillData;
        skillData.id = item["id"];
        skillData.name = item["name"];
        skillData.descriptions[0] = item["level1"];
        skillData.descriptions[1] = item["level2"];
        skillData.descriptions[2] = item["level3"];
        skillData.descriptions[3] = item["level4"];
        skillData.descriptions[4] = item["level5"];
        totalSkillDatas[skillData.id] = skillData;
    }
}

bool DataManager::GetDescription(string id, OUT SkillDescriptionData& info)
{
    if (totalSkillDatas.count(id) == 0) return false;

    info = totalSkillDatas[id];
    return true;
}
