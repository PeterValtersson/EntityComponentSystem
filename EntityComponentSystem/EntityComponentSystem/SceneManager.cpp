#include "SceneManager.h"
#include <Profiler.h>

ECS::SceneManager::SceneManager(SceneManagerInitializationInfo ii) : initInfo(ii)
{
	_ASSERT_EXPR(initInfo.entityManager, "SceneManager must have entitymanager");
}

ECS::SceneManager::~SceneManager()
{
}



void ECS::SceneManager::RegisterManager(Manager_Base * manager)noexcept
{
}

void ECS::SceneManager::CreateFromResource(Entity entity, ResourceHandler::Resource resource)noexcept
{
}

void ECS::SceneManager::CreateFromStream(Entity entity, std::istream * stream)noexcept
{
}

std::function<bool(std::ostream* file)> ECS::SceneManager::GetDataWriter(Entity entity)const noexcept
{
	auto findf = entityToEntry.find(entity);
	_ASSERT_EXPR(findf != entityToEntry.end(), "Can not get DataWriter if entity is not registered");

	return [entity, this](std::ostream* file) {
		
		if (auto find = entityToEntry.find(entity); find == entityToEntry.end())
			return false;
		else
		{
			auto& ents = entries.entitiesInScene[find->second];

			file->write((char*)&version, sizeof(version));
			uint32_t numEnt = ents.size();

			for (auto& manager : managers)
			{

			}



			return true;
		}};
}



void ECS::SceneManager::Create(Entity entity)noexcept
{
	StartProfile;
	if (auto find = entityToEntry.find(entity); find != entityToEntry.end())
		return;

	if (!initInfo.entityManager->IsAlive(entity))
		return;

	uint32_t index = static_cast<uint32_t>(entries.entity.size());
	entries.entity.push_back(entity);
	entries.entitiesInScene.push_back({});
	entries.entityToEntityInScene.push_back({});

}
void ECS::SceneManager::AddNamedEntityToScene(Entity scene, Entity entity, const std::string & name)noexcept
{
	StartProfile;
	if (!initInfo.entityManager->IsAlive(entity))
		return;

	if (auto find = entityToEntry.find(scene); find == entityToEntry.end())
		return;
	else
	{
		auto& sceneMap = entries.entityToEntityInScene[find->second];
		if (auto findInScene = sceneMap.find(entity); findInScene != sceneMap.end())
		{
			auto& inScene = entries.entitiesInScene[find->second];
			inScene[findInScene->second].name = name;
		}
		else
		{
			auto& inScene = entries.entitiesInScene[find->second];
			uint32_t index = static_cast<uint32_t>(inScene.size());
			inScene.push_back({ entity, name });
			sceneMap[entity] = index;
		}

	}
}
void ECS::SceneManager::AddEntityToScene(Entity scene, Entity entity)noexcept
{
	StartProfile;
	if (!initInfo.entityManager->IsAlive(entity))
		return;

	if (auto find = entityToEntry.find(scene); find == entityToEntry.end())
		return;
	else
	{
		auto& sceneMap = entries.entityToEntityInScene[find->second];
		if (auto findInScene = sceneMap.find(entity); findInScene != sceneMap.end())
			return;

		auto& inScene = entries.entitiesInScene[find->second];
		uint32_t index = static_cast<uint32_t>(inScene.size());
		inScene.push_back({ entity, {} });
		sceneMap[entity] = index;


	}
}

void ECS::SceneManager::AddEntitiesToScene(Entity scene, const Entity entities[], uint32_t numEntities)noexcept
{
	StartProfile;
	
	if (auto find = entityToEntry.find(scene); find == entityToEntry.end())
		return;
	else
	{
		auto& sceneMap = entries.entityToEntityInScene[find->second];
		for (uint32_t i = 0; i < numEntities; i++)
		{
			if (!initInfo.entityManager->IsAlive(entities[i]))
				continue;

		
			if (auto findInScene = sceneMap.find(entities[i]); findInScene != sceneMap.end())
				continue;

			auto& inScene = entries.entitiesInScene[find->second];
			uint32_t index = static_cast<uint32_t>(inScene.size());
			inScene.push_back({ entities[i],{} });
			sceneMap[entities[i]] = index;

		}
	}
}

uint32_t ECS::SceneManager::GetNumberOfEntitiesInScene(Entity scene)const noexcept
{
	StartProfile;

	if (auto find = entityToEntry.find(scene); find == entityToEntry.end())
		return 0;
	else
	{
		return entries.entitiesInScene[find->second].size();
	}
}

void ECS::SceneManager::GetEntitiesInScene(Entity scene, Entity entities[])const noexcept
{

	StartProfile;

	if (auto find = entityToEntry.find(scene); find == entityToEntry.end())
		return;
	else
	{
		memcpy(entities, entries.entitiesInScene[find->second].data(), sizeof(Entity)* entries.entitiesInScene[find->second].size());
	}
}

void ECS::SceneManager::Destroy(Entity entity)noexcept
{
	StartProfile;
	if (auto find = entityToEntry.find(entity); find == entityToEntry.end())
		return;
	else
	{
		uint32_t last = static_cast<uint32_t>(entries.entity.size() - 1);


		entries.entity[find->second] = entries.entity[last];
		entries.entitiesInScene[find->second] = entries.entitiesInScene[last];
		entries.entityToEntityInScene[find->second] = entries.entityToEntityInScene[last];

		entries.entity.pop_back();
		entries.entitiesInScene.pop_back();
		entries.entityToEntityInScene.pop_back();

		entityToEntry[entries.entity[find->second]] = find->second;
	}
}

void ECS::SceneManager::DestroyEntities(const Entity entities[], uint32_t numEntities)noexcept
{
	StartProfile;
	for (uint32_t i = 0; i < numEntities; i++)
	{
		if (auto find = entityToEntry.find(entities[i]); find == entityToEntry.end())
			return;
		else
		{
			uint32_t last = static_cast<uint32_t>(entries.entity.size() - 1);


			entries.entity[find->second] = entries.entity[last];
			entries.entitiesInScene[find->second] = entries.entitiesInScene[last];
			entries.entityToEntityInScene[find->second] = entries.entityToEntityInScene[last];

			entries.entity.pop_back();
			entries.entitiesInScene.pop_back();
			entries.entityToEntityInScene.pop_back();

			entityToEntry[entries.entity[find->second]] = find->second;
		}
	}
}

uint32_t ECS::SceneManager::GetNumberOfRegisteredEntities() const noexcept
{
	return entries.entity.size();
}

void ECS::SceneManager::GetRegisteredEntities(Entity entities[], uint32_t numEntities) const noexcept
{
	StartProfile;
	if (numEntities != entries.entity.size())
		return;
	memcpy(entities, entries.entity.data(), sizeof(Entity)*numEntities);
}

void ECS::SceneManager::Frame()noexcept
{
	StartProfile;
}

uint64_t ECS::SceneManager::GetMemoryUsage() const noexcept
{
	return uint64_t();
}

void ECS::SceneManager::ShrinkToFit() noexcept
{
}



bool ECS::SceneManager::IsRegistered(Entity entity) const noexcept
{
	return entityToEntry.find(entity) != entityToEntry.end();
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
