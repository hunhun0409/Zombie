#include "Framework.h"

QuadTree::QuadTree(Terrain* terrain)
{
    root = new Node();

    root->size = terrain->GetSize().x * terrain->Scale().x;
    root->x = root->size/2;
    root->z = root->size/2;
}

QuadTree::~QuadTree()
{
    DeleteNode(root);
    delete root;
}

void QuadTree::Update()
{
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
}

void QuadTree::Split(Node* node)
{
    //LT
    node->children[0] = new Node();
    node->children[0]->size = node->size / 2;
    node->children[0]->x = node->x - node->size/4;
    node->children[0]->x = node->z + node->size/4;
    //RT
    node->children[1] = new Node();
    node->children[1]->size = node->size / 2;
    node->children[1]->x = node->x + node->size / 4;
    node->children[1]->x = node->z + node->size / 4;
    //LB
    node->children[2] = new Node();
    node->children[2]->size = node->size / 2;
    node->children[2]->x = node->x - node->size / 4;
    node->children[2]->x = node->z - node->size / 4;
    //RB
    node->children[3] = new Node();
    node->children[3]->size = node->size / 2;
    node->children[3]->x = node->x + node->size / 4;
    node->children[3]->x = node->z - node->size / 4;
}

void QuadTree::Merge(Node* node)
{
}

vector<Collider*> QuadTree::GetCollidersInArea(Node* node)
{
    return vector<Collider*>();
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

    //이미 분열됨
    if (node->children[0] != nullptr)
    {
        int index = GetQuadrant(node, collider);
        if (index != -1)
        {
            InsertRecursive(node->children[index], collider, level + 1);
            return;
        }
    }

    node->colliders.push_back(collider);
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
