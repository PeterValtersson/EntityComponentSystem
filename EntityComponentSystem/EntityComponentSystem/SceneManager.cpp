#include "SceneManager.h"
#include <Profiler.h>
#include <Utilz\StringReadWrite.h>
ECS::SceneManager::SceneManager(SceneManagerInitializationInfo ii) : initInfo(ii)
{
	_ASSERT_EXPR(initInfo.entityManager, "SceneManager must have entitymanager");
}

ECS::SceneManager::~SceneManager()
{
}

void ECS::SceneManager::RemoveEntityFromScene(Entity scene, Entity entity)noexcept
{
	if (auto find = entityToEntry.find(scene); find == entityToEntry.end())
		return;
	else
	{
		auto& map = entries.entityToEntityInScene[find->second];
		if (auto findE = map.find(entity); findE == map.end())
			return;
		else
		{
			auto& ents = entries.entitiesInScene[find->second];
			auto& names = entries.entityNamesInScene[find->second];
			uint32_t last = static_cast<uint32_t>(ents.size() - 1);
			auto lastE = ents[last];

			ents[findE->second] = ents[last];
			names[findE->second] = names[last];

			ents.pop_back();
			names.pop_back();

			map[lastE] = findE->second; 
			map.erase(entity);
		}
	}
}

const char * ECS::SceneManager::GetNameOfEntityInScene(Entity scene, Entity entity) const noexcept
{

	if (auto find = entityToEntry.find(scene); find == entityToEntry.end())
		return nullptr;
	else
	{
		auto& map = entries.entityToEntityInScene[find->second];
		if (auto findE = map.find(entity); findE == map.end())
			return nullptr;
		else
		{
			return entries.entityNamesInScene[find->second][findE->second].c_str();
		}
	}
}

const char * ECS::SceneManager::GetNameOfScene(Entity scene) const noexcept
{
	if (auto find = entityToEntry.find(scene); find == entityToEntry.end())
		return nullptr;
	else
	{
		return entries.name[find->second].c_str();
	}
}



void ECS::SceneManager::RegisterManager(Manager_Base * manager)noexcept
{
	StartProfile;
	for (auto& m : managers)
		if (m == manager)
			return;
	managers.push_back(manager);
}

void ECS::SceneManager::CreateFromResource(Entity entity, ResourceHandler::Resource resource)noexcept
{
}

void ECS::SceneManager::CreateFromStream(Entity entity, std::istream * stream)noexcept
{
}

uint64_t ECS::SceneManager::GetDataWriter(Entity entity, std::function<bool(std::ostream* file)>& writer)const noexcept
{
	if (auto findf = entityToEntry.find(entity); findf == entityToEntry.end())
		return 0;
	else
	{



		uint64_t size = sizeof(version) + sizeof(uint32_t)*2 + static_cast<uint64_t>(entries.name[findf->second].size());
		auto& ents = entries.entitiesInScene[findf->second];
		auto& names = entries.entityNamesInScene[findf->second];
		struct Component
		{
			Utilz::GUID type;
			std::function<bool(std::ostream* file)> writer;
		};
		struct WriteInfo
		{
			std::string name;
			uint32_t componentCount;
			std::vector<Component> eInfo;
		};
		std::vector<WriteInfo> writeInfo;
		writeInfo.reserve(ents.size());
		for(size_t i = 0; i < ents.size(); i++)
		{
			auto& e = ents[i];
		
			WriteInfo info{ names[i], 0 };
			size += sizeof(uint32_t) + static_cast<uint64_t>(info.name.size());
			size += sizeof(uint32_t);
			for (auto m : managers)
			{
				if (m->IsRegistered(e))
				{
					Component comp{ m->GetManagerType() };
				
					auto writerSize = m->GetDataWriter(e, comp.writer);
					if (writerSize == 0)
						continue;
					size += sizeof(comp.type);
					size += writerSize;
					info.eInfo.push_back(std::move(comp));
				}

			}
			info.componentCount = static_cast<uint32_t>(info.eInfo.size());
			writeInfo.push_back(std::move(info));
		}

		writer = [entity, this,writeInfo](std::ostream* file) {

			if (auto find = entityToEntry.find(entity); find == entityToEntry.end())
				return false;
			else
			{
				auto& ents = entries.entitiesInScene[find->second];

				file->write((char*)&version, sizeof(version));
				Utilz::writeString(file, entries.name[find->second]);
				uint32_t numEnt = static_cast<uint32_t>(ents.size());
				file->write((char*)&numEnt, sizeof(numEnt));
				for (auto& info : writeInfo)
				{
					Utilz::writeString(file, info.name);
					file->write((char*)&info.componentCount, sizeof(info.componentCount));
					for (auto& comp : info.eInfo)
					{
						file->write((char*)&comp.type, sizeof(comp.type));
						if (!comp.writer(file))
							return false;
					}
				}



				return true;
			}};



		return 0;
	}
}



void ECS::SceneManager::Create(Entity entity, const std::string& name)noexcept
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
	entries.entityNamesInScene.push_back({});
	entries.name.push_back(name);

	entityToEntry[entity] = index;
}
void ECS::SceneManager::AddEntityToScene(Entity scene, Entity entity, const std::string & name)noexcept
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
			auto& names = entries.entityNamesInScene[find->second];
			names[findInScene->second] = name;
		}
		else
		{
			auto& ents = entries.entitiesInScene[find->second];
			auto& names = entries.entityNamesInScene[find->second];
			uint32_t index = static_cast<uint32_t>(ents.size());
			ents.push_back(entity);
			names.push_back(name);
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

		auto& ents = entries.entitiesInScene[find->second];
		auto& names = entries.entityNamesInScene[find->second];
		uint32_t index = static_cast<uint32_t>(ents.size());
		ents.push_back(entity);
		names.push_back({});
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

			auto& ents = entries.entitiesInScene[find->second];
			auto& names = entries.entityNamesInScene[find->second];
			uint32_t index = static_cast<uint32_t>(ents.size());
			ents.push_back(entities[i]);
			names.push_back({});
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
		return static_cast<uint32_t>(entries.entitiesInScene[find->second].size());
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
		Entity lastE = entries.entity[last];

		entries.entity[find->second] = entries.entity[last];
		entries.entitiesInScene[find->second] = entries.entitiesInScene[last];
		entries.entityToEntityInScene[find->second] = entries.entityToEntityInScene[last];
		entries.entityNamesInScene[find->second] = entries.entityNamesInScene[last];
		entries.name[find->second] = entries.name[last];

		entries.entity.pop_back();
		entries.name.pop_back();
		entries.entitiesInScene.pop_back();
		entries.entityNamesInScene.pop_back();
		entries.entityToEntityInScene.pop_back();

		entityToEntry[lastE] = find->second;
		entityToEntry.erase(entity);
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
			entries.name[find->second] = entries.name[last];

			entries.entity.pop_back();
			entries.name.pop_back();
			entries.entitiesInScene.pop_back();
			entries.entityToEntityInScene.pop_back();

			entityToEntry[entries.entity[find->second]] = find->second;
		}
	}
}

uint32_t ECS::SceneManager::GetNumberOfRegisteredEntities() const noexcept
{
	return static_cast<uint32_t>(entries.entity.size());
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

Utilz::GUID ECS::SceneManager::GetManagerType() const noexcept
{
	return "SceneManager";
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
