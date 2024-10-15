#pragma once

struct QNode
{
    AABB bounds;
    //float x, z, size;
    int level;
    vector<Collider*> colliders;
    QNode* children[4] = {};
    QNode* parent = nullptr;
};

class QuadTree
{
private:
    const int MAX_LEVEL = 5;
    const int MAX_OBJECT = 10;
    const float MAX_HEIGHT = 10.0f;

    

public:
	QuadTree(class Terrain* terrain);
	~QuadTree();

    void Update();
    void Render();

    void Insert(Collider* collider);
    void Remove(Collider* collider);

    vector<Collider*> GetPotentialColliders(Collider* collider);
    void QueryAreaRecursive(QNode* node, const AABB& area, vector<Collider*>& result);
    int GetTotalColliders(QNode* node);


private:
    void Split(QNode* node);
    void Merge(QNode* node);

    void UpdateCollider(Collider* collider); // collider를 업데이트
    void UpdateTree(QNode* node); // node의 Merge여부를 확인

    void DeleteNode(QNode* node);

    QNode* FindInsertNode(QNode* node, Collider* collider);
    //void InsertRecursive(QNode* node, Collider* collider, int level);
    //bool RemoveRecursive(Node* node, Collider* collider);
    int GetQuadrant(QNode* node, Collider* collider);

    bool IsColliderInNode(QNode* node, Collider* collider);

    bool AABBOverlap(const AABB& a, const AABB& b);
private:
    QNode* root;
    unordered_map<Collider*, QNode*> colliderNodeMap;

};
