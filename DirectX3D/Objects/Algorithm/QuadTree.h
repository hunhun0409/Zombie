#pragma once

class QuadTree
{
private:
    const float MAX_LEVEL = 5;
    const float MAX_OBJECT = 10;

    struct Node
    {
        float x, z, size;
        int level;
        vector<Collider*> colliders;
        Node* children[4] = {};
        Node* parent = nullptr;
    };

    struct AABB
    {
        Vector3 minPos;
        Vector3 maxPos;
    };

public:
	QuadTree(class Terrain* terrain);
	~QuadTree();

    void Update();
    void UpdateTree(Node* node);
    void Render();

    void Insert(Collider* collider);
    void Remove(Collider* collider);

    void Split(Node* node);
    void Merge(Node* node);

    int GetTotalColliders(Node* node);

private:
    void DeleteNode(Node* node);

    void InsertRecursive(Node* node, Collider* collider, int level);
    bool RemoveRecursive(Node* node, Collider* collider);
    int GetQuadrant(Node* node, Collider* collider);

    bool IsColliderInNode(Node* node, Collider* collider);
private:
    Node* root;
};
