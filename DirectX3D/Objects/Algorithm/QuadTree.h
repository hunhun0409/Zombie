#pragma once

class QuadTree
{
private:
    const float MAX_LEVEL = 5;
    const float MAX_OBJECT = 10;

    struct Node
    {
        float x, z, size;
        vector<Collider*> colliders;
        Node* children[4] = {};
    };

public:
	QuadTree(class Terrain* terrain);
	~QuadTree();

    void Update();
    void Render();

    void Insert(Collider* collider);
    void Remove(Collider* collider);

    void Split(Node* node);
    void Merge(Node* node);

    vector<Collider*> GetCollidersInArea(Node* node);

private:
    void DeleteNode(Node* node);

    void InsertRecursive(Node* node, Collider* collider, int level);
    int GetQuadrant(Node* node, Collider* collider);
private:
    Node* root;
};
