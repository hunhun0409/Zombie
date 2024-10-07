#pragma once

class WeaponScene : public Scene
{
public:
	WeaponScene();
	~WeaponScene();

	void Update() override;
	void PreRender() override;
	void Render() override;
	void PostRender() override;
	void GUIRender() override;

private:
	Weapon* sword;
	RangeWeapon* gun;

};
