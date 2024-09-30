#pragma once

class AStar
{
public:
	AStar(UINT width = 20, UINT height = 20);
	~AStar();

	void Update();
	void Render();

	void SetNode(class Terrain* terrain);

	int FindCloseNode(Vector3 pos);
	int FindRandomNode(Vector3 pos, float range);

	void GetPath(IN int start, IN int end, OUT vector<Vector3>& path);
	void MakeDirectPath(IN Vector3 start, IN Vector3 end, OUT vector<Vector3>& path);

	Vector3 GetNodePosition(UINT index);

	bool IsCollisionObstacle(Vector3 start, Vector3 end);

	void AddObstacle(Collider* collider);

private:
	void Reset();

	float GetDiagonalManhattanDistance(int start, int end);

	void Extend(int center, int end);
	int GetMinNode();

	void SetEdge();

private:
	UINT width, height;
	Vector2 interval;

	vector<Node*> nodes;
	Heap* heap;

	vector<Collider*> obstacles;
};
