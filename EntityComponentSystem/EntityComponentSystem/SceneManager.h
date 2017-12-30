#ifndef _ECS_SCENE_MANAGER_H_
#define _ECS_SCENE_MANAGER_H_
#include <Managers\SceneManager_Interface.h>
#include <unordered_map>
namespace ECS
{
	class SceneManager : public SceneManager_Interface
	{
	public:
		SceneManager(SceneManagerInitializationInfo ii);
		virtual ~SceneManager();
		virtual	void Create(Entity entity)noexcept override;
		virtual void AddEntityToScene(Entity scene, Entity entity)noexcept override;
		virtual void AddEntitiesToScene(Entity scene, const Entity entities[], uint32_t numEntities)noexcept override;
		virtual void AddNamedEntityToScene(Entity scene, Entity entity, const std::string& name)noexcept override;
		virtual uint32_t GetNumberOfEntitiesInScene(Entity scene)const noexcept override;
		virtual void GetEntitiesInScene(Entity scene, Entity entities[])const noexcept override;
		virtual void RegisterManager(Manager_Base* manager)noexcept override;

		virtual void CreateFromResource(Entity entity, ResourceHandler::Resource resource)noexcept override;
		virtual void CreateFromStream(Entity entity, std::istream* stream)noexcept override;
		virtual std::function<bool(std::ostream* file)> GetDataWriter(Entity entity)const noexcept override;

		virtual bool IsRegistered(Entity entity)const noexcept override;
		virtual void Destroy(Entity entity)noexcept override;
		virtual void DestroyEntities(const Entity entities[], uint32_t numEntities)noexcept override;

		virtual uint32_t GetNumberOfRegisteredEntities()const noexcept override;
		virtual void GetRegisteredEntities(Entity entities[], uint32_t numEntities)const noexcept override;

		virtual void Frame()noexcept override;

		virtual uint64_t GetMemoryUsage()const noexcept override;
		virtual void ShrinkToFit()noexcept override;
		virtual void WriteToFile(std::ofstream& file)const;
		virtual void CreateFromFile(std::ifstream& file);
	protected:
		virtual void GarbageCollection()noexcept override;

	private:
		uint32_t version = 0;
		std::vector<Manager_Base*> managers;

		SceneManagerInitializationInfo initInfo;
		struct EntityInfo
		{
			Entity entity;
			std::string name;
		};
		struct SceneEntries
		{
			std::vector<Entity> entity;
			std::vector<std::vector<EntityInfo>> entitiesInScene;
			std::vector<std::unordered_map<Entity, uint32_t, Entity::Hasher>> entityToEntityInScene;
			//std::vector<bool> update;
		}entries;
		std::unordered_map<Entity, uint32_t, Entity::Hasher> entityToEntry;
	};
}
#endif