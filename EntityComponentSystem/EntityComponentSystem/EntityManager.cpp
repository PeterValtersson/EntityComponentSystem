#include "EntityManager.h"
#include <Profiler.h>
#include <Managers\Manager_Base.h>
namespace ECS
{
	EntityManager::EntityManager()
	{
	}

	

	Entity EntityManager::Create()noexcept
	{
		StartProfile;
		if (freeIndices.size())
		{
			auto top = freeIndices.top();
			freeIndices.pop();
			return Entity(generation[top], top);
		}

		uint32_t index = static_cast<uint32_t>(generation.size());
		generation.push_back(0);
		return Entity(0, index);
	}
	void EntityManager::CreateMultiple(Entity entities[], uint32_t numEntities)noexcept
	{
		StartProfile;
		for (uint32_t i = 0; i < numEntities; i++)
		{
			if (freeIndices.size())
			{
				auto top = freeIndices.top();
				freeIndices.pop();
				entities[i] = Entity(generation[top], top);
			}

			uint32_t index = static_cast<uint32_t>(generation.size());
			generation.push_back(0);
			entities[i] = Entity(0, index);
		}
	}

	bool EntityManager::IsAlive(Entity entity)noexcept
	{
		StartProfile;
		if (generation.size() <= entity.Index())
			return false;
		return generation[entity.Index()] == entity.Gen();
	}
	void EntityManager::Destroy(Entity entity)noexcept
	{
		StartProfile;
		if (generation.size() <= entity.Index())
			return;
		if (generation[entity.Index()] == entity.Gen())
		{
			generation[entity.Index()]++;
			freeIndices.push(entity.Index());
		}
	}


	void EntityManager::DestroyMultiple(const Entity entities[], uint32_t numEntities)noexcept
	{
		StartProfile;
		for (uint32_t i = 0; i < numEntities; i++)
		{
			if (generation.size() <= entities[i].Index())
				continue;
			if (generation[entities[i].Index()] == entities[i].Gen())
			{
				generation[entities[i].Index()]++;
				freeIndices.push(entities[i].Index());
			}
		}
	}
	void EntityManager::DestroyMultipleNow(const Entity entities[], uint32_t numEntities)noexcept
	{
		StartProfile;
		for (auto m : managers)
			m->DestroyEntities(entities, numEntities);
		for (uint32_t i = 0; i < numEntities; i++)
		{
			if (generation.size() <= entities[i].Index())
				continue;
			
			if (generation[entities[i].Index()] == entities[i].Gen())
			{
				
				generation[entities[i].Index()]++;
				freeIndices.push(entities[i].Index());
			}
		}
	}
	void EntityManager::DestroyNow(Entity entity)noexcept
	{
		StartProfile;
		if (generation.size() <= entity.Index())
			return;
		if (generation[entity.Index()] == entity.Gen())
		{
			for (auto m : managers)
				m->Destroy(entity);
			generation[entity.Index()]++;
			freeIndices.push(entity.Index());
		}
	}
	void EntityManager::DestroyAll(bool immediate)noexcept
	{
		StartProfile;
		if (immediate)
		{
			auto entities = new Entity[generation.size()];
			for (uint32_t i = 0; i < generation.size(); i++)
			{
				entities[i] = Entity(generation[i], i);
				for (auto m : managers)
					m->DestroyEntities(entities, static_cast<uint32_t>(generation.size()));
			}
		}
		generation.clear();
		freeIndices = std::stack<decltype(Entity::id)>();
	}
	uint32_t EntityManager::GetNumberOfAliveEntities() const noexcept
	{
		return static_cast<uint32_t>(generation.size()) - static_cast<uint32_t>(freeIndices.size());
	}
	void EntityManager::RegisterManagerForDestroyNow(Manager_Base * manager)noexcept
	{
		StartProfile;
		for (auto& m : managers)
			if (m == manager)
				return;
		managers.push_back(manager);
	}
	void EntityManager::UnregisterManagerForDestroyNow(Manager_Base * manager)noexcept
	{
		StartProfile;
		for(size_t i = 0; i < managers.size(); i++)
			if (managers[i] == manager)
			{
				managers[i] = managers[managers.size() - 1];
				managers.pop_back();
			}
	}
	uint64_t EntityManager::GetMemoryUsage() const noexcept
	{
		StartProfile;
		uint64_t size = 0;
		size += generation.size() * sizeof(Entity::GENERATION_TYPE);
		size += freeIndices.size() * sizeof(decltype(Entity::id));
		size += managers.size() * sizeof(Manager_Base*);
		return  size;
	}
	void EntityManager::ShrinkToFit()noexcept
	{
		StartProfile;
		generation.shrink_to_fit();
		managers.shrink_to_fit();
	}
	void EntityManager::WriteToFile(std::ofstream & file) const
	{
		StartProfile;
		uint32_t size = static_cast<uint32_t>(generation.size());
		file.write((char*)&size, sizeof(size));
		file.write((char*)generation.data(), size* sizeof(Entity::GENERATION_TYPE));

		std::vector<decltype(Entity::id)> fi;
		auto tempInds = freeIndices;
		while (!tempInds.empty())
		{
			fi.push_back(tempInds.top());
			tempInds.pop();
		}		

		size = static_cast<uint32_t>(fi.size());
		file.write((char*)&size, sizeof(size));
		file.write((char*)fi.data(), size * sizeof(decltype(Entity::id)));

	}

	void EntityManager::CreateFromFile(std::ifstream & file)
	{
		StartProfile;
		uint32_t size = 0;
		file.read((char*)&size, sizeof(size));
		generation.resize(size);
		file.read((char*)generation.data(), size * sizeof(Entity::GENERATION_TYPE));

		file.read((char*)&size, sizeof(size));
		std::vector<decltype(Entity::id)> fi;
		fi.resize(size);
		freeIndices = std::stack<decltype(Entity::id)>();
		file.read((char*)fi.data(), size * sizeof(decltype(Entity::id)));
		for (auto i : fi)
			freeIndices.push(i);
	}

}