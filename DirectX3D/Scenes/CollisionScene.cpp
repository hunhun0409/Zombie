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

    for (int i = 0; i < 1000; i++)
    {
        colliders.push_back(new CapsuleCollider());
        colliders.back()->SetTag("Capsule" + to_string(i));
        colliders.back()->Pos().x = Random(-50.0f, 50.0f);
        colliders.back()->Pos().z = Random(-50.0f, 50.0f);
        colliders.back()->UpdateWorld();

        dirs.push_back(Direction::LEFT);
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

    for (int i=0; i<colliders.size(); i++)
    {
        if (!colliders[i]->Active()) continue;

        if (colliders[i]->Pos().x >= 49.0f && colliders[i]->Pos().x <= 50.0f)
        {
            dirs[i] = Direction::LEFT;
        }
        else if (colliders[i]->Pos().x <= -49.0f && colliders[i]->Pos().x >= -50.0f)
        {
            dirs[i] = Direction::RIGHT;
        }

        if (dirs[i] == Direction::LEFT)
        {
            colliders[i]->Pos().x -= 30 * DELTA;
        }
        else
        {
            colliders[i]->Pos().x += 30 * DELTA;
        }
        colliders[i]->UpdateWorld();
    }

    

    if (KEY_DOWN(VK_TAB))
    {
        b = !b;
        ColliderManager::Get()->SetQuadTreeMode(b);
    }

    if (index < 100)
    {
        interval += DELTA;
        if (interval >= rate)
        {

            interval -= rate;
            ColliderManager::Get()->Add(colliders[index++]);
            /*for (int i = 0; i < 500; i++)
            {
                ColliderManager::Get()->Add(colliders[i]);
            }*/
        }
    }
    
    if (KEY_DOWN('K'))
    {
        for (int i = 0; i < 1000; i++)
        {
            colliders[i]->SetActive(false);
        }
    }
    if (KEY_DOWN('L'))
    {
        for (int i = 0; i < 1000; i++)
        {
            colliders[i]->SetActive(true);
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

    /*for (Collider* collider : colliders)
    {
        if (collider->Pos().x <= 50)
        {
            collider->Pos().x += 2 * DELTA;
        }
        collider->Update();
    }*/
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
    /*for (Collider* collider : colliders)
        collider->GUIRender();*/

    ColliderManager::Get()->GUIRender();
}
