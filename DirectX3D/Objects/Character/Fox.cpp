#include "Framework.h"

Fox::Fox()
	:Character("Fox")
{
	model->ReadClip("Idle", true);
	model->ReadClip("Run", true, 0, "FoxTransform");

	model->PlayClip(IDLE);
}

Fox::~Fox()
{
}

void Fox::Update()
{
	Control();
	Move();
	Rotate();

	Character::Update();
}

void Fox::Render()
{
	Character::Render();
}

void Fox::Control()
{
	if (KEY_DOWN(VK_LBUTTON))
	{
		destPos = terrain->Picking();
		destPos = aStar->GetNodePosition(aStar->FindCloseNode(destPos));

		if (aStar->IsCollisionObstacle(GlobalPos(), destPos))
		{
			SetPath();
		}
		else
		{
			path.clear();
			path.push_back(destPos);
		}
		
	}

	//controller->Update();
}

void Fox::Move()
{
	if (path.empty())
	{
		SetState(IDLE);
		return;
	}
	SetState(RUN);

	Vector3 dest = path.back();

	Vector3 direction = dest - GlobalPos();
	direction.y = 0.0f;

	if (direction.Length() < 1.0f)
	{
		path.pop_back();
	}

	velocity = direction.GetNormalized();

	Pos() += velocity * moveSpeed * DELTA;

	//characterMovement->Move();
}

void Fox::Rotate()
{
	if (curState == IDLE) return;

	Vector3 forward = Forward();
	Vector3 cross = Cross(forward, velocity);

	if (cross.y < 0)
	{
		Rot().y += rotSpeed * DELTA;
	}
	else if (cross.y > 0)
	{
		Rot().y -= rotSpeed * DELTA;
	}
}

void Fox::SetState(State state)
{
	if (curState == state) return;

	curState = state;
	model->PlayClip(state);
}

void Fox::SetPath()
{
	int startIndex = aStar->FindCloseNode(GlobalPos());
	int endIndex = aStar->FindCloseNode(destPos);

	if (startIndex == -1 || endIndex == -1)
	{
		return;
	}

	aStar->GetPath(startIndex, endIndex, path);

	aStar->MakeDirectPath(GlobalPos(), destPos, path);

	path.insert(path.begin(), destPos);

	UINT pathSize = path.size();

	while (path.size() > 2)
	{
		vector<Vector3> tempPath = path;
		tempPath.erase(tempPath.begin());
		tempPath.pop_back();

		Vector3 start = path.back();
		Vector3 end = path.front();

		aStar->MakeDirectPath(start, end, tempPath);
		path.clear();
		path = tempPath;
		path.insert(path.begin(), end);
		path.push_back(start);

		if (pathSize == path.size())
		{
			break;
		}
		else
		{
			pathSize = path.size();
		}
	}
}
