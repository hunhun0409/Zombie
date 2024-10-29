#pragma once

struct QNode
{
    AABB bounds;
    int level;
    vector<Collider*> colliders;
    QNode* children[4] = {};
    QNode* parent = nullptr;
};

class QuadTree
{
private:
    const int MAX_LEVEL = 8;
    const int MAX_OBJECT = 8;
    const float MAX_HEIGHT = 10.0f;

    

public:
	QuadTree(Vector3 min, Vector3 max);
	QuadTree(class Terrain* terrain);
	~QuadTree();

    void Update();
    void Render();
    void GUIRender();

    void Insert(Collider* collider);
    void Remove(Collider* collider);//collider와 연결된 node만 제거
    void Delete(Collider* collider);//collider자체를 제거

    void Clear();

    vector<Collider*> GetPotentialColliders(Collider* collider);
    void QueryAreaRecursive(QNode* node, const AABB& area, vector<Collider*>& result);
    int GetTotalColliders(QNode* node);


private:
    //quadTree 기능
    void Split(QNode* node);
    void Merge(QNode* node);

    void UpdateCollider(Collider* collider); // collider를 업데이트
    void UpdateTree(QNode* node); // node의 Merge여부를 확인

    void RenderNode(QNode* node);
    void DrawAABB(const AABB& aabb, const Float4& color);

    void DeleteNode(QNode* node);

    QNode* FindInsertNode(QNode* node, Collider* collider);
    int FindChildQuadrantIndex(QNode* node, Collider* collider);

    bool IsColliderInNode(QNode* node, Collider* collider);
    bool IsColliderInTree(Collider* collider);

    bool AABBOverlap(const AABB& a, const AABB& b);
private:
    Material* material;
    Mesh<VertexColor>* mesh;

    UINT curWidth, curHeight;
    UINT fixWidth, fixHeight;

    MatrixBuffer* worldBuffer;

    QNode* root;
    unordered_map<Collider*, QNode*> colliderNodeMap;
};
