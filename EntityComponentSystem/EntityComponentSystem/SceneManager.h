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
		virtual	void Create(Entity entity,const std::string& name)noexcept override;
		virtual void AddEntityToScene(Entity scene, Entity entity)noexcept override;
		virtual void AddEntitiesToScene(Entity scene, const Entity entities[], uint32_t numEntities)noexcept override;
		virtual void AddEntityToScene(Entity scene, Entity entity, const std::string& name)noexcept override;
		virtual void RemoveEntityFromScene(Entity scene, Entity entity)noexcept override;
		virtual uint32_t GetNumberOfEntitiesInScene(Entity scene)const noexcept override;
		virtual void GetEntitiesInScene(Entity scene, Entity entities[])const noexcept override;
		virtual void RegisterManager(Manager_Base* manager)noexcept override;
		virtual const char* GetNameOfScene(Entity scene)const noexcept override;
		virtual const char* GetNameOfEntityInScene(Entity scene, Entity entity)const noexcept override;
		virtual void GetChildResourcesOfSceneResource(ResourceHandler::Resource resource, std::vector<Utilz::GUID>& resources)const noexcept override;


		virtual void CreateFromResource(Entity entity, ResourceHandler::Resource resource)noexcept override;
		virtual void CreateFromStream(Entity entity, std::istream* stream)noexcept override;
		virtual uint64_t GetDataWriter(Entity entity, std::function<bool(std::ostream* file)>& writer)const noexcept override;

		virtual bool IsRegistered(Entity entity)const noexcept override;
		virtual void Destroy(Entity entity)noexcept override;
		virtual void DestroyEntities(const Entity entities[], uint32_t numEntities)noexcept override;
		virtual void DestroyAll()noexcept override;
		virtual uint32_t GetNumberOfRegisteredEntities()const noexcept override;
		virtual void GetRegisteredEntities(Entity entities[], uint32_t numEntities)const noexcept override;

		virtual void Frame()noexcept override;
		virtual Utilz::GUID GetManagerType()const noexcept override;

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

		struct SceneEntries
		{
			std::vector<Entity> entity;
			std::vector<std::string> name;
			std::vector<std::vector<Entity>> entitiesInScene;
			std::vector<std::vector<std::string>> entityNamesInScene;
			std::vector<std::unordered_map<Entity, uint32_t, Entity::Hasher>> entityToEntityInScene;
			//std::vector<bool> update;
		}entries;
		std::unordered_map<Entity, uint32_t, Entity::Hasher> entityToEntry;
	};
}
#endif