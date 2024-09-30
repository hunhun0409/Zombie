#include "Framework.h"

InstanceCharacter::InstanceCharacter(string name, Transform* transform, ModelAnimatorInstancing* instancing, UINT index)
	:name(name), transform(transform), instancing(instancing), index(index)
{
	transform->SetTag(name);
	transform->Load();

	collider = new CapsuleCollider(0.4f, 1.2f);
	collider->SetTag(name + "_Collider");
	collider->Load();
	collider->SetParent(transform);
	collider->Update();

	name += "_" + to_string(index);

	motion = instancing->GetMotion(index);

	curState = 0;
}

InstanceCharacter::~InstanceCharacter()
{
	delete collider;
}

void InstanceCharacter::Update()
{
	if (!transform->Active()) return;

	collider->Update();

	ExecuteEvent();
}

void InstanceCharacter::Render()
{
	if (!transform->Active()) return;

	collider->Render();
}

void InstanceCharacter::GUIRender()
{
	transform->GUIRender();
	collider->GUIRender();
}

void InstanceCharacter::Spawn(Vector3 pos)
{
	transform->SetActive(true);
	collider->SetActive(true);

	transform->Pos() = pos;

	SetState(0);
}

void InstanceCharacter::ApplySeperation(vector<InstanceCharacter*>& others)
{
	if (!transform->Active()) return;
	Vector3 separationForce;
	int neighborCount = 0;

	for (auto other : others)
	{
		if (this == other) continue;
		if (!other->transform->Active()) continue;

		Vector3 toOther = other->transform->GlobalPos() - transform->GlobalPos();
		float distance = toOther.Length();

		if (distance < separationRadius)
		{
			Vector3 repulsionForce = toOther.GetNormalized() * (separationRadius - distance);
			separationForce -= repulsionForce;
			neighborCount++;
		}
	}

	if (neighborCount > 0)
	{
		separationForce /= (float)neighborCount;
		separationForce = separationForce.GetNormalized() * separationStrength;

		transform->Pos() += separationForce * DELTA;
	}
}

void InstanceCharacter::SetState(UINT state, float playRate, float takeTime, bool canOverride)
{
	if (curState == state && !canOverride) return;

	curState = state;
	instancing->PlayClip(index, state, playRate, takeTime);
	eventIters[state] = totalEvents[state].begin();
}

void InstanceCharacter::SetEvent(int clip, Event event, float timeRatio)
{
	if (totalEvents[clip].count(timeRatio) > 0)
		return;

	totalEvents[clip][timeRatio] = event;
}

void InstanceCharacter::ExecuteEvent()
{
	int index = curState;

	if (totalEvents[index].empty()) return;
	if (eventIters[index] == totalEvents[index].end()) return;

	float ratio = motion->runningTime / motion->duration;

	if (eventIters[index]->first > ratio) return;

	eventIters[index]->second();
	eventIters[index]++;
}


void InstanceCharacter::SetPath()
{
	int startIndex = aStar->FindCloseNode(transform->GlobalPos());
	int endIndex = aStar->FindCloseNode(destPos);

	if (startIndex == -1 || endIndex == -1)
	{
		return;
	}

	aStar->GetPath(startIndex, endIndex, path);

	aStar->MakeDirectPath(transform->GlobalPos(), destPos, path);

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

void InstanceCharacter::TakeDamage(float damage)
{
	if (curHp <= 0) return;
	curHp -= damage;
	ParticleManager::Get()->Play("BloodExplode", transform->Pos() + Vector3(0, 6, 0));
}


