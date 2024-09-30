#include "Framework.h"
#include "ModelAnimationScene.h"

ModelAnimationScene::ModelAnimationScene()
{
    //modelAnimator = new ModelAnimator("Knight");
    ////modelAnimator->GetMesh(0)->SetMaterial(modelAnimator->AddMaterial("base1"));
    ////modelAnimator->GetMesh(1)->SetMaterial(modelAnimator->AddMaterial("base2"));
    ////modelAnimator->GetMesh(2)->SetMaterial(modelAnimator->AddMaterial("detail"));
    ////modelAnimator->ReadClip("Walk Forward", true);
    ////modelAnimator->ReadClip("Run", true);
    //modelAnimator->ReadClip("Idle",true);
    //modelAnimator->ReadClip("Block", false, 0, "RootNode");
    //modelAnimator->ReadClip("Block Idle", false, 0, "Paladin_J_Nordstrom");
    ////modelAnimator->GetClip(2)->SetEvent(bind(&ModelAnimationScene::SetIdle, this), 1.0f);

    modelAnimator = new ModelAnimator("ZombieWoman");
    modelAnimator->ReadClip("BiteStart", true, 0, "mixamorig:Hips");
    modelAnimator->ReadClip("BiteLoop", true, 0, "mixamorig:Hips");
    modelAnimator->ReadClip("BiteEnd", true, 0, "mixamorig:Hips");
}

ModelAnimationScene::~ModelAnimationScene()
{
    delete modelAnimator;
}

void ModelAnimationScene::Update()
{
    if (KEY_DOWN('1'))
        modelAnimator->PlayClip(0);
    if (KEY_DOWN('2'))
        modelAnimator->PlayClip(1, 1.0f, 0.1f);
    if (KEY_DOWN('3'))
        modelAnimator->PlayClip(2, 1.0f, 0.1f);
    if (KEY_DOWN('4'))
        modelAnimator->PlayClip(3);
    if (KEY_DOWN('5'))
        modelAnimator->PlayClip(4, 1.0f, 0.0f);
    if (KEY_DOWN('6'))
        modelAnimator->PlayClip(5);
    if (KEY_DOWN('7'))
        modelAnimator->PlayClip(6);
    if (KEY_DOWN('8'))
        modelAnimator->PlayClip(7);
    if (KEY_DOWN('9'))
        modelAnimator->PlayClip(8);
    if (KEY_DOWN('0'))
        modelAnimator->PlayClip(9);
    modelAnimator->Update();
}

void ModelAnimationScene::PreRender()
{
}

void ModelAnimationScene::Render()
{
    modelAnimator->Render();
}

void ModelAnimationScene::PostRender()
{
}

void ModelAnimationScene::GUIRender()
{
    modelAnimator->GUIRender();
}

void ModelAnimationScene::SetDraw2()
{
    modelAnimator->PlayClip(2, 1.0f, 0.1f);
}

void ModelAnimationScene::SetSheath2()
{
    modelAnimator->PlayClip(5, 1.0f, 0.1f);

}

void ModelAnimationScene::SetEndSheath()
{
    modelAnimator->PlayClip(0, 1.0f, 0.1f);

}

void ModelAnimationScene::SetIdle()
{
    modelAnimator->PlayClip(3, 1.0f, 0.2f);

}
