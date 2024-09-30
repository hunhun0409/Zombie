#include "Framework.h"
#include "ParticleScene.h"

ParticleScene::ParticleScene()
{
	//particle = new Sprite(L"Textures/Effect/fire_8x2.png", 5, 20, 8, 2, true);
	//particle = new Sprite(L"Textures/Effect/explosion.png", 5, 5, 5, 3, false);
	//particle = new Spark(L"Textures/Effect/snow.png");
	//particle = new Spark(L"Textures/Effect/star.png", true);
	//particle = new Rain();
	//particle = new Snow();
	particle = new ParticleSystem("TextData/Particle/bloodBurst.fx");
	//particle->Play(Vector3());

	collider = new SphereCollider(50);
}

ParticleScene::~ParticleScene()
{
	delete particle;
	delete collider;
}

void ParticleScene::Update()
{
	if (KEY_DOWN(VK_LBUTTON))
	{
		Ray ray = CAM->ScreenPointToRay(mousePos);
		Contact contact;


		if (collider->IsRayCollision(ray, &contact))
		{
			Vector3 dir = contact.hitPoint - collider->GlobalPos();
			Vector3 rot;
			// y축 회전 (yaw) 계산
			rot.y = atan2(dir.x, dir.z);

			// x축 회전 (pitch) 계산
			rot.x = atan2(-dir.y, sqrt(dir.x * dir.x + dir.z * dir.z));
			
			particle->Play(contact.hitPoint, rot);
		}
	}
	
	

	particle->Update();
}

void ParticleScene::PreRender()
{
}

void ParticleScene::Render()
{
	particle->Render();
	collider->Render();
}

void ParticleScene::PostRender()
{
}

void ParticleScene::GUIRender()
{
	//particle->GUIRender();
}
