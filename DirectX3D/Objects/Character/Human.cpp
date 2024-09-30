#include "Framework.h"

Human::Human() : ModelAnimator("Mannequin")
{
	weapon = new MeleeWeapon("sword");

	ReadClip("Idle", 1);
	ReadClip("Run", 1);
	ReadClip("Slash", 1);

	rightHand = new Transform();

	//trail = new Trail(L"textures/Effect/Trail.png", weapon->GetStartPos(), weapon->GetEndPos(), 20, 50.0f);
	//trail->GetMaterial()->GetData().diffuse = { 0.3f,0.3f,0.3f,1.0f };

	weapon->SetParent(rightHand);
	weapon->Load();
	
	PlayClip(2);
}

Human::~Human()
{
	delete weapon;
	delete rightHand;

	delete trail;
}

void Human::Update()
{
	if (KEY_DOWN('1'))
		PlayClip(0);
	if (KEY_DOWN('2'))
		PlayClip(1);
	if (KEY_DOWN('3'))
		PlayClip(2);


	ModelAnimator::Update();

	rightHand->SetWorld(GetTransformByNode(11));

	weapon->Update();
	trail->Update();
}

void Human::Render()
{
	ModelAnimator::Render();

	weapon->Render();
	trail->Render();
}

void Human::GUIRender()
{
	ModelAnimator::GUIRender();

	weapon->GUIRender();
}
