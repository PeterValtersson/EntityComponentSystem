#include "SceneManager.h"
#include <Profiler.h>

ECS::SceneManager::SceneManager(SceneManagerInitializationInfo ii) : initInfo(ii)
{
	_ASSERT_EXPR(initInfo.entityManager, "SceneManager must have entitymanager");
}

ECS::SceneManager::~SceneManager()
{
}

void ECS::SceneManager::Create(Entity entity)noexcept
{
	StartProfile;
	if (auto find = entityToEntry.find(entity); find != entityToEntry.end())
		return;

	if (!initInfo.entityManager->IsAlive(entity))
		return;



}

void ECS::SceneManager::AddEntityToScene(Entity scene, Entity entity)noexcept
{
}

void ECS::SceneManager::AddEntitiesToScene(Entity scene, const Entity entities[], uint32_t numEntities)noexcept
{
}

uint32_t ECS::SceneManager::GetNumberOfEntitiesInScene(Entity scene)const noexcept
{
	return uint32_t();
}

void ECS::SceneManager::GetEntitiesInScene(Entity scene, Entity entities[])const noexcept
{
}

void ECS::SceneManager::Destroy(Entity entity)noexcept
{
}

void ECS::SceneManager::DestroyEntities(const Entity entities[], uint32_t numEntities)noexcept
{
}

uint32_t ECS::SceneManager::GetNumberOfRegisteredEntities() const noexcept
{
	return uint32_t();
}

void ECS::SceneManager::GetRegisteredEntities(Entity entities[], uint32_t numEntities) const noexcept
{
}

void ECS::SceneManager::Frame()noexcept
{
}

uint64_t ECS::SceneManager::GetMemoryUsage() const noexcept
{
	return uint64_t();
}

void ECS::SceneManager::ShrinkToFit() noexcept
{
}



void ECS::SceneManager::WriteToFile(std::ofstream & file) const
{
}

void ECS::SceneManager::CreateFromFile(std::ifstream & file)
{
}

void ECS::SceneManager::GarbageCollection()noexcept
{
}
