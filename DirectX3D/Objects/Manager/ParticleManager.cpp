#include "Framework.h"

ParticleManager::~ParticleManager()
{
	for (auto particles : totalParticles)
	{
		for (auto particle : particles.second)
		{
			delete particle;
		}
	}
}

void ParticleManager::Update()
{
	for (auto particles : totalParticles)
	{
		for (auto particle : particles.second)
		{
			particle->Update();
		}
	}
}

void ParticleManager::Render()
{
	for (auto particles : totalParticles)
	{
		for (auto particle : particles.second)
		{
			particle->Render();
		}
	}
}

void ParticleManager::Play(string key, Vector3 pos, Vector3 rot)
{
	if (totalParticles.count(key) == 0) return;

	for (auto particle : totalParticles[key])
	{
		if (!particle->Active())
		{
			particle->Play(pos, rot);
			return;
		}
	}
}

void ParticleManager::Add(string key, string file, UINT poolSize)
{
	if (totalParticles.count(key) != 0) return;

	Particles particles;
	particles.resize(poolSize);

	for (ParticleSystem*& particle : particles)
	{
		particle = new ParticleSystem(file);
	}
	totalParticles[key] = particles;
}

void ParticleManager::Remove(string key)
{
	if (totalParticles.count(key) == 0) return;

	for (ParticleSystem* particle : totalParticles[key])
	{
		delete particle;
	}
	totalParticles[key].clear();

}
