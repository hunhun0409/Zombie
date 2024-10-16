#include "Framework.h"
#include "CollisionScene.h"

CollisionScene::CollisionScene()
{
    //colliders.push_back(new BoxCollider());
    //colliders.back()->SetTag("Box1");
    //
    //colliders.push_back(new BoxCollider());
    //colliders.back()->SetTag("Box2");
        
    //
    //colliders.push_back(new SphereCollider());
    //colliders.back()->SetTag("Sphere2");

    qt = new QuadTree(Vector3(-50, 0, -50), Vector3(50, 50, 50));
    ColliderManager::Get()->SetQuadTree(qt);

    for (int i = 0; i < 100; i++)
    {
        colliders.push_back(new CapsuleCollider());
        colliders.back()->SetTag("Capsule" + to_string(i));
        colliders.back()->Pos().x = Random(-50.0f, 50.0f);
        colliders.back()->Pos().z = Random(-50.0f, 50.0f);
    }
    

    //colliders.push_back(new CapsuleCollider());
    //colliders.back()->SetTag("Box1");

    
}

CollisionScene::~CollisionScene()
{
    for (Collider* collider : colliders)
        delete collider;
    delete qt;
}

void CollisionScene::Update()
{
    //Ray ray = CAM->ScreenPointToRay(mousePos);
    //
    //Contact contact;
    //if (colliders[0]->IsRayCollision(ray, &contact))
    //{
    //    colliders[0]->SetColor(1, 0, 0);
    //    colliders[1]->Pos() = contact.hitPoint;
    //}
    //else
    //{
    //    colliders[0]->SetColor(0, 1, 0);
    //}

    if (KEY_DOWN(VK_F10))
    {
        b = !b;
        ColliderManager::Get()->SetQuadTreeMode(b);
    }

    if (KEY_DOWN('P'))
    {
        for (int i = 0; i < 100; i++)
        {
            ColliderManager::Get()->Add(colliders[i]);
        }
    }
    


    ColliderManager::Get()->Update();

    /*if (colliders[0]->IsCollision(colliders[1]))
    {
        colliders[0]->SetColor(1, 0, 0);
        colliders[1]->SetColor(1, 0, 0);
    }
    else
    {
        colliders[0]->SetColor(0, 1, 0);
        colliders[1]->SetColor(0, 1, 0);
    }*/

    for (Collider* collider : colliders)
    {
        if (collider->Pos().x <= 50)
        {
            collider->Pos().x += 2 * DELTA;
        }
        collider->Update();
    }
}

void CollisionScene::PreRender()
{
}

void CollisionScene::Render()
{
    //for (Collider* collider : colliders)
    //    collider->Render();
    ColliderManager::Get()->Render();
}

void CollisionScene::PostRender()
{
}

void CollisionScene::GUIRender()
{
    for (Collider* collider : colliders)
        collider->GUIRender();
}
