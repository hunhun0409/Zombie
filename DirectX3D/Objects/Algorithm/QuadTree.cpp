#include "Framework.h"

QuadTree::QuadTree(Vector3 min, Vector3 max)
{
    root = new QNode();

    root->bounds.minPos = min;
    root->bounds.maxPos = max;
    root->level = 0;

    material = new Material(L"Basic/Grid.hlsl");
    mesh = new Mesh<VertexColor>();
    worldBuffer = new MatrixBuffer();
}

QuadTree::QuadTree(Terrain* terrain)
{
    root = new QNode();

    root->bounds.minPos = Vector3(0, 0, 0);
    root->bounds.maxPos = Vector3(terrain->GetSize().x, MAX_HEIGHT, terrain->GetSize().y);
    root->level = 0;

    material = new Material(L"Basic/Grid.hlsl");
    mesh = new Mesh<VertexColor>();
    worldBuffer = new MatrixBuffer();
}

QuadTree::~QuadTree()
{
    DeleteNode(root);
    colliderNodeMap.clear();
    delete root;
}

void QuadTree::Update()
{
    for (auto it = colliderNodeMap.begin(); it != colliderNodeMap.end();)
    {
        Collider* collider = it->first;
        if (!IsColliderInTree(collider))
        {
            Remove(collider);
            it = colliderNodeMap.erase(it);
        }
        else
        {
            UpdateCollider(collider);
            ++it;
        }
    }
    UpdateTree(root);
}

void QuadTree::Render()
{
    for (auto collider : colliderNodeMap)
    {
        collider.first->Render();
    }

    worldBuffer->SetVS(0);
    material->Set();

    mesh->Vertices().clear();
    RenderNode(root);
    mesh->CreateMesh();

    mesh->Draw(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
}

void QuadTree::GUIRender()
{
    int count = colliderNodeMap.size();

    ImGui::Text("QuadTree ColliderCount : %d", count);
    
}


void QuadTree::UpdateCollider(Collider* collider)
{
    auto it = colliderNodeMap.find(collider);
    if (it == colliderNodeMap.end()) return;

    QNode* currentNode = it->second;
    if (!IsColliderInNode(currentNode, collider))
    {
        Remove(collider);
        Insert(collider);
    }
}

void QuadTree::UpdateTree(QNode* node)
{
    if (!node) return;

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

void QuadTree::RenderNode(QNode* node)
{
    if (!node) return;

    if (node->children[0] == nullptr)
    {
        DrawAABB(node->bounds, Float4(0, 1, 0, 1));
    }
    else
    {
        for (int i = 0; i < 4; i++)
        {
            RenderNode(node->children[i]);
        }
    }
}

void QuadTree::DrawAABB(const AABB& aabb, const Float4& color)
{
    vector<VertexColor>& vertices = mesh->Vertices();

    vertices.emplace_back(aabb.minPos.x, 2, aabb.minPos.z, 1, 0, 0);
    vertices.emplace_back(aabb.maxPos.x, 2, aabb.minPos.z, 1, 0, 0);

    vertices.emplace_back(aabb.maxPos.x, 2, aabb.minPos.z, 1, 0, 0);
    vertices.emplace_back(aabb.maxPos.x, 2, aabb.maxPos.z, 1, 0, 0);

    vertices.emplace_back(aabb.maxPos.x, 2, aabb.maxPos.z, 1, 0, 0);
    vertices.emplace_back(aabb.minPos.x, 2, aabb.maxPos.z, 1, 0, 0);

    vertices.emplace_back(aabb.minPos.x, 2, aabb.maxPos.z, 1, 0, 0);
    vertices.emplace_back(aabb.minPos.x, 2, aabb.minPos.z, 1, 0, 0);
}


void QuadTree::Insert(Collider* collider)
{
    QNode* node = FindInsertNode(root, collider);
    if (node)
    {
        node->colliders.push_back(collider);
        colliderNodeMap[collider] = node;

        if (node->colliders.size() > MAX_OBJECT && node->level < MAX_LEVEL - 1)
        {
            Split(node);
        }
    }
}

void QuadTree::Remove(Collider* collider)
{
    auto it = colliderNodeMap.find(collider);
    if (it != colliderNodeMap.end())
    {
        QNode* node = it->second;
        node->colliders.erase(remove(node->colliders.begin(), node->colliders.end(), collider), node->colliders.end());
    }
}

void QuadTree::Delete(Collider* collider)
{
    auto it = colliderNodeMap.find(collider);
    if (it != colliderNodeMap.end())
    {
        QNode* node = it->second;
        node->colliders.erase(remove(node->colliders.begin(), node->colliders.end(), collider), node->colliders.end());
        colliderNodeMap.erase(collider);
    }
}

void QuadTree::Clear()
{
    DeleteNode(root);
    colliderNodeMap.clear();
}


void QuadTree::Split(QNode* node)
{

    if (node->level >= MAX_LEVEL - 1) return; // 최대 레벨에 도달하면 분할 중지

    float midX = (node->bounds.minPos.x + node->bounds.maxPos.x) * 0.5f;
    float midZ = (node->bounds.minPos.z + node->bounds.maxPos.z) * 0.5f;

    // 자식 노드 생성
    for (int i = 0; i < 4; ++i)
    {
        node->children[i] = new QNode();
        node->children[i]->level = node->level + 1;
        node->children[i]->parent = node;
    }

    // 자식 노드의 경계 설정
    node->children[0]->bounds = AABB{ {node->bounds.minPos.x, 0.0f, midZ}, {midX, MAX_HEIGHT, node->bounds.maxPos.z} }; // LT
    node->children[1]->bounds = AABB{ {midX, 0.0f, midZ}, {node->bounds.maxPos.x, MAX_HEIGHT, node->bounds.maxPos.z} }; // RT
    node->children[2]->bounds = AABB{ {node->bounds.minPos.x, 0.0f, node->bounds.minPos.z}, {midX, MAX_HEIGHT, midZ} }; // LB
    node->children[3]->bounds = AABB{ {midX, 0.0f, node->bounds.minPos.z}, {node->bounds.maxPos.x, MAX_HEIGHT, midZ} }; // RB

    // Collider 재분배
    vector<Collider*> tempColliders = std::move(node->colliders);
    for (Collider* collider : tempColliders)
    {
        int index = FindChildQuadrantIndex(node, collider);
        if (index >= 0 && index < 4)
        {
            node->children[index]->colliders.push_back(collider);
            colliderNodeMap[collider] = node->children[index];
        }
        else
        {
            // 어떤 자식 노드에도 속하지 않는 경우, 현재 노드에 유지
            node->colliders.push_back(collider);
            colliderNodeMap[collider] = node;
        }
    }
}

void QuadTree::Merge(QNode* node)
{
    if (!node || node->children[0] == nullptr) return;

    // 모든 자식 노드의 콜라이더를 현재 노드로 이동
    for (int i = 0; i < 4; ++i)
    {
        for (Collider* collider : node->children[i]->colliders)
        {
            node->colliders.push_back(collider);
            colliderNodeMap[collider] = node;
        }
    }

    // 자식 노드들을 삭제하고 메모리 해제
    for (int i = 0; i < 4; ++i)
    {
        DeleteNode(node->children[i]);
        delete node->children[i];
        node->children[i] = nullptr;
    }
}

vector<Collider*> QuadTree::GetPotentialColliders(Collider* collider)
{
    vector<Collider*> potentialColliders;

    AABB colliderAABB = collider->GetAABB();

    Vector3 expansion = (colliderAABB.maxPos - colliderAABB.minPos) * 0.5f;
    AABB expandedAABB;
    expandedAABB.minPos = colliderAABB.minPos - expansion;
    expandedAABB.maxPos = colliderAABB.maxPos + expansion;

    QueryAreaRecursive(root, expandedAABB, potentialColliders);

    potentialColliders.erase(
        remove(potentialColliders.begin(), potentialColliders.end(), collider),
        potentialColliders.end()
    );
    return potentialColliders;
}

void QuadTree::QueryAreaRecursive(QNode* node, const AABB& area, vector<Collider*>& result)
{
    if (!node || !AABBOverlap(node->bounds, area)) return;

    for (Collider* collider : node->colliders)
    {
        if (AABBOverlap(collider->GetAABB(), area))
        {
            result.push_back(collider);
        }
    }

    for (int i = 0; i < 4; ++i)
    {
        if (node->children[i])
        {
            QueryAreaRecursive(node->children[i], area, result);
        }
    }
}


int QuadTree::GetTotalColliders(QNode* node)
{
    int total = 0;

    for (Collider* collider : node->colliders)
    {
        ++total;
    }

    for (int i = 0; i < 4; ++i) {
        if (node->children[i]) {
            total += GetTotalColliders(node->children[i]);
        }
    }
    return total;
}

void QuadTree::DeleteNode(QNode* node)
{
    if (!node) return;

    for (int i = 0; i < 4; ++i)
    {
        if (node->children[i])
        {
            DeleteNode(node->children[i]);
            delete node->children[i];
            node->children[i] = nullptr;
        }
    }

    node->colliders.clear();
}

QNode* QuadTree::FindInsertNode(QNode* node, Collider* collider)
{
    if (!node || !IsColliderInNode(node, collider)) return nullptr;

    

    if (node->children[0] == nullptr || node->level >= MAX_LEVEL - 1)
    {
        return node;
    }

    int quadrant = FindChildQuadrantIndex(node, collider);
    QNode* childNode = FindInsertNode(node->children[quadrant], collider);
    return childNode ? childNode : node;
}

int QuadTree::FindChildQuadrantIndex(QNode* node, Collider* collider)
{
    AABB colliderAABB = collider->GetAABB();

    for (int i = 0; i < 4; ++i)
    {
        if (AABBOverlap(node->children[i]->bounds, colliderAABB))
        {
            return i;
        }
    }

    return -1;
}

bool QuadTree::IsColliderInNode(QNode* node, Collider* collider)
{
    AABB colliderAABB = collider->GetAABB();

    // 노드의 AABB와 충돌체의 AABB 간의 겹침 검사
    return AABBOverlap(node->bounds, colliderAABB);
}

bool QuadTree::IsColliderInTree(Collider* collider)
{
    AABB colliderAABB = collider->GetAABB();
    return AABBOverlap(root->bounds, colliderAABB);
}

bool QuadTree::AABBOverlap(const AABB& a, const AABB& b)
{
    return (a.minPos.x <= b.maxPos.x && a.maxPos.x >= b.minPos.x) &&
        (a.minPos.z <= b.maxPos.z && a.maxPos.z >= b.minPos.z);
}


