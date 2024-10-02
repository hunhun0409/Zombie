#pragma once

class RangeWeapon : public Weapon
{
public:
	RangeWeapon();
	~RangeWeapon();

	virtual void Update() override;
	virtual void Render() override;
	virtual void GUIRender() override;


private:

};
