#include "Framework.h"

QuadTree::QuadTree(Terrain* terrain)
{
    root = new Node();

    root->size = terrain->GetSize().x * terrain->Scale().x;
    root->x = root->size/2;
    root->z = root->size/2;
    root->level = 0;
}

QuadTree::~QuadTree()
{
    DeleteNode(root);
    delete root;
}

void QuadTree::Update()
{
    UpdateTree(root);
}

void QuadTree::UpdateTree(Node* node)
{
    if (!node) return;

    for (auto it = node->colliders.begin(); it != node->colliders.end();)
    {
        Collider* collider = *it;
        if (!IsColliderInNode(node, collider))
        {
            it = node->colliders.erase(it);
            Insert(collider);
        }
        else
        {
            it++;
        }
    }
    FOR(4)
    {
        if (node->children[i])
        {
            UpdateTree(node->children[i]);
        }
    }

    if (node->level > 0 && GetTotalColliders(node) <= MAX_OBJECT)
    {
        Merge(node);
    }

}

void QuadTree::Render()
{
}

void QuadTree::Insert(Collider* collider)
{
    InsertRecursive(root, collider, 0);
}

void QuadTree::Remove(Collider* collider)
{
    RemoveRecursive(root, collider);
}

void QuadTree::Split(Node* node)
{
    //LT
    node->children[0] = new Node();
    node->children[0]->size = node->size / 2;
    node->children[0]->x = node->x - node->size/4;
    node->children[0]->z = node->z + node->size/4;
    node->children[0]->level = node->level + 1;
    node->children[0]->parent = node;

    //RT
    node->children[1] = new Node();
    node->children[1]->size = node->size / 2;
    node->children[1]->x = node->x + node->size / 4;
    node->children[1]->z = node->z + node->size / 4;
    node->children[1]->level = node->level + 1;
    node->children[1]->parent = node;

    //LB
    node->children[2] = new Node();
    node->children[2]->size = node->size / 2;
    node->children[2]->x = node->x - node->size / 4;
    node->children[2]->z = node->z - node->size / 4;
    node->children[2]->level = node->level + 1;
    node->children[2]->parent = node;

    //RB
    node->children[3] = new Node();
    node->children[3]->size = node->size / 2;
    node->children[3]->x = node->x + node->size / 4;
    node->children[3]->z = node->z - node->size / 4;
    node->children[3]->level = node->level + 1;
    node->children[3]->parent = node;

    //추가 : 분열되면 가지고 있는 collider 다 자식에게 넘김
    for (Collider* collider : node->colliders)
    {
        int index = GetQuadrant(node, collider);
        InsertRecursive(node->children[index], collider, node->level + 1);
    }
}

void QuadTree::Merge(Node* node)
{
    if (node->level >= MAX_LEVEL)
    {
        for (Collider* collider : node->colliders)
        {
            InsertRecursive(node->parent, collider, node->level - 1);
        }
        return;
    }
    if (node->children[0] == nullptr)
    {
        for (Collider* collider : node->colliders)
        {
            InsertRecursive(node->parent, collider, node->level - 1);
        }
        return;
    }

    FOR(4)
    {
        if (node->children[i])
        {
            Merge(node->children[i]);
        }
    }
}


int QuadTree::GetTotalColliders(Node* node)
{
    int total = node->colliders.size();
    for (int i = 0; i < 4; ++i) {
        if (node->children[i]) {
            total += GetTotalColliders(node->children[i]);
        }
    }
    return total;
}

void QuadTree::DeleteNode(Node* node)
{
    FOR(4)
    {
        if (node->children[i])
        {
            DeleteNode(node->children[i]);
            delete node->children[i];
        }
    }
}

void QuadTree::InsertRecursive(Node* node, Collider* collider, int level)
{
    if (level >= MAX_LEVEL)
    {
        node->colliders.push_back(collider);
        return;
    }

    //분열전 && 수량 초과
    if (node->children[0] == nullptr && node->colliders.size() >= MAX_OBJECT)
    {
        Split(node);
    }

    //분열됨
    if (node->children[0] != nullptr)
    {
        int index = GetQuadrant(node, collider);
        if (index != -1)
        {
            InsertRecursive(node->children[index], collider, level + 1);
            return;
        }
    }
    else
    {
        node->colliders.push_back(collider);
    }
}

bool QuadTree::RemoveRecursive(Node* node, Collider* collider)
{
    if (!node)return false;

    //이 노드에서 찾아본다
    for (auto it = node->colliders.begin(); it != node->colliders.end(); it++)
    {
        if (*it == collider)
        {
            node->colliders.erase(it);
            return true;
        }
    }

    //이 노드에 없네? 자식 노드에 가서 찾는다
    if (node->children[0])
    {
        int index = GetQuadrant(node, collider);
        if (RemoveRecursive(node->children[index], collider))
        {
            return true;
        }
    }

    return false;
}

int QuadTree::GetQuadrant(Node* node, Collider* collider)
{
    Vector3 center = collider->GlobalPos();
   
    if (center.x < node->x)
    {
        if (center.z < node->z) return 0; // 좌상단
        else return 2; // 좌하단
    }
    else
    {
        if (center.z < node->z) return 1; // 우상단
        else return 3; // 우하단
    }
}

bool QuadTree::IsColliderInNode(Node* node, Collider* collider)
{
    if (collider->GlobalPos().x > node->x + node->size * 0.5f || collider->GlobalPos().x < node->x - node->size * 0.5f
        || collider->GlobalPos().z > node->z + node->size * 0.5f || collider->GlobalPos().z < node->z - node->size * 0.5f)
    {
        return false;
    }
    return true;
}
