#include "Framework.h"

void DataManager::Init()
{
	LoadSkillDescription();
}

void DataManager::LoadSkillDescription()
{
    WIN32_FIND_DATA findData;

    HANDLE handle = FindFirstFile(L"TextData/SkillDescription/*.json", &findData);

    bool result = true;
    wstring fileName;

    while (result)
    {
        fileName = L"TextData/Dialog/";
        fileName += findData.cFileName;
        result = FindNextFile(handle, &findData);


        ifstream loadFile(fileName);
        json data = json::parse(loadFile);
        for (const auto& item : data)
        {
            SkillDescriptionInfo skillData;
            skillData.id = item["id"];
            skillData.name = item["name"];
            for (const auto& description : item["descriptions"])
            {
                skillData.descriptions.push_back(description);
            }
            
            totalSkillInfos[skillData.id] = skillData;
        }
    }
}

void DataManager::GetDescription(string id, OUT SkillDescriptionInfo& info)
{

}
