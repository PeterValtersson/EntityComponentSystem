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
		//virtual void AddNamedEntityToScene(Entity scene, Entity entity, Utilz::GUID guid)override;
		virtual uint32_t GetNumberOfEntitiesInScene(Entity scene)const noexcept override;
		virtual void GetEntitiesInScene(Entity scene, Entity entities[])const noexcept override;


		virtual Entity CreateFromResource(ResourceHandler::Resource resource)noexcept override;
		virtual std::function<bool(std::fstream&file)> GetDataWriter(Entity entity)const noexcept override;

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
		SceneManagerInitializationInfo initInfo;
		struct EntityInfo
		{
			Entity entity;
			//Resource entityDetails
		};
		struct SceneEntries
		{
			std::vector<Entity> entity;
			std::vector<std::vector<Entity>> entitiesInScene;
			//std::vector<bool> update;
		}entries;
		std::unordered_map<Entity, uint32_t, Entity::Hasher> entityToEntry;
	};
}
#endif