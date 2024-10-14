#include "Framework.h"

QuadTree::QuadTree(Terrain* terrain)
{
    root = new Node();

    root->bounds.minPos = Vector3(0, 0, 0);
    root->bounds.maxPos = Vector3(terrain->GetSize().x, MAX_HEIGHT, terrain->GetSize().y);
    //root->size = terrain->GetSize().x * terrain->Scale().x;
    //root->x = root->size/2;
    //root->z = root->size/2;
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
    float midX = (node->bounds.minPos.x + node->bounds.maxPos.x) * 0.5f;
    float midZ = (node->bounds.minPos.z + node->bounds.maxPos.z) * 0.5f;

    // LT (Left Top)
    node->children[0] = new Node();
    node->children[0]->bounds = {
        {node->bounds.minPos.x, 0.0f, midZ},
        {midX, MAX_HEIGHT, node->bounds.maxPos.z}
    };

    // RT (Right Top)
    node->children[1] = new Node();
    node->children[1]->bounds = {
        {midX, 0.0f, midZ},
        {node->bounds.maxPos.x, MAX_HEIGHT, node->bounds.maxPos.z}
    };

    // LB (Left Bottom)
    node->children[2] = new Node();
    node->children[2]->bounds = {
        {node->bounds.minPos.x, 0.0f, node->bounds.minPos.z},
        {midX, MAX_HEIGHT, midZ}
    };

    // RB (Right Bottom)
    node->children[3] = new Node();
    node->children[3]->bounds = {
        {midX, 0.0f, node->bounds.minPos.z},
        {node->bounds.maxPos.x, MAX_HEIGHT, midZ}
    };

    // Common properties for all children
    for (int i = 0; i < 4; ++i)
    {
        node->children[i]->level = node->level + 1;
        node->children[i]->parent = node;
    }

    // Redistribute colliders to children
    for (Collider* collider : node->colliders)
    {
        int index = GetQuadrant(node, collider);
        InsertRecursive(node->children[index], collider, node->level + 1);
    }
    node->colliders.clear();  // Clear parent's colliders after redistribution
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
    Vector3 nodeCenter = (node->bounds.minPos + node->bounds.maxPos) * 0.5f;
   
    if (center.x < nodeCenter.x)
    {
        if (center.z < nodeCenter.z) return 0; // 좌상단
        else return 2; // 좌하단
    }
    else
    {
        if (center.z < nodeCenter.z) return 1; // 우상단
        else return 3; // 우하단
    }
}

bool QuadTree::IsColliderInNode(Node* node, Collider* collider)
{
    AABB colliderAABB = collider->GetAABB();

    // 노드의 AABB와 충돌체의 AABB 간의 겹침 검사
    return AABBOverlap(node->bounds, colliderAABB);
}

bool QuadTree::AABBOverlap(const AABB& a, const AABB& b)
{
    return (a.minPos.x <= b.maxPos.x && a.maxPos.x >= b.minPos.x) &&
        (a.minPos.z <= b.maxPos.z && a.maxPos.z >= b.minPos.z);
}


