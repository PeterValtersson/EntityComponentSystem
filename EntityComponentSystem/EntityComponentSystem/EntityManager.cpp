#include "EntityManager.h"
namespace ECS
{
	EntityManager::EntityManager()
	{
	}

	Entity EntityManager::Create()
	{
		if (freeIndices.size())
		{
			auto top = freeIndices.top();
			freeIndices.pop();
			return Entity(generation[top], top);
		}

		uint32_t index = generation.size();
		generation.push_back(0);
		return Entity(0, index);
	}

	bool EntityManager::IsAlive(Entity entity)
	{
		if (generation.size() <= entity.Index())
			return false;
		return generation[entity.Index()] == entity.Gen();
	}
	void EntityManager::Destroy(Entity entity)
	{
		if (generation.size() <= entity.Index())
			return;
		if (generation[entity.Index()] == entity.Gen())
		{
			generation[entity.Index()]++;
			freeIndices.push(entity.Index());
		}
	}
	void EntityManager::DestroyNow(Entity entity)
	{
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
	void EntityManager::DestroyAll(bool immediate)
	{
		if (immediate)
		{
			auto entities = new Entity[generation.size()];
			for (uint32_t i = 0; i < generation.size(); i++)
			{
				entities[i] = Entity(generation[i], i);
				for (auto m : managers)
					m->DestroyEntities(entities, generation.size());
			}
		}
		generation.clear();
		freeIndices = std::stack<decltype(Entity::id)>();
	}
	uint32_t EntityManager::GetNumberOfAliveEntities() const
	{
		return generation.size() - freeIndices.size();
	}
	void EntityManager::RegisterManagerForDestroyNow(Manager_Base * manager)
	{
		for (auto& m : managers)
			if (m == manager)
				return;
		managers.push_back(manager);
	}
	void EntityManager::UnregisterManagerForDestroyNow(Manager_Base * manager)
	{
		for(size_t i = 0; i < managers.size(); i++)
			if (managers[i] == manager)
			{
				managers[i] = managers[managers.size() - 1];
				managers.pop_back();
			}
	}
	uint64_t EntityManager::GetMemoryUsage() const
	{
		uint64_t size = 0;
		size += generation.size() * sizeof(Entity::GENERATION_TYPE);
		size += freeIndices.size() * sizeof(decltype(Entity::id));
		size += managers.size() * sizeof(Manager_Base*);
		return  size;
	}
	void EntityManager::ShrinkToFit()
	{
		generation.shrink_to_fit();
		managers.shrink_to_fit();
	}
	void EntityManager::WriteToFile(std::ofstream & file) const
	{
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