#include "Framework.h"

MagnetUpgrade::MagnetUpgrade()
	:PassiveSkill("magnet", "magnet", L"Textures/UI/MagnetIcon.png", 0, 10)
{
}


void MagnetUpgrade::Activate()
{
	PassiveSkill::Activate();
}

void MagnetUpgrade::Deactivate()
{
	PassiveSkill::Deactivate();
}

void MagnetUpgrade::LevelUp()
{
	PassiveSkill::LevelUp();

	effectValue = baseEffectValue + effectPerLevel * level;
	Character::StatusUpgradeInfo& status = owner->GetUpgradeStatus();
	status.upgradeMagnet = effectValue;
	owner->UpdateStatus();
}

void MagnetUpgrade::Update()
{
	PassiveSkill::Update();
}

void MagnetUpgrade::Render()
{
	PassiveSkill::Render();
}
